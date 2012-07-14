/*! \file     general_io/sk3w_rotator_control/rotator.h
 *  \ingroup  general_io_group
 *  \brief    Rotator state machine base
 *  \author   Tobias Blomberg, SN0SVX
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

#ifndef _ROTATOR_H_
#define _ROTATOR_H_


/**
 * Error codes which the rotator state machine may emit
 */
enum {
  ERROR_NONE,           /*!< No error */
  ERROR_WRONG_DIR,      /*!< The rotator is turning in the wrong direction */ 
  ERROR_ROTATOR_STUCK   /*!< The rotator is stuck */
};


/**
 * \brief Initialize the Rotator state machine
 *
 * Call this function once to initialize the rotator state machine.
 */
void rotator_init(void);

/**
 * \brief Set all configuration variables to their default values
 *
 * All configuration variables will be set to their default values and written
 * to the EEPROM.
 */
void rotator_set_default_config(void);

/**
 * \brief   Enter calibration mode for the specified rotator
 * \param   rot_idx The rotator index
 * \returns Return 0 on success or -1 on failure
 *
 * Call this function to enable calibration mode for the specified rotator.
 * Note that calibration mode can only be entered when the rotator is idle.
 * Trying to enter calibration mode when not idle will just be ignored.
 *
 * Calibrating the rotator is very easy. First enter calibration mode by calling
 * this function. Start by turning the rotator to the heading where the
 * CCW limit should be and set the limit using the rotator_set_ccw_limit
 * function. Now rotate to where the CW limit should be and call the
 * rotator_set_cw_limit function.
 * Exit calibration mode by calling rotator_cal_off to activate and save the
 * new values.
 */
int8_t rotator_cal_on(uint8_t rot_idx);

/**
 * \brief   Exit calibration mode for the specified rotator
 * \param   rot_idx The rotator index
 * \returns Return 0 on success or -1 on failure
 *
 * Exit calibration mode. When exiting, new values for heading scale and offset
 * will be calculated and written to the EEPROM.
 */
int8_t rotator_cal_off(uint8_t rot_idx);

/**
 * \brief   Set the counter clockwise rotation limit
 * \param   rot_idx The rotator index
 * \param   limit_deg The rotation limit in degrees
 * \returns Return 0 on success or -1 on failure
 *
 * This function can only be used while in calibration mode. Setting of a
 * rotation limit while not in calibration mode will be ignored.
 * The given limit in degrees will be associated with the current raw heading.
 * This information is used both as a rotation limit and also together with
 * the CW limit value to calculate scale and offset values used to translate
 * between headings in degrees and raw (ADC) headings.
 */
int8_t rotator_set_ccw_limit(uint8_t rot_idx, int16_t limit_deg);

/**
 * \brief   Set the clockwise rotation limit
 * \param   rot_idx The rotator index
 * \param   limit_deg The rotation limit in degrees
 * \returns Return 0 on success or -1 on failure
 *
 * This function can only be used while in calibration mode. Setting of a
 * rotation limit while not in calibration mode will be ignored.
 * The given limit in degrees will be associated with the current raw heading.
 * This information is used both as a rotation limit and also together with
 * the CCW limit value to calculate scale and offset values used to translate
 * between headings in degrees and raw (ADC) headings.
 */
int8_t rotator_set_cw_limit(uint8_t rot_idx, int16_t limit_deg);

/**
 * \brief   Start clockwise rotation
 * \param   rot_idx The rotator index
 * \returns Return 0 on success or -1 on failure
 *
 * Start rotation clockwise. The rotation will only start if it is appropriate
 * to do so. The start may be delayed for safety reasons.
 */
int8_t rotator_rotate_cw(uint8_t rot_idx);

/**
 * \brief   Start counter clockwise rotation
 * \param   rot_idx The rotator index
 * \returns Return 0 on success or -1 on failure
 *
 * Start rotation counter clockwise. The rotation will only start if it is
 * appropriate to do so. The start may be delayed for safety reasons.
 */
int8_t rotator_rotate_ccw(uint8_t rot_idx);

/**
 * \brief   Stop rotation
 * \param   rot_idx The rotator index
 * \returns Return 0 on success or -1 on failure
 *
 * The rotation will stop immediately if the rotator is turning.
 */
int8_t rotator_stop(uint8_t rot_idx);

/**
 * \brief   Return the current rotator heading in degrees
 * \param   rot_idx The rotator index
 * \returns Returns the current heading in degrees
 */
int16_t rotator_current_heading(uint8_t rot_idx);

/**
 * \brief   Return the current rotator raw heading
 * \param   rot_idx The rotator index
 * \returns Returns the current raw heading
 *
 * This function will return the raw current heading. The raw heading is
 * essentially the value directly from the ADC. However, it's converted to
 * signed integer and has gone through some filtering.
 */
int16_t rotator_current_heading_raw(uint8_t rot_idx);

/**
 * \brief   Return the target rotator heading in degrees
 * \param   rot_idx The rotator index
 * \returns Returns the target heading in degrees
 */
int16_t rotator_target_heading(uint8_t rot_idx);

/**
 * \brief   Set the target heading
 * \param   rot_idx The rotator index
 * \param   target_heading_deg The target heading in degrees
 * \returns Return 0 on success or -1 on failure
 *
 * Calling this function will set a target heading. Rotation will start in
 * the direction where the travel will be the shortest. If the target heading
 * cannot be reached, nothing will happen.
 */
int8_t rotator_set_target_heading(uint8_t rot_idx, int16_t target_heading_deg);


/**
 * \brief   Called by the rotator code when the heading display is to be updated
 * \param   rot_idx The rotator index
 * \param   dir The new direction in degrees
 *
 * This function must be implemented by code external to the rotator. It will
 * then act as a callback to the other code. The rotator code will call this
 * function when the heading displpay should be updated.
 */
extern void rotator_direction_updated(uint8_t rot_idx, int16_t dir);


#endif /* _ROTATOR_H_ */
