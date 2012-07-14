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


enum {
  ERROR_NONE,
  ERROR_WRONG_DIR,
  ERROR_ROTATOR_STUCK,
  ERROR_COUNT
};


/**
 * \brief Initialize the PA state machine
 */
void rotator_init(void);

/**
 * \brief Set all configuration variables to their default values
 */
void rotator_set_default_config(void);


int8_t rotator_cal_on(uint8_t rot_idx);
int8_t rotator_cal_off(uint8_t rot_idx);
int8_t rotator_rotate_cw(uint8_t rot_idx);
int8_t rotator_rotate_ccw(uint8_t rot_idx);
int8_t rotator_stop(uint8_t rot_idx);
int16_t rotator_current_heading(uint8_t rot_idx);
int16_t rotator_target_heading(uint8_t rot_idx);
int8_t rotator_set_angle(uint8_t rot_idx, int16_t target_heading_deg);
int8_t rotator_set_ccw_limit(uint8_t rot_idx, int16_t limit_deg);
int8_t rotator_set_cw_limit(uint8_t rot_idx, int16_t limit_deg);
int8_t rotator_direction(uint8_t rot_idx, int16_t *dir, int16_t *dir_deg);

extern void rotator_direction_updated(uint8_t rot_idx, int16_t dir);

#endif /* _ROTATOR_H_ */
