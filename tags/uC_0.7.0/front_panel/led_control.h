/*! \file front_panel/led_control.h 
 *  \brief Front panel LED control functions
 *  \ingroup front_panel_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/led_control.h" \endcode
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

//! Bits which indicate the status of each individual LED
#define LED_STATUS_ERROR      0
#define LED_STATUS_TX_ANT1    1
#define LED_STATUS_TX_ANT2    2
#define LED_STATUS_TX_ANT3    3
#define LED_STATUS_TX_ANT4    4
#define LED_STATUS_RX_ANT1    5
#define LED_STATUS_RX_ANT2    6
#define LED_STATUS_RX_ANT3    7
#define LED_STATUS_RX_ANT4    8
#define LED_STATUS_AUX        9
#define LED_STATUS_ROTATING   10
#define LED_STATUS_ROTATE     11
#define LED_STATUS_TXRX       12
#define LED_STATUS_RXANT      13
#define LED_STATUS_SUB        14
#define LED_STATUS_MENU       15

#define LED_PTT_STATUS_OK       0
#define LED_PTT_STATUS_INHIBIT  1
#define LED_PTT_STATUS_ACTIVE   2

//! PTT led state
enum enum_led_ptt_state {
	//! Used to set the LED to PTT active color
	LED_STATE_PTT_ACTIVE,
	//! Used to set the LED to INHIBIT color
 	LED_STATE_PTT_INHIBIT,
	//! Used to set the LED to PTT OK color (which means that it is OK to transmit)
 	LED_STATE_PTT_OK
};

//! Regular LED state
enum enum_led_state {
	//! Used to set the LED as ON
	LED_STATE_ON,
	//! Used to set the LED as OFF
	LED_STATE_OFF
};

unsigned char led_status_changed(void);
unsigned int led_get_status(void);
unsigned char led_get_ptt_status(void);
void led_status_clear(void);

void led_set_band(unsigned char band);
void led_set_band_none(void);

void led_set_ptt(enum enum_led_ptt_state state);
void led_set_error(enum enum_led_state state);
void led_set_rotation_active(enum enum_led_state state);

void led_set_tx_ant(unsigned char index, enum enum_led_state state);
void led_set_rx_ant(unsigned char index, enum enum_led_state state);

void led_set_rotate(enum enum_led_state state);
void led_set_txrx(enum enum_led_state state);
void led_set_rxant(enum enum_led_state state);
void led_set_aux(enum enum_led_state state);
void led_set_menu(enum enum_led_state state);
void led_set_submenu(enum enum_led_state state);

void led_set_all(enum enum_led_state state);
#endif