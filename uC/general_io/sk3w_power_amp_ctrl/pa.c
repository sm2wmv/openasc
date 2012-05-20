/*****************************************************************************
* Model: 
* File:  ./pa.c
*
* Please visit www.state-machine.com/qm for more information.
*****************************************************************************/
#include <string.h>

#include <avr/io.h>

#include <global.h>
#include <wmv_bus/bus_commands.h>

#include "qpn_port.h"
#include "bsp.h"
#include "bus_handler.h"
#include "pa.h"


Q_DEFINE_THIS_FILE  /* Define file name to make assertions work */


#define UNUSED_TIMEOUT    2*60*60   /* 2 hours */
#define WARMUP_TIMEOUT    3*60      /* 3 minutes */
#define COOLDOWN_TIMEOUT  2*60      /* 2 minutes */
#define SM_COUNT          6
#define SM_QUEUE_LEN      3
#define SM_UNUSED         255

//#define DEBUG_PRINT(str...) send_ascii_data(0, str);
#define DEBUG_PRINT(str...)


enum PaSignals {
	TOGGLE_MAINS_SIG = Q_USER_SIG,
	TX_ACTIVE_ON_SIG,
	TX_ACTIVE_OFF_SIG,
	BAND_SELECTED_SIG,
	BAND_UNSELECTED_SIG
};

/** 
* PA state machine class
*/
typedef struct PaTag {
/* protected: */
    QActive super;

/* private: */
		uint8_t band;
		uint8_t ctrlr;
		uint8_t op_status;
} Pa;


//! Event queues for the state machines
static QEvent pa_queue[SM_COUNT][SM_QUEUE_LEN];

//! One state machine for each PA
static Pa pa_sm[SM_COUNT];

//! Band to state machine map
static uint8_t band2sm[BAND_MAX+1];

//! State machine to band map
static const uint8_t sm2band[SM_COUNT] = {
	BAND_160M, BAND_80M, BAND_40M, BAND_20M, BAND_15M, BAND_10M
};

