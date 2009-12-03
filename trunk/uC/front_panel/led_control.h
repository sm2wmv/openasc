/*! \file led_control.h \brief Front panel LED control functions
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-25
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
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


#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

enum enum_led_state {
	//! Used to set the LED as ON
	LED_STATE_ON,
	//! Used to set the LED as OFF
	LED_STATE_OFF,
	//! Used to set the LED to GREEN
	LED_STATE_GREEN,
	//! Used to set the LED to RED
	LED_STATE_RED
};

void led_set_band(unsigned char band);
void led_set_band_none(void);

void led_set_ptt(enum enum_led_state state);
void led_set_error(enum enum_led_state state);
void led_set_rotation_active(enum enum_led_state state);

void led_set_tx_ant(unsigned char index, enum enum_led_state state);
void led_set_rx_ant(unsigned char index, enum enum_led_state state);

void led_set_rotate(enum enum_led_state state);
void led_set_txrx(enum enum_led_state state);
void led_set_rxant(enum enum_led_state state);
void led_set_aux(enum enum_led_state state);
void led_set_menu(enum enum_led_state state);

void led_set_ptt_on(void);
void led_set_ptt_off(void);

void led_set_all(enum enum_led_state state);
#endif