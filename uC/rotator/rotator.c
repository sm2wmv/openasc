/*! \file     rotator/rotator.c
 *  \ingroup  rotator_group
 *  \brief    Rotator state machine base
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-06-16
 *
 * Implements the base for the Rotator state machine. The actual state
 * handling is done in the auto generated file rotatorsm.c, which is included at
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

/******************************************************************************
 *
 * Standard library includes
 *
 *****************************************************************************/

#include <string.h>
#include <stdlib.h>

#include <avr/io.h>


/******************************************************************************
 *
 * Project includes
 *
 *****************************************************************************/

#include <global.h>
#include <delay.h>
#include <wmv_bus/bus_commands.h>
#include <misc/eeprom.h>


/******************************************************************************
 *
 * Local includes
 *
 *****************************************************************************/

#include "bsp.h"
#include "bus_handler.h"
#include "rotator.h"


/******************************************************************************
 *
 * Macros and type definitions
 *
 *****************************************************************************/

//! The length of each state machines event queue
#define SM_QUEUE_LEN            3
//! Macro used to print text debugging messages using the bus.
#define DEBUG_PRINT(str...)     send_ascii_data(0, str);
//#define DEBUG_PRINT(str...)


/**
 * Signals (events) for the state machine
 */
enum RotatorSignals {
  CAL_ENABLE_SIG = Q_USER_SIG,  /*!< Enter calibration mode */
  CAL_DISABLE_SIG,              /*!< Exit calibration mode */
  SET_CCW_LIMIT_SIG,            /*!< Set the CCW limit */
  SET_CW_LIMIT_SIG,             /*!< Set the CW limit */
  ROTATE_CW_SIG,                /*!< Rotate clockwise */
  ROTATE_CCW_SIG,               /*!< Rotate counter clockwise */
  STOP_SIG,                     /*!< Stop rotation */
  ROTATION_LIMIT_SIG,           /*!< A rotation limit has been reached */
  ROTATOR_ERROR_SIG             /*!< An error condition has occurred */
};


//! Include the tate machine declaration
#include "rotatorsm.h"


/******************************************************************************
 *
 * Global variable declarations
 *
 *****************************************************************************/

Q_DEFINE_THIS_FILE              /* Define file name to make assertions work */

//! Event queues for the state machines
static QEvent rotator_queue[ROTATOR_COUNT][SM_QUEUE_LEN];

//! One state machine for each Rotator
static Rotator rotator_sm[ROTATOR_COUNT];

//! QF_active[] array defines all active object control blocks
QActiveCB const Q_ROM Q_ROM_VAR QF_active[ROTATOR_COUNT + 1] = {
  {(QActive *)0,              (QEvent *)0,      0                      },
  {(QActive *)&rotator_sm[0], rotator_queue[0], Q_DIM(rotator_queue[0])},
  {(QActive *)&rotator_sm[1], rotator_queue[1], Q_DIM(rotator_queue[1])},
  {(QActive *)&rotator_sm[2], rotator_queue[2], Q_DIM(rotator_queue[2])},
  {(QActive *)&rotator_sm[3], rotator_queue[3], Q_DIM(rotator_queue[3])},
  {(QActive *)&rotator_sm[4], rotator_queue[4], Q_DIM(rotator_queue[4])}
};

/* make sure that the QF_active[] array matches QF_MAX_ACTIVE in qpn_port.h */
Q_ASSERT_COMPILE(QF_MAX_ACTIVE == Q_DIM(QF_active) - 1);


/******************************************************************************
 *
 * Function prototypes for private functions
 *
 *****************************************************************************/

static int8_t post_event(uint8_t rot_idx, enum RotatorSignals sig, QParam par);
static int8_t ccw_limit_exceeded(Rotator *me);
static int8_t cw_limit_exceeded(Rotator *me);
static int16_t range_adjust_heading(int16_t heading);
static uint16_t median_filter(Rotator *me, uint16_t adc);
static int8_t is_calibrated(Rotator *me);

static void Rotator_ctor(Rotator *rot, uint8_t rot_idx);
static void Rotator_set_ccw_limit(Rotator *me, int16_t limit_deg);
static void Rotator_set_cw_limit(Rotator *me, int16_t limit_deg);
static void Rotator_calc_heading_coeffs(Rotator *me);
static int16_t Rotator_adc2deg(Rotator *me, int16_t adc);
static int16_t Rotator_deg2adc(Rotator *me, int16_t deg);


