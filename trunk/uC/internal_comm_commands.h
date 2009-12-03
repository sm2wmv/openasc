/*! \file internal_comm_commands.h \brief Internal communication commands
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-12-18
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

#ifndef _INTERNAL_COMM_COMMANDS_H_
#define _INTERNAL_COMM_COMMANDS_H_

/* For turning on/off the driver outputs the same commands is sent as to the driver modules */

//! Initialize the shut down sequence
#define INT_COMM_TURN_DEVICE_OFF						0x01
//! Shut down the device
#define INT_COMM_PULL_THE_PLUG							0x02
//! Driver output activate command
#define INT_COMM_ACTIVATE_DRIVER_OUTPUT			0x10
//! Driver output deactivate command
#define INT_COMM_DEACTIVATE_DRIVER_OUTPUT		0x11
//! Change the state of one of the AUX pins on the X11 connector
#define INT_COMM_AUX_CHANGE_OUTPUT_PIN			0x20
//! Read the input status of an AUX pin on the X11 connector
#define INT_COMM_AUX_READ_INPUT_PIN					0x21
//! Read the BCD input on the top floor
#define INT_COMM_GET_BAND_BCD_STATUS				0x30
//! A key was pressed on the external PS2 keypad
#define INT_COMM_PS2_KEYPRESSED							0x40
//! Command used to transfer commands from the PC to the front panel and vice versa
#define INT_COMM_PC_CTRL										0x50

#endif