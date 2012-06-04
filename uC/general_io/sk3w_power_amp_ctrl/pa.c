/*! \file     general_io/sk3w_power_amp_ctrl/pa.c
 *  \ingroup  general_io_group
 *  \brief    PA state machine base
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-05-23
 * 
 * Implements the base for the PA state machine. The actual state
 * handling is done in the auto generated file pasm.c, which is included at
 * the bottom of this file.
 */
//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <string.h>

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/crc16.h>

#include <global.h>
#include <wmv_bus/bus_commands.h>

#include "qpn_port.h"
#include "bsp.h"
#include "bus_handler.h"
#include "pa.h"


Q_DEFINE_THIS_FILE              /* Define file name to make assertions work */


#define DEFAULT_UNUSED_TIMEOUT    2*60*60       /* 2 hours */
#define DEFAULT_WARMUP_TIMEOUT    3*60          /* 3 minutes */
#define DEFAULT_COOLDOWN_TIMEOUT  2*60          /* 2 minutes */
#define SM_COUNT                  6
#define SM_QUEUE_LEN              3
#define SM_UNUSED                 255
//#define DEBUG_PRINT(str...) send_ascii_data(0, str);
#define DEBUG_PRINT(str...)


//! Signals (events) for the PA state machine
enum PaSignals {
  TOGGLE_MAINS_SIG = Q_USER_SIG,
  TX_ACTIVE_ON_SIG,
  TX_ACTIVE_OFF_SIG,
  BAND_SELECTED_SIG,
  BAND_UNSELECTED_SIG
};


//! Include the tate machine declaration
#include "pasm.h"

//! Configuration variables that are persisted in the EEPROM
typedef struct {
  uint16_t unused_timeout;      //! The "unused" timeout in seconds
  uint16_t warmup_timeout;      //! The "warmup" timeout in seconds
  uint16_t cooldown_timeout;    //! The "cooldown" timeout in seconds
} Config;


//! EEPROM config variable container
static Config EEMEM ee_cfg;

//! EEPROM CRC
static uint16_t EEMEM ee_crc;

//! Config variable container
static Config cfg;

//! Event queues for the state machines
static QEvent pa_queue[SM_COUNT][SM_QUEUE_LEN];

//! One state machine for each PA
static Pa pa_sm[SM_COUNT];

//! Band to state machine map
static uint8_t band2sm[BAND_MAX + 1];

//! State machine to band map
static const uint8_t sm2band[SM_COUNT] = {
  BAND_160M, BAND_80M, BAND_40M, BAND_20M, BAND_15M, BAND_10M
};