/******************************************************************************
 *
 * Public functions
 *
 *****************************************************************************/

void rotator_init(void) {
  eeprom_read_config();
  for (int i = 0; i < ROTATOR_COUNT; ++i) {
    Rotator_ctor(&rotator_sm[i], i);
  }
}


void rotator_set_default_config(void) {
  eeprom_set_default_config();
  eeprom_write_config();
  for (int i = 0; i < ROTATOR_COUNT; ++i) {
    Rotator *me = &rotator_sm[i];
    Rotator_calc_heading_coeffs(me);
    if (!is_calibrated(me)) {
      me->error = ROTATOR_ERROR_NO_CAL;
      post_event(i, ROTATOR_ERROR_SIG, 0);
    }
  }
}


int8_t rotator_cal_on(uint8_t rot_idx) {
  return post_event(rot_idx, CAL_ENABLE_SIG, 0);
}


int8_t rotator_cal_off(uint8_t rot_idx) {
  return post_event(rot_idx, CAL_DISABLE_SIG, 0);
}


int8_t rotator_set_ccw_limit(uint8_t rot_idx, int16_t limit_deg) {
  return post_event(rot_idx, SET_CCW_LIMIT_SIG, limit_deg);
}


int8_t rotator_set_cw_limit(uint8_t rot_idx, int16_t limit_deg) {
  return post_event(rot_idx, SET_CW_LIMIT_SIG, limit_deg);
}


int8_t rotator_rotate_cw(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return -1;
  }
  Rotator *me = &rotator_sm[rot_idx];
  me->target_heading = INT16_MAX;
  return post_event(rot_idx, ROTATE_CW_SIG, 0);
}


int8_t rotator_rotate_ccw(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return -1;
  }
  Rotator *me = &rotator_sm[rot_idx];
  me->target_heading = INT16_MAX;
  return post_event(rot_idx, ROTATE_CCW_SIG, 0);
}


int8_t rotator_stop(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return -1;
  }
  Rotator *me = &rotator_sm[rot_idx];
  me->target_heading = INT16_MAX;
  return post_event(rot_idx, STOP_SIG, 0);
}


int16_t rotator_current_heading(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return 0;
  }
  Rotator *me = &rotator_sm[rot_idx];
  return Rotator_adc2deg(me, rotator_sm[rot_idx].current_heading);
}


int16_t rotator_current_heading_raw(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return 0;
  }
  return rotator_sm[rot_idx].current_heading;
}


int16_t rotator_target_heading(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return 0;
  }
  Rotator *me = &rotator_sm[rot_idx];
  return Rotator_adc2deg(me, rotator_sm[rot_idx].target_heading);
}


int8_t rotator_set_target_heading(uint8_t rot_idx, int16_t target_heading_deg) {
  if (rot_idx >= ROTATOR_COUNT) {
    return -1;
  }
  Rotator *me = &rotator_sm[rot_idx];
  RotatorConfig *conf = &cfg.rot[me->rot_idx];

    /* Get the current and target headings and adjust them so that they
     * are within the 0-359 degree range */
  target_heading_deg = range_adjust_heading(target_heading_deg);
  int16_t unadj_current_heading_deg = Rotator_adc2deg(me, me->current_heading);
  int16_t current_heading_deg = range_adjust_heading(unadj_current_heading_deg);

  if (target_heading_deg == current_heading_deg) {
    return 0;
  }

    /* Find out how far we have to travel in the CW and CCW directions
     * respectively to reach the target heading */
  int16_t ccw_diff_deg =
      range_adjust_heading(current_heading_deg - target_heading_deg);
  int16_t cw_diff_deg =
      range_adjust_heading(target_heading_deg - current_heading_deg);

    /* Find out if we will hit any rotation limits when going in a
      * certain direction */
  if (unadj_current_heading_deg - ccw_diff_deg < conf->ccw_limit_deg) {
    ccw_diff_deg = INT16_MAX;
  }
  if (unadj_current_heading_deg + cw_diff_deg > conf->cw_limit_deg) {
    cw_diff_deg = INT16_MAX;
  }

    /* Choose which is the best rotation direction */
  int16_t diff_deg;
  if (ccw_diff_deg < cw_diff_deg) {
    diff_deg = -ccw_diff_deg;
  }
  else {
    diff_deg = cw_diff_deg;
  }

    /* If we hit the rotation limits no matter in which direction we go,
     * there is no way to reach the target heading. */
  if (diff_deg == INT16_MAX) {
    return -1;
  }

    /* Set up the target heading and check if we should go CW or CCW.
     * Post an event to the state machine to get things going. */
  target_heading_deg = unadj_current_heading_deg + diff_deg;
  me->target_heading = Rotator_deg2adc(me, target_heading_deg);
  if (me->target_heading > me->current_heading) {
    return post_event(rot_idx, ROTATE_CW_SIG, 0);
  }
  else if (me->target_heading < me->current_heading) {
    return post_event(rot_idx, ROTATE_CCW_SIG, 0);
  }

    /* We did not need to move */
  me->target_heading = INT16_MAX;
  rotator_stop(rot_idx);
  
  return 0;
}


