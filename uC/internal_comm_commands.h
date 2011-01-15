/*! \file internal_comm_commands.h
 *  \brief The internal communication commands
 *  \ingroup internal_comm_group Internal
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm_commands.c" \endcode
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

//All the internal communication commands should be between 0xC0 - 0xEF except BUS_CMDs which are sent internally

//! Initialize the shut down sequence
#define INT_COMM_TURN_DEVICE_OFF						0xC0
//! Shut down the device
#define INT_COMM_PULL_THE_PLUG							0xC1

//! Change the state of one of the AUX pins on the X11 connector
#define INT_COMM_AUX_CHANGE_OUTPUT_PIN			0xC2
//! Read the input status of an AUX pin on the X11 connector
#define INT_COMM_AUX_READ_INPUT_PIN					0xC3
//! Read the BCD input on the top floor
#define INT_COMM_GET_BAND_BCD_STATUS				0xC4
//! A key was pressed on the external PS2 keypad
#define INT_COMM_PS2_KEYPRESSED							0xC5
//! Command used to transfer commands from the PC to the front panel and vice versa
#define INT_COMM_PC_CTRL										0xC6
//! Command to tell the computer that the last package was ACKED ok
#define INT_COMM_PC_CTRL_ACK								0xC7
//! Command to tell the computer that the last package was NACKED
#define INT_COMM_PC_CTRL_NACK								0xC8
//! Command which tells the front panel to send and receive ASCII data to a certain device on the bus
#define INT_COMM_PC_CONNECT_TO_ADDR         0xD0
//! Command to send data to a device
#define INT_COMM_PC_SEND_TO_ADDR            0xD1

#endif