//! QF_active[] array defines all active object control blocks
QActiveCB const Q_ROM Q_ROM_VAR QF_active[SM_COUNT + 1] = {
  {(QActive *)0,         (QEvent *)0, 0                 },
  {(QActive *)&pa_sm[0], pa_queue[0], Q_DIM(pa_queue[0])},
  {(QActive *)&pa_sm[1], pa_queue[1], Q_DIM(pa_queue[1])},
  {(QActive *)&pa_sm[2], pa_queue[2], Q_DIM(pa_queue[2])},
  {(QActive *)&pa_sm[3], pa_queue[3], Q_DIM(pa_queue[3])},
  {(QActive *)&pa_sm[4], pa_queue[4], Q_DIM(pa_queue[4])},
  {(QActive *)&pa_sm[5], pa_queue[5], Q_DIM(pa_queue[5])},
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


static int8_t post_event(uint8_t subaddr, enum PaSignals sig, QParam par);
static void save_config(void);
static void read_config(void);

static void Pa_ctor(Pa *pa, uint8_t band);
static void Pa_setOpStatus(Pa *pa, uint8_t op_status);
static void Pa_setCtrlr(Pa *pa, uint8_t ctrlr);


void pa_init(void) {
  read_config();
  
  memset(band2sm, SM_UNUSED, sizeof(band2sm));
  for (int i = 0; i < SM_COUNT; ++i) {
      /* Set up the band to state machine map to match the
       * state machine to band map */
    band2sm[sm2band[i]] = i;

      /* Call the state machine constructor */
    Pa_ctor(&pa_sm[i], sm2band[i]);
  }
}

void pa_set_unused_timeout(uint16_t new_timeout) {
  if (new_timeout < 1) {
    new_timeout = 1;
  }
  cfg.unused_timeout = new_timeout;
  save_config();
}

uint16_t pa_unused_timeout(void) {
  return cfg.unused_timeout;
}

void pa_set_warmup_timeout(uint16_t new_timeout) {
  if (new_timeout < 1) {
    new_timeout = 1;
  }
  cfg.warmup_timeout = new_timeout;
  save_config();
}

uint16_t pa_warmup_timeout(void) {
  return cfg.warmup_timeout;
}

void pa_set_cooldown_timeout(uint16_t new_timeout) {
  if (new_timeout < 1) {
    new_timeout = 1;
  }
  cfg.cooldown_timeout = new_timeout;
  save_config();
}

uint16_t pa_cooldown_timeout(void) {
  return cfg.cooldown_timeout;
}

void pa_set_default_config(void) {
  cfg.unused_timeout = DEFAULT_UNUSED_TIMEOUT;
  cfg.warmup_timeout = DEFAULT_WARMUP_TIMEOUT;
  cfg.cooldown_timeout = DEFAULT_COOLDOWN_TIMEOUT;
  save_config();
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
  if (sm != SM_UNUSED) {
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
  if (sm != SM_UNUSED) {
    return pa_sm[sm].op_status;
  }
  return AMP_OP_STATUS_OFF;
}


/**
 * \brief   Send an event to the state machine associated with the given band
 * \param   band  The band (e.g. BAND_160M)
 * \param   sig   The event (signal) to send to the state machine
 * \param   par   The event parameter
 */
static int8_t post_event(uint8_t band, enum PaSignals sig, QParam par) {
  Q_REQUIRE(band <= BAND_MAX);
  uint8_t sm = band2sm[band];
  if (sm != SM_UNUSED) {
    QActive_post((QActive *)&pa_sm[sm], sig, par);
    return 0;
  }
  return -1;
}

/**
 * \brief   Save configuration to the EEPROM
 */
static void save_config(void) {
  eeprom_update_block(&cfg, &ee_cfg, sizeof(cfg));
  uint16_t calculated_crc = 0xffff;
  uint8_t *ptr = (uint8_t *)&cfg;
  for (int i=0; i<sizeof(cfg); ++i) {
    calculated_crc = _crc_ccitt_update(calculated_crc, *ptr++);
  }
  eeprom_update_word(&ee_crc, calculated_crc);
}

/**
 * \brief   Read configuration from the EEPROM
 */
static void read_config(void) {
  eeprom_read_block(&cfg, &ee_cfg, sizeof(cfg));
  uint16_t calculated_crc = 0xffff;
  uint8_t *ptr = (uint8_t *)&cfg;
  for (int i=0; i<sizeof(cfg); ++i) {
    calculated_crc = _crc_ccitt_update(calculated_crc, *ptr++);
  }
  uint16_t crc = eeprom_read_word(&ee_crc);
  if (crc != calculated_crc) {
    pa_set_default_config();
  }
}


/**
 * \brief Constructor for the state machine
 * \param pa    The Pa object to be initialized
 * \param band  The band (e.g. BAND_160M)
 */
static void Pa_ctor(Pa *pa, uint8_t band) {
  pa->band = band;
  pa->ctrlr = PA_CTRLR_UNUSED;
  pa->op_status = AMP_OP_STATUS_OFF;
  QActive_ctor((QActive *)pa, (QStateHandler)&Pa_initial);
}

/**
 * \brief Set the operational status also calling pa_op_status_changed
 * \param pa        A previously initialized Pa object
 * \param op_status The new operational staus to set (one of AMP_OP_STATUS_*)
 */
static void Pa_setOpStatus(Pa *pa, uint8_t op_status) {
  pa->op_status = op_status;
  pa_op_status_changed(pa->band);
}

/**
 * \brief Set the controller also calling pa_ctrlr_changed
 * \param pa    A previously initialized Pa object
 * \param ctrlr The new controller to set
 */
static void Pa_setCtrlr(Pa *pa, uint8_t ctrlr) {
  pa->ctrlr = ctrlr;
  pa_ctrlr_changed(pa->band);
}

//! Include the state machine definition
#include "pasm.c"