RotatorError rotator_current_error(uint8_t rot_idx) {
  if (rot_idx >= ROTATOR_COUNT) {
    return ROTATOR_ERROR_OK;
  }
  Rotator *me = &rotator_sm[rot_idx];
  return me->error;
}

const char *rotator_strerror(RotatorError error) {
  switch (error) {
    case ROTATOR_ERROR_OK:
      return "OK";
    case ROTATOR_ERROR_WRONG_DIR:
      return "WRONG DIRECTION";
    case ROTATOR_ERROR_STUCK:
      return "ROTATOR STUCK";
    case ROTATOR_ERROR_ASSERT:
      return "ASSERTION FAILED";
    case ROTATOR_ERROR_NO_CAL:
      return "NOT CALIBRATED";
  }
  return "?";
}



/******************************************************************************
 *
 * Externally declared functions
 *
 *****************************************************************************/

/**
 * @brief Called from the BSP code when a new direction value have been measured
 * @param rot_idx The rotator index
 * @param dir     The raw direction directly from the ADC
 *
 * This function is declared in bsp.h and called from the BSP code when a new
 * direction value have been measured for the specified rotator. The value is
 * the raw ADC value and so it must be filtered and converted to degrees.
 */
void bsp_heading_updated(uint8_t rot_idx, uint16_t adc) {
  Q_REQUIRE(rot_idx < ROTATOR_COUNT);
  
  Rotator *me = &rotator_sm[rot_idx];
  RotatorConfig *conf = &cfg.rot[me->rot_idx];

    /* Apply a median filter to remove transients */
  adc = median_filter(me, adc);

    /* Convert the raw ADC unsigned value to a signed integer */
  int16_t heading = (int16_t)(adc >> 1) - 0x4000;
  
    // Apply IIR filter
  heading = ((heading - me->prev_heading) >> 1) + me->prev_heading;
  
  int16_t speed = 10 * (heading - me->prev_heading);
  me->prev_heading = heading;
  me->current_heading = heading;

    /* Check if we have hit the CW or CCW limits */
  if ((me->rotate_dir > 0) && (me->current_heading >= conf->cw_limit)) {
    me->target_heading = INT16_MAX;
    (void)post_event(rot_idx, ROTATION_LIMIT_SIG, 0);
  }
  else if ((me->rotate_dir < 0) && (me->current_heading <= conf->ccw_limit)) {
    me->target_heading = INT16_MAX;
    (void)post_event(rot_idx, ROTATION_LIMIT_SIG, 0);
  }

    /* Check if the target heading has been reached, if set */
  if (me->target_heading != INT16_MAX) {
    if ((me->rotate_dir > 0) && (me->current_heading >= me->target_heading)) {
      me->target_heading = INT16_MAX;
      (void)post_event(rot_idx, STOP_SIG, 0);
    }
    else if ((me->rotate_dir < 0) && (me->current_heading <= me->target_heading)) {
      me->target_heading = INT16_MAX;
      (void)post_event(rot_idx, STOP_SIG, 0);
    }
  }

  if (bsp_rotator_is_running(me->rot_idx)) {
      /* Check if the rotator is stuck. We require at least one
       * degree per second */
    if (abs(speed) < me->heading_scale) {
      if (++me->stuck_cnt >= 10) {
        me->stuck_cnt = 0;
        me->error = ROTATOR_ERROR_STUCK;
        me->target_heading = INT16_MAX;
        (void)post_event(rot_idx, ROTATOR_ERROR_SIG, 0);
      }
    }
    else {
      if (me->stuck_cnt > 0) {
        me->stuck_cnt--;
      }
      
        /* Check if the rotator is turning in the correct direction */
      if (((me->rotate_dir ^ speed) & 0x8000) != 0) {
        if (++me->wrong_dir_cnt >= 10) {
          me->wrong_dir_cnt = 0;
          me->error = ROTATOR_ERROR_WRONG_DIR;
          me->target_heading = INT16_MAX;
          (void)post_event(rot_idx, ROTATOR_ERROR_SIG, 0);
        }
      }
      else if (me->wrong_dir_cnt > 0) {
        me->wrong_dir_cnt--;
      }
    }
  }
  else {
    me->wrong_dir_cnt = 0;
    me->stuck_cnt = 0;
  }

    /* Check if we should send a heading update */
  if (me->dir_update_counter == 0) {
    rotator_direction_updated(rot_idx,
                              Rotator_adc2deg(me, me->current_heading));
    me->dir_update_counter = 9;
  }
  else {
    me->dir_update_counter--;
  }
}



