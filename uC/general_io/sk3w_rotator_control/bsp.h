/*! \file     general_io/sk3w_rotator_control/bsp.h
 *  \ingroup  general_io_group
 *  \brief    Board support package for the SK3W rotator controller
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-06-16
 * 
 * The board support package is used to isolate hardware access from other
 * parts of the application. This will make it easier to port software to
 * other hardware if necessary.
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

#ifndef _BSP_H_
#define _BSP_H_


/**
 * \brief Initialize the board support package
 */
void bsp_init(void);

/**
 * \brief Initialize timer 2 that is used for the bus communications
 */
void bsp_init_timer_2(void);

int8_t bsp_rotator_release_break(uint8_t rot_idx);
int8_t bsp_rotator_apply_break(uint8_t rot_idx);
void bsp_rotator_run_ccw(uint8_t rot_idx);
void bsp_rotator_run_cw(uint8_t rot_idx);
int8_t bsp_rotator_is_running(uint8_t rot_idx);
void bsp_rotator_stop(uint8_t rot_idx);

extern void bsp_direction_updated(uint8_t rot_idx, uint16_t dir);


#endif
