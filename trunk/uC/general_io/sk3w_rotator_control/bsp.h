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
 *
 * Call this function once at the start of the application to initialize the
 * board support package. Since it initializes low level stuff it should be
 * called early in the statup code.
 */
void bsp_init(void);

/**
 * \brief Initialize timer 2 that is used for the bus communications
 *
 * This function is called from the bus communications code to set up the
 * communications timer.
 */
void bsp_init_timer_2(void);

/**
 * \brief   Release the break
 * \param   rot_idx The rotator index
 *
 * Call this function to release the break for the specified rotator.
 */
void bsp_rotator_release_break(uint8_t rot_idx);

/**
 * \brief   Apply the break
 * \param   rot_idx The rotator index
 *
 * Call this function to apply the break for the specified rotator
 */
void bsp_rotator_apply_break(uint8_t rot_idx);

/**
 * \brief   Start running counter clockwise
 * \param   rot_idx The rotator index
 *
 * Call this function to start turning the specified rotator counter clockwise.
 * If it is running clockwise, that control output will be set to off first.
 *
 * Note that the break have to be released by calling bsp_rotator_release_break
 * prior to starting rotation. This is NOT done automatically.
 */
void bsp_rotator_run_ccw(uint8_t rot_idx);

/**
 * \brief   Start running clockwise
 * \param   rot_idx The rotator index
 *
 * Call this function to start turning the specified rotator clockwise.
 * If it is running counter clockwise, that control output will be set
 * to off first.
 *
 * Note that the break have to be released by calling bsp_rotator_release_break
 * prior to starting rotation. This is NOT done automatically.
 */
void bsp_rotator_run_cw(uint8_t rot_idx);

/**
 * \brief   Read the hardware registers to see if the rotator is running or not
 * \param   rot_idx The rotator index
 * \returns Returns 1 if the rotator is running or 0 if it's not
 *
 * This function will read the output port register directly to see if the
 * rotator is running or not. Reading the output port register just returns
 * what was previously written to it so it should reflect the state of the port.
 */
int8_t bsp_rotator_is_running(uint8_t rot_idx);

/**
 * \brief   Stop rotation
 * \param   rot_idx The rotator index
 *
 * Immediately stop the rotation for the specified rotator.
 */
void bsp_rotator_stop(uint8_t rot_idx);


/**
 * \brief   Called by the BSP code when a new heading has been measured
 * \param   rot_idx The rotator index
 * \param   adc The raw ADC value expressed as a two byte unsigned
 *
 * This function is declared here and should be implemented in external code.
 * It will then serve as a callback for when a new heading is available.
 * The ADC value is a 10 bit value that is left adjusted in a 16 bit unsigned
 * variable,
 */
extern void bsp_heading_updated(uint8_t rot_idx, uint16_t adc);


#endif