/******************************************************************************
 *
 * Private functions
 *
 *****************************************************************************/

/**
 * \brief   Send an event to the state machine associated with the given rotator
 * \param   rot_idx The rotator index
 * \param   sig     The event (signal) to send to the state machine
 * \param   par     The event parameter
 */
static int8_t post_event(uint8_t rot_idx, enum RotatorSignals sig, QParam par) {
  if (rot_idx >= ROTATOR_COUNT) {
    return -1;
  }
  QActive_post((QActive *)&rotator_sm[rot_idx], sig, par);
  return 0;
}


/**
 * \brief   Check if the CCW limit have been exceeded or not
 * \param   me The state machine object
 * \returns Return 1 if the limit has been exceeded or 0 if not
 */
static int8_t ccw_limit_exceeded(Rotator *me) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  return me->current_heading <= conf->ccw_limit;
}


/**
 * \brief   Check if the CW limit have been exceeded or not
 * \param   me The state machine object
 * \returns Return 1 if the limit has been exceeded or 0 if not
 */
static int8_t cw_limit_exceeded(Rotator *me) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  return me->current_heading >= conf->cw_limit;
}


/**
 * \brief   Range adjust the given heading so that it lies within 0-359 degrees
 * \param   me The state machine object
 * \returns Returns the range adjusted heading
 */
static int16_t range_adjust_heading(int16_t heading) {
  while (heading < 0) {
    heading += 360;
  }
  while (heading > 359) {
    heading -= 360;
  }
  return heading;
}


/**
 * \brief   Median filter the incoming signal
 * \param   me The state machine object
 * \param   adc The incoming ADC value to filter
 * \returns Returns a filtered ADC value
 *
 * The incoming signal will be filtered using a median filter. A median filter
 * will keep the last N samples in a circular buffer. For each sample, the
 * median in the buffer is calculated and returned. This will effectively
 * remove extreme values in the incoming data stream.
 *
 * In this implementation the filter length (N) is 3 so only glitches of one
 * sample in length can be suppressed.
 */
static uint16_t median_filter(Rotator *me, uint16_t adc) {
  me->median_buf[me->median_head] = adc;
  if (me->median_head >= 2) {
    me->median_head = 0;
  }
  else {
    me->median_head++;
  }

  uint16_t a = me->median_buf[0];
  uint16_t b = me->median_buf[1];
  uint16_t c = me->median_buf[2];
  
  if ((a <= b) && (a <= c))
  {
      /* a is the smallest value so the median must be either b or c */
    return (b <= c) ? b : c;
  }
  else if ((b <= a) && (b <= c))
  {
      /* b is the smallest value so the median must be either a or c */
    return (a <= c) ? a : c;
  }

    /* c is the smallest value so the median must be either a or b */
  return (a <= b) ? a : b;
}


/**
 * \brief   Check if this rotator has been calibrated or not
 * \param   me The state machine object
 * \returns Return 1 if calibrated or 0 if not
 */
static int8_t is_calibrated(Rotator *me) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  return (conf->ccw_limit != conf->cw_limit);
}



/******************************************************************************
 *
 * Private class methods
 *
 *****************************************************************************/

/**
 * \brief Constructor for the state machine
 * \param me	  The Rotator object to be initialized
 * \param rot_idx The rotator index
 */