//! QF_active[] array defines all active object control blocks
QActiveCB const Q_ROM Q_ROM_VAR QF_active[SM_COUNT+1] = {
    { (QActive *)0,         (QEvent *)0, 0                  },
    { (QActive *)&pa_sm[0], pa_queue[0], Q_DIM(pa_queue[0]) },
    { (QActive *)&pa_sm[1], pa_queue[1], Q_DIM(pa_queue[1]) },
    { (QActive *)&pa_sm[2], pa_queue[2], Q_DIM(pa_queue[2]) },
    { (QActive *)&pa_sm[3], pa_queue[3], Q_DIM(pa_queue[3]) },
    { (QActive *)&pa_sm[4], pa_queue[4], Q_DIM(pa_queue[4]) },
    { (QActive *)&pa_sm[5], pa_queue[5], Q_DIM(pa_queue[5]) },
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


static int8_t post_event(uint8_t subaddr, enum PaSignals sig, QParam par);

static void Pa_ctor(Pa *pa, uint8_t band);
static QState Pa_initial(Pa *me);
static QState Pa_powerOff(Pa *me);
static QState Pa_powerOn(Pa *me);
static QState Pa_warmup(Pa *me);
static QState Pa_operational(Pa *me);
static QState Pa_ready(Pa *me);
static QState Pa_transmitting(Pa *me);
static QState Pa_unused(Pa *me);
static QState Pa_cooldown(Pa *me);
static void Pa_setOpStatus(Pa *pa, uint8_t op_status);
static void Pa_setCtrlr(Pa *pa, uint8_t ctrlr);


void pa_init(void) {
	memset(band2sm, SM_UNUSED, sizeof(band2sm));
	for (int i=0; i<SM_COUNT; ++i) {
      /* Set up the band to state machine map to match the
       * state machine to band map */
		band2sm[sm2band[i]] = i;
    
      /* Call the state machine constructor */
    Pa_ctor(&pa_sm[i], sm2band[i]);
	}
}

void pa_set_controller(uint8_t band, uint8_t ctrlr) {
	post_event(band, BAND_SELECTED_SIG, ctrlr);
}

void pa_clear_controller(uint8_t band) {
  post_event(band, BAND_UNSELECTED_SIG, 0);
}

uint8_t pa_controller(uint8_t band) {
  Q_REQUIRE(band <= BAND_MAX);
  uint8_t sm = band2sm[band];
  if (sm != SM_UNUSED)
  {
    return pa_sm[sm].ctrlr;
  }
  return PA_CTRLR_UNUSED;
}

int8_t pa_toggle_mains(uint8_t band) {
	return post_event(band, TOGGLE_MAINS_SIG, 0);
}


void pa_set_tx_active(uint8_t band, int8_t on) {
	post_event(band, on ? TX_ACTIVE_ON_SIG : TX_ACTIVE_OFF_SIG, 0);
}


uint8_t pa_op_status(uint8_t band) {
  Q_REQUIRE(band <= BAND_MAX);
  uint8_t sm = band2sm[band];
  if (sm != SM_UNUSED)
  {
    return pa_sm[sm].op_status;
  }
  return AMP_OP_STATUS_OFF;
}


static int8_t post_event(uint8_t band, enum PaSignals sig, QParam par) {
	Q_REQUIRE(band <= BAND_MAX);
	uint8_t sm = band2sm[band];
	if (sm != SM_UNUSED)
	{
		QActive_post((QActive *)&pa_sm[sm], sig, par);
    return 0;
	}
	return -1;
}



/* Pa class definition ------------------------------------------------*/
static void Pa_ctor(Pa *pa, uint8_t band) {
	pa->band = band;
	pa->ctrlr = PA_CTRLR_UNUSED;
	pa->op_status = AMP_OP_STATUS_OFF;
  QActive_ctor((QActive *)pa, (QStateHandler)&Pa_initial);
}

static QState Pa_initial(Pa *me) {
    return Q_TRAN(&Pa_powerOff);
}


static QState Pa_powerOff(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_powerOff/ENTRY\r\n");
      Pa_setOpStatus(me, AMP_OP_STATUS_OFF);
			bsp_set_pa_mains(me->band, 0);
			return Q_HANDLED();
		}
		case TOGGLE_MAINS_SIG: {
			DEBUG_PRINT("Pa_powerOff/TOGGLE_MAINS\r\n");
			return Q_TRAN(&Pa_powerOn);
		}
		case BAND_SELECTED_SIG: {
			DEBUG_PRINT("Pa_powerOff/BAND_SELECTED\r\n");
			Pa_setCtrlr(me, Q_PAR(me));
			return Q_HANDLED();
		}
		case BAND_UNSELECTED_SIG: {
			DEBUG_PRINT("Pa_powerOff/BAND_UNSELECTED\r\n");
      Pa_setCtrlr(me, PA_CTRLR_UNUSED);
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&QHsm_top);
}


static QState Pa_powerOn(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_powerOn/ENTRY\r\n");
			bsp_set_pa_mains(me->band, 1);
			return Q_HANDLED();
		}
		case Q_INIT_SIG: {
			DEBUG_PRINT("Pa_powerOn/INIT\r\n");
			return Q_TRAN(&Pa_warmup);
		}
		case TOGGLE_MAINS_SIG: {
			DEBUG_PRINT("Pa_powerOn/TOGGLE_MAINS\r\n");
			return Q_TRAN(&Pa_cooldown);
		}
		case BAND_UNSELECTED_SIG: {
			DEBUG_PRINT("Pa_powerOn/BAND_UNSELECTED\r\n");
      Pa_setCtrlr(me, PA_CTRLR_UNUSED);
			return Q_HANDLED();
		}
		case BAND_SELECTED_SIG: {
			DEBUG_PRINT("Pa_powerOn/BAND_SELECTED\r\n");
      Pa_setCtrlr(me, Q_PAR(me));
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&QHsm_top);
}


static QState Pa_warmup(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_warmup/ENTRY\r\n");
      Pa_setOpStatus(me, AMP_OP_STATUS_WARMUP);
			QActive_arm((QActive *)me, WARMUP_TIMEOUT);
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_warmup/EXIT\r\n");
			return Q_HANDLED();
		}
		case TOGGLE_MAINS_SIG: {
			DEBUG_PRINT("Pa_warmup/TOGGLE_MAINS\r\n");
			return Q_TRAN(&Pa_powerOff);
		}
		case Q_TIMEOUT_SIG: {
			DEBUG_PRINT("Pa_warmup/TIMEOUT\r\n");
			return Q_TRAN(&Pa_operational);
		}
	}
	return Q_SUPER(&Pa_powerOn);
}


