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

/* EXTERNAL CTRL FUNCTIONS */
//! Ext ctrl - No function assigned
#define EXT_CTRL_SEL_NONE						0
//! Ext ctrl - Set RX antenna #1
#define EXT_CTRL_SEL_RX_ANT1					1
//! Ext ctrl - Set RX antenna #2
#define EXT_CTRL_SEL_RX_ANT2					2
//! Ext ctrl - Set RX antenna #3
#define EXT_CTRL_SEL_RX_ANT3					3
//! Ext ctrl - Set RX antenna #4
#define EXT_CTRL_SEL_RX_ANT4					4
//! Ext ctrl - Set RX antenna #5
#define EXT_CTRL_SEL_RX_ANT5					5
//! Ext ctrl - Set RX antenna #6
#define EXT_CTRL_SEL_RX_ANT6					6
//! Ext ctrl - Set RX antenna #7
#define EXT_CTRL_SEL_RX_ANT7					7
//! Ext ctrl - Set RX antenna #8
#define EXT_CTRL_SEL_RX_ANT8					8
//! Ext ctrl - Set RX antenna #9
#define EXT_CTRL_SEL_RX_ANT9					9
//! Ext ctrl - Set RX antenna #10
#define EXT_CTRL_SEL_RX_ANT10				10

//! Ext ctrl - Toggle TX antenna combination #1
#define EXT_CTRL_TOGGLE_TX_ANT1			11
//! Ext ctrl - Toggle TX antenna combination #2
#define EXT_CTRL_TOGGLE_TX_ANT2			12
//! Ext ctrl - Toggle TX antenna combination #3
#define EXT_CTRL_TOGGLE_TX_ANT3			13
//! Ext ctrl - Toggle TX antenna combination #4
#define EXT_CTRL_TOGGLE_TX_ANT4			14

//! Ext ctrl - Toggle RX antenna combination #1
#define EXT_CTRL_TOGGLE_RX_ANT1			15
//! Ext ctrl - Toggle RX antenna combination #2
#define EXT_CTRL_TOGGLE_RX_ANT2			16
//! Ext ctrl - Toggle RX antenna combination #3
#define EXT_CTRL_TOGGLE_RX_ANT3			17
//! Ext ctrl - Toggle RX antenna combination #4
#define EXT_CTRL_TOGGLE_RX_ANT4			18

//! Ext ctrl - Toggle RX antenna enabled
#define EXT_CTRL_TOGGLE_RX_ANT_MODE	19
//! Ext ctrl - Toggle TX/RX mode on/off
#define EXT_CTRL_TOGGLE_TXRX_MODE		20

//! Ext ctrl - Select array direction #1
#define EXT_CTRL_SET_ARRAY_DIR1			21
//! Ext ctrl - Select array direction #2
#define EXT_CTRL_SET_ARRAY_DIR2			22
//! Ext ctrl - Select array direction #3
#define EXT_CTRL_SET_ARRAY_DIR3			23
//! Ext ctrl - Select array direction #4
#define EXT_CTRL_SET_ARRAY_DIR4			24
//! Ext ctrl - Select array direction #5
#define EXT_CTRL_SET_ARRAY_DIR5			25
//! Ext ctrl - Select array direction #6
#define EXT_CTRL_SET_ARRAY_DIR6			26
//! Ext ctrl - Select array direction #7
#define EXT_CTRL_SET_ARRAY_DIR7			27
//! Ext ctrl - Select array direction #8
#define EXT_CTRL_SET_ARRAY_DIR8			28

//! Ext ctrl - Select stack combo #1
#define EXT_CTRL_SET_STACK_COMB1			29
//! Ext ctrl - Select stack combo #2
#define EXT_CTRL_SET_STACK_COMB2			30
//! Ext ctrl - Select stack combo #3
#define EXT_CTRL_SET_STACK_COMB3			31
//! Ext ctrl - Select stack combo #4
#define EXT_CTRL_SET_STACK_COMB4			32
//! Ext ctrl - Select stack combo #5
#define EXT_CTRL_SET_STACK_COMB5			33
//! Ext ctrl - Select stack combo #6
#define EXT_CTRL_SET_STACK_COMB6			34

//! Ext ctrl - Toggle the amplifier on/off
#define EXT_CTRL_AMPLIFIER_TOGGLE_ON_OFF	35
//! Ext ctrl - Toggle the amplifier standby
#define EXT_CTRL_AMPLIFIER_TOGGLE_STANDBY	36
//! Ext ctrl - Tune the amplifier to the correct band
#define EXT_CTRL_AMPLIFIER_TUNE	37
//! Ext ctrl - Reset the amplifier
#define EXT_CTRL_AMPLIFIER_RESET	38

//! Ext ctrl - Rotate the last antenna clock wise
#define EXT_CTRL_ROTATE_CW	39
//! Ext ctrl - Rotate the last antenna counter clock wise
#define EXT_CTRL_ROTATE_CCW 40

//! Ext ctrl - Select band portion
#define EXT_CTRL_SEL_BAND_PORTION 41
//! Ext ctrl - Change band up
#define EXT_CTRL_CHANGE_BAND_UP		42
//! Ext ctrl - Change band down
#define EXT_CTRL_CHANGE_BAND_DOWN 43


#define MAX_ASCII_CMD_ARGS  5

void event_check_pings(void);

void event_internal_comm_parse_message(UC_MESSAGE message);
void event_handler_process_ps2(unsigned char key_code);
void event_pulse_sensor_up(void);
void event_pulse_sensor_down(void);

void event_update_display(void);

void event_poll_buttons(void);
void event_poll_ext_device(void);
void event_bus_parse_message(void);
void event_parse_ext_event(unsigned int ext_event_status);

void event_sub_button_pressed(void);

void event_tx_button1_pressed(void);
void event_tx_button2_pressed(void);
void event_tx_button3_pressed(void);
void event_tx_button4_pressed(void);
void event_rotate_button_pressed(void);
void event_rxant_button_pressed(void);

void event_aux1_button_pressed(void);
void event_aux2_button_pressed(void);

void event_aux1_button_released(void);
void event_aux2_button_released(void);

void event_process_task(unsigned char task_index);

void __inline__ event_set_rx_antenna(unsigned char ant_index);

void event_handler_check_uc_cmd(void);

#endif