static void Rotator_ctor(Rotator *me, uint8_t rot_idx) {
  me->current_heading = 0;
  me->target_heading = INT16_MAX;
  me->rotate_dir = 0;
  me->rot_idx = rot_idx;
  me->dir_update_counter = rot_idx * 2;
  me->error = ROTATOR_ERROR_OK;
  me->prev_heading = 0;
  me->wrong_dir_cnt = 0;
  Rotator_calc_heading_coeffs(me);
  me->stuck_cnt = 0;
  memset(me->median_buf, 0, sizeof(me->median_buf));
  me->median_head = 0;
  QActive_ctor((QActive *)me, (QStateHandler)&Rotator_initial);
}

/**
 * \brief   Set up the ccw_limt and ccw_limit_deg variables
 * \param   me The state machine object
 * \param   limit_deg The limit in degrees
 *
 * This function will set up the ccw_limit and ccw_limit_deg variables. The
 * value for ccw_limit will be taken from the current heading.
 * The CW limit variables will be adjusted if necessary.
 */
static void Rotator_set_ccw_limit(Rotator *me, int16_t limit_deg) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  conf->ccw_limit = me->current_heading;
  conf->ccw_limit_deg = limit_deg;
  if ((conf->cw_limit < conf->ccw_limit)
      || (conf->cw_limit_deg < conf->ccw_limit_deg)) {
    Rotator_set_cw_limit(me, limit_deg);
  }
}

/**
 * \brief   Set up the cw_limt and cw_limit_deg variables
 * \param   me The state machine object
 * \param   limit_deg The limit in degrees
 *
 * This function will set up the cw_limit and cw_limit_deg variables. The
 * value for cw_limit will be taken from the current heading.
 * The CCW limit variables will be adjusted if necessary.
 */
static void Rotator_set_cw_limit(Rotator *me, int16_t limit_deg) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  conf->cw_limit = me->current_heading;
  conf->cw_limit_deg = limit_deg;
  if ((conf->cw_limit < conf->ccw_limit)
      || (conf->cw_limit_deg < conf->ccw_limit_deg)) {
    Rotator_set_ccw_limit(me, limit_deg);
  }
}

/**
 * \brief   Set up the rotate_dir variable
 * \param   me The state machine object
 * \param   dir The direction to set
 *
 * Use this function to set the rotate_dir variable. The rotation limits will
 * be checked to see if it is possible to rotate in the specified direction.
 * If we cannot rotate, rotate_dir will be set to 0.
 */
static void Rotator_set_rotate_dir(Rotator *me, int8_t dir) {
  if (dir == me->rotate_dir) {
    return;
  }
  
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  if ((dir > 0) && (me->current_heading < conf->cw_limit)) {
    me->rotate_dir = dir;
  }
  else if ((dir < 0) && (me->current_heading > conf->ccw_limit)) {
    me->rotate_dir = dir;
  }
  else {
    me->rotate_dir = 0;
  }
}

/**
 * \brief   Calculate the coefficients used to translate between adc <-> degrees
 * \param   me The state machine object
 *
 * Call this function after changing any of the cw_limit or ccw_limit variables.
 * It will calclulate new scale and offset values to be used when translating
 * between raw (ADC) heading values and heading values in degrees.
 */
static void Rotator_calc_heading_coeffs(Rotator *me) {
  RotatorConfig *conf = &cfg.rot[me->rot_idx];
  if (conf->cw_limit_deg == conf->ccw_limit_deg) {
    me->heading_scale = 0;
    me->heading_offset = 0;
    return;
  }
  me->heading_scale = (conf->cw_limit - conf->ccw_limit)
                      / (conf->cw_limit_deg - conf->ccw_limit_deg);
  me->heading_offset = conf->cw_limit / me->heading_scale - conf->cw_limit_deg;
}

/**
 * \brief   Convert raw heading values into degrees
 * \param   me The state machine object
 * \param   adc The raw heading value
 * \returns Returns the heading value in degrees
 */
static int16_t Rotator_adc2deg(Rotator *me, int16_t adc) {
  return adc / me->heading_scale - me->heading_offset;
}

/**
 * \brief   Convert heading values given in degrees into raw heading values
 * \param   me The state machine object
 * \param   deg The heading value given in degrees
 * \returns Returns the raw heading value
 */
static int16_t Rotator_deg2adc(Rotator *me, int16_t deg) {
  return (deg + me->heading_offset) * me->heading_scale;
}


//! Include the state machine definition
#include "rotatorsm.c"

