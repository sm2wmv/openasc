/*! \file     general_io/sk3w_power_amp_ctrl/bsp.h
 *  \ingroup  general_io_group
 *  \brief    Board support package for the SK3W PA controller
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-05-23
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

/**
 * \brief   Get the state of the TX_ACTIVE pins
 * \returns Returns the state of the TX_ACTIVE pins
 * 
 * This function will, in the low six bits of the returned byte, indicate if
 * the TX_ACTIVE input is active or not for each controller (box).
 * The lowest bit is controller 0 (box A) and the highest is controller 5
 * (box F).
 */
uint8_t bsp_get_tx_active(void);

/**
 * \brief   Get the state of the ALARM pin for all bands
 * \returns Returns a 16 bit unsigned integer band bitmap
 * 
 * This function will return a 16 bit bitmap containing the alarm state for
 * each band. If a bit is set, the alarm is active. 160M=bit 0, 80M=bit 1 etc.
 */
uint16_t get_pa_alarm(void);

/**
 * \brief Set the PTT on or off
 * \param band  The band (BAND_160M, BAND_80M etc) to set the PTT on
 * \param on    Set to 1 to activate the PTT or or 0 to deactivate it
 */
void bsp_set_pa_ptt(unsigned char band, int on);

/**
 * \brief Set the mains on or off
 * \param band  The band (BAND_160M, BAND_80M etc) to set the mains on
 * \param on    Set to 1 to set the mains or or 0 to turn it off
 */
void bsp_set_pa_mains(unsigned char band, int on);

/**
 * \brief   Check if mains is on or off
 * \param   band  The band (BAND_160M, BAND_80M etc) to check the mains on
 * \returns Returns 1 if the mains is on or 0 if it's off
 */
uint8_t bsp_mains_is_on(unsigned char band);

/**
 * \brief Set or clear the PA reset pin
 * \param band  The band (BAND_160M, BAND_80M etc) to control
 * \param on    Set to 1 to reset the PA and 0 to clear the reset
 */
void bsp_set_pa_reset(unsigned char band, int on);

#endif
