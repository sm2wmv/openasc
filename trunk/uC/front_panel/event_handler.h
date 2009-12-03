/*! \file event_handler.h \brief Event handler of various things
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-04-19
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

#define KEYPAD_BTN_1 			0x69
#define KEYPAD_BTN_2 			0x72
#define KEYPAD_BTN_3 			0x7A
#define KEYPAD_BTN_4 			0x6B
#define KEYPAD_BTN_5 			0x73
#define KEYPAD_BTN_6 			0x74
#define KEYPAD_BTN_7 			0x6C
#define KEYPAD_BTN_8 			0x75
#define KEYPAD_BTN_9 			0x7D
#define KEYPAD_BTN_0			0x70
#define KEYPAD_BTN_A			0x77	
#define KEYPAD_BTN_B			0x4A
#define KEYPAD_BTN_C			0x7C
#define KEYPAD_BTN_D			0x7B
#define KEYPAD_BTN_E			0x79
#define KEYPAD_BTN_F			0x5A
#define KEYPAD_BTN_G			0x71

/* EXTERNAL KEYBOARD FUNCTIONS */
#define EXT_KEY_SEL_NONE						0
#define EXT_KEY_SEL_RX_ANT1					1
#define EXT_KEY_SEL_RX_ANT2					2
#define EXT_KEY_SEL_RX_ANT3					3
#define EXT_KEY_SEL_RX_ANT4					4
#define EXT_KEY_SEL_RX_ANT5					5
#define EXT_KEY_SEL_RX_ANT6					6
#define EXT_KEY_SEL_RX_ANT7					7
#define EXT_KEY_SEL_RX_ANT8					8

#define EXT_KEY_TOGGLE_TX_ANT1			9
#define EXT_KEY_TOGGLE_TX_ANT2			10
#define EXT_KEY_TOGGLE_TX_ANT3			11
#define EXT_KEY_TOGGLE_TX_ANT4			12

#define EXT_KEY_TOGGLE_RX_ANT1			13
#define EXT_KEY_TOGGLE_RX_ANT2			14
#define EXT_KEY_TOGGLE_RX_ANT3			15
#define EXT_KEY_TOGGLE_RX_ANT4			16

#define EXT_KEY_TOGGLE_RX_ANT_MODE	17
#define EXT_KEY_TOGGLE_TXRX_MODE		18

#define EXT_KEY_SET_ARRAY_DIR1			19
#define EXT_KEY_SET_ARRAY_DIR2			20
#define EXT_KEY_SET_ARRAY_DIR3			21
#define EXT_KEY_SET_ARRAY_DIR4			22
#define EXT_KEY_SET_ARRAY_DIR5			23
#define EXT_KEY_SET_ARRAY_DIR6			24
#define EXT_KEY_SET_ARRAY_DIR7			25
#define EXT_KEY_SET_ARRAY_DIR8			26

#define EXT_KEY_SET_STACK_COMB1			27
#define EXT_KEY_SET_STACK_COMB2			28
#define EXT_KEY_SET_STACK_COMB3			29
#define EXT_KEY_SET_STACK_COMB4			30
#define EXT_KEY_SET_STACK_COMB5			31
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
void event_parse_button_pressed(unsigned int btn_status);
void event_bus_parse_message(void);
void event_parse_ext_event(unsigned int ext_event_status);

void event_tx_button1_pressed(void);
void event_tx_button2_pressed(void);
void event_tx_button3_pressed(void);
void event_tx_button4_pressed(void);
void event_rotate_button_pressed(void);
void event_rxant_button_pressed(void);

void __inline__ event_set_rx_antenna(unsigned char ant_index);

#endif