static QState Pa_operational(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_operational/ENTRY\r\n");
      Pa_setOpStatus(me, AMP_OP_STATUS_READY);
			return Q_HANDLED();
		}
		case Q_INIT_SIG: {
			DEBUG_PRINT("Pa_operational/INIT\r\n");
			if (me->ctrlr != PA_CTRLR_UNUSED) {
				return Q_TRAN(&Pa_ready);
			}
			else {
				return Q_TRAN(&Pa_unused);
			}
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_operational/EXIT\r\n");
			return Q_HANDLED();
		}
		case BAND_UNSELECTED_SIG: {
			DEBUG_PRINT("Pa_operational/BAND_UNSELECTED\r\n");
			return Q_TRAN(&Pa_unused);
		}
	}
	return Q_SUPER(&Pa_powerOn);
}


static QState Pa_ready(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_ready/ENTRY\r\n");
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_ready/EXIT\r\n");
			return Q_HANDLED();
		}
		case TX_ACTIVE_ON_SIG: {
			DEBUG_PRINT("Pa_ready/TX_ACTIVE_ON\r\n");
			return Q_TRAN(&Pa_transmitting);
		}
	}
	return Q_SUPER(&Pa_operational);
}


static QState Pa_transmitting(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_transmitting/ENTRY\r\n");
			bsp_set_pa_ptt(me->band, 1);
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_transmitting/EXIT\r\n");
			bsp_set_pa_ptt(me->band, 0);
			return Q_HANDLED();
		}
		case TX_ACTIVE_OFF_SIG: {
			DEBUG_PRINT("Pa_transmitting/TX_ACTIVE_OFF\r\n");
			return Q_TRAN(&Pa_ready);
		}
    case TOGGLE_MAINS_SIG: {
      DEBUG_PRINT("Pa_transmitting/TOGGLE_MAINS\r\n");
      return Q_HANDLED();
    }
	}
	return Q_SUPER(&Pa_operational);
}


static QState Pa_unused(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_unused/ENTRY\r\n");
      Pa_setCtrlr(me, PA_CTRLR_UNUSED);
			QActive_arm((QActive *)me, UNUSED_TIMEOUT);
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_unused/EXIT\r\n");
			return Q_HANDLED();
		}
		case Q_TIMEOUT_SIG: {
			DEBUG_PRINT("Pa_unused/TIMEOUT\r\n");
			return Q_TRAN(&Pa_powerOff);
		}
		case BAND_SELECTED_SIG: {
			DEBUG_PRINT("Pa_unused/BAND_SELECTED\r\n");
      Pa_setCtrlr(me, Q_PAR(me));
			return Q_TRAN(&Pa_operational);
		}
		case BAND_UNSELECTED_SIG: {
			DEBUG_PRINT("Pa_unused/BAND_UNSELECTED\r\n");
			return Q_HANDLED();
		}
	}
	return Q_SUPER(&Pa_operational);
}


static QState Pa_cooldown(Pa *me) {
	switch (Q_SIG(me)) {
		case Q_ENTRY_SIG: {
			DEBUG_PRINT("Pa_cooldown/ENTRY\r\n");
      Pa_setOpStatus(me, AMP_OP_STATUS_COOLDOWN);
			QActive_arm((QActive *)me, COOLDOWN_TIMEOUT);
			return Q_HANDLED();
		}
		case Q_EXIT_SIG: {
			DEBUG_PRINT("Pa_cooldown/EXIT\r\n");
			return Q_HANDLED();
		}
		case Q_TIMEOUT_SIG: {
			DEBUG_PRINT("Pa_cooldown/TIMEOUT\r\n");
			return Q_TRAN(&Pa_powerOff);
		}
		case TOGGLE_MAINS_SIG: {
			DEBUG_PRINT("Pa_cooldown/TOGGLE_MAINS\r\n");
			return Q_TRAN(&Pa_operational);
		}
	}
	return Q_SUPER(&Pa_powerOn);
}


static void Pa_setOpStatus(Pa *pa, uint8_t op_status) {
  pa->op_status = op_status;
  pa_op_status_changed(pa->band);
}

static void Pa_setCtrlr(Pa *pa, uint8_t ctrlr) {
  pa->ctrlr = ctrlr;
  pa_ctrlr_changed(pa->band);
}

