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

#include "../ext_events.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

#define FLAG_NO_ROTATION          1
//! The rotator is allowed to be rotated
#define FLAG_ROTATION_ALLOWED     2
//! The rotator is being rotated CW
#define FLAG_ROTATION_CW          3
//! The rotator is being rotated CCW
#define FLAG_ROTATION_CCW         4

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

//! \brief These are extra keys which exist on some keypads (ESC, CTRL, Alt, <--)
//! External keyboard keycode for Button E1
#define KEYPAD_BTN_E1      0x76
//! External keyboard keycode for Button E2
#define KEYPAD_BTN_E2      0x14
//! External keyboard keycode for Button E3
#define KEYPAD_BTN_E3      0x11
//! External keyboard keycode for Button E4
#define KEYPAD_BTN_E4      0x66

#define MAX_ASCII_CMD_ARGS  5

void event_check_pings(void);
void event_internal_comm_parse_message(struct_comm_interface_msg message);
void __inline__ event_handler_process_ps2(unsigned char key_code);
void event_pulse_sensor_up(void);
void event_pulse_sensor_down(void);

void event_poll_buttons(void);
void event_poll_ext_device(void);
void event_bus_parse_message(BUS_MESSAGE bus_message);
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

void event_txrx_mode_pressed(void);

unsigned char event_check_critical_cmd_list(void);

#endif