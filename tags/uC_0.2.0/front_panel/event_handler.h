/*! \file front_panel/event_handler.h
 *  \brief Event handler of various things
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/event_handler.h" \endcode
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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

#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

//! External keyboard keycode for Button 1
#define KEYPAD_BTN_1 			0x69
//! External keyboard keycode for Button 2
#define KEYPAD_BTN_2 			0x72
//! External keyboard keycode for Button 3
#define KEYPAD_BTN_3 			0x7A
//! External keyboard keycode for Button 4
#define KEYPAD_BTN_4 			0x6B
//! External keyboard keycode for Button 5
#define KEYPAD_BTN_5 			0x73
//! External keyboard keycode for Button 6
#define KEYPAD_BTN_6 			0x74
//! External keyboard keycode for Button 7
#define KEYPAD_BTN_7 			0x6C
//! External keyboard keycode for Button 8
#define KEYPAD_BTN_8 			0x75
//! External keyboard keycode for Button 9
#define KEYPAD_BTN_9 			0x7D
//! External keyboard keycode for Button 0
#define KEYPAD_BTN_0			0x70
//! External keyboard keycode for Button A
#define KEYPAD_BTN_A			0x77	
//! External keyboard keycode for Button B
#define KEYPAD_BTN_B			0x4A
//! External keyboard keycode for Button C
#define KEYPAD_BTN_C			0x7C
//! External keyboard keycode for Button D
#define KEYPAD_BTN_D			0x7B
//! External keyboard keycode for Button E
#define KEYPAD_BTN_E			0x79
//! External keyboard keycode for Button F
#define KEYPAD_BTN_F			0x5A
//! External keyboard keycode for Button G
#define KEYPAD_BTN_G			0x71

/* EXTERNAL KEYBOARD FUNCTIONS */
//! Ext keyboard - No function assigned
#define EXT_KEY_SEL_NONE						0
//! Ext keyboard - Set RX antenna #1
#define EXT_KEY_SEL_RX_ANT1					1
//! Ext keyboard - Set RX antenna #2
#define EXT_KEY_SEL_RX_ANT2					2
//! Ext keyboard - Set RX antenna #3
#define EXT_KEY_SEL_RX_ANT3					3
//! Ext keyboard - Set RX antenna #4
#define EXT_KEY_SEL_RX_ANT4					4
//! Ext keyboard - Set RX antenna #5
#define EXT_KEY_SEL_RX_ANT5					5
//! Ext keyboard - Set RX antenna #6
#define EXT_KEY_SEL_RX_ANT6					6
//! Ext keyboard - Set RX antenna #7
#define EXT_KEY_SEL_RX_ANT7					7
//! Ext keyboard - Set RX antenna #8
#define EXT_KEY_SEL_RX_ANT8					8
//! Ext keyboard - Set RX antenna #9
#define EXT_KEY_SEL_RX_ANT9					9
//! Ext keyboard - Set RX antenna #10
#define EXT_KEY_SEL_RX_ANT10				10

//! Ext keyboard - Toggle TX antenna combination #1
#define EXT_KEY_TOGGLE_TX_ANT1			9
//! Ext keyboard - Toggle TX antenna combination #2
#define EXT_KEY_TOGGLE_TX_ANT2			10
//! Ext keyboard - Toggle TX antenna combination #3
#define EXT_KEY_TOGGLE_TX_ANT3			11
//! Ext keyboard - Toggle TX antenna combination #4
#define EXT_KEY_TOGGLE_TX_ANT4			12

//! Ext keyboard - Toggle RX antenna combination #1
#define EXT_KEY_TOGGLE_RX_ANT1			13
//! Ext keyboard - Toggle RX antenna combination #2
#define EXT_KEY_TOGGLE_RX_ANT2			14
//! Ext keyboard - Toggle RX antenna combination #3
#define EXT_KEY_TOGGLE_RX_ANT3			15
//! Ext keyboard - Toggle RX antenna combination #4
#define EXT_KEY_TOGGLE_RX_ANT4			16

//! Ext keyboard - Toggle RX antenna enabled
#define EXT_KEY_TOGGLE_RX_ANT_MODE	17
//! Ext keyboard - Toggle TX/RX mode on/off
#define EXT_KEY_TOGGLE_TXRX_MODE		18

//! Ext keyboard - Select array direction #1
#define EXT_KEY_SET_ARRAY_DIR1			19
//! Ext keyboard - Select array direction #2
#define EXT_KEY_SET_ARRAY_DIR2			20
//! Ext keyboard - Select array direction #3
#define EXT_KEY_SET_ARRAY_DIR3			21
//! Ext keyboard - Select array direction #4
#define EXT_KEY_SET_ARRAY_DIR4			22
//! Ext keyboard - Select array direction #5
#define EXT_KEY_SET_ARRAY_DIR5			23
//! Ext keyboard - Select array direction #6
#define EXT_KEY_SET_ARRAY_DIR6			24
//! Ext keyboard - Select array direction #7
#define EXT_KEY_SET_ARRAY_DIR7			25
//! Ext keyboard - Select array direction #8
#define EXT_KEY_SET_ARRAY_DIR8			26

//! Ext keyboard - Select stack combo #1
#define EXT_KEY_SET_STACK_COMB1			27
//! Ext keyboard - Select stack combo #2
#define EXT_KEY_SET_STACK_COMB2			28
//! Ext keyboard - Select stack combo #3
#define EXT_KEY_SET_STACK_COMB3			29
//! Ext keyboard - Select stack combo #4
#define EXT_KEY_SET_STACK_COMB4			30
//! Ext keyboard - Select stack combo #5
#define EXT_KEY_SET_STACK_COMB5			31
//! Ext keyboard - Select stack combo #6
#define EXT_KEY_SET_STACK_COMB6			32

void event_set_error(unsigned char error_type, unsigned char state);
unsigned char event_get_errors(void);
unsigned char event_get_error_state(unsigned char error_type);

void event_internal_comm_parse_message(UC_MESSAGE message);
void event_handler_process_ps2(unsigned char key_code);
void event_pulse_sensor_up(void);
void event_pulse_sensor_down(void);
void event_update_display(void);
void event_poll_buttons(void);
void event_poll_ext_device(void);
void event_bus_parse_message(void);
void event_parse_ext_event(unsigned int ext_event_status);

void event_tx_button1_pressed(void);
void event_tx_button2_pressed(void);
void event_tx_button3_pressed(void);
void event_tx_button4_pressed(void);
void event_rotate_button_pressed(void);
void event_rxant_button_pressed(void);
void event_aux2_button_pressed(void);

void __inline__ event_set_rx_antenna(unsigned char ant_index);

#endif