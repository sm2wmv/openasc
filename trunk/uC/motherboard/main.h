/*! \file motherboard/main.h 
 *  \defgroup  motherboard_group Motherboard
 *  \brief Main file of the motherboard
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "motherboard/main.h" \endcode
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

#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//#define DEBUG_REMOTE_CTRL

//! Internal communication command to redirect data
#define INT_COMM_REDIRECT_DATA	0x10

//TODO: We might want to change the PS2_STRUCT so that it uses less space, grouping together flags

//! Struct of the PS/2 interface status
typedef struct {
	//! To see if we have started to read a keyboard command
	unsigned char started;
	//! Number of bytes we have receieved
	unsigned char bit_count;
	//! The actual data received
	unsigned char data;
	//! Flag to indicate that data is ready
	unsigned char data_ready;
	//! Flag to indicate that we are transmitting
	unsigned char transmit;
	//! The parity byte
	unsigned char parity;
	//! Transmit data
	unsigned char tx_data;
	//! Previous command
	unsigned char prev_cmd;
} PS2_STRUCT;

/*!Structure of the driver output status. It contains information of which address
 * an output was activated/deactivated from last and also it's status */
typedef struct {
	//! The address of the device that last changed the status of the output */
	unsigned char driver_output_owner[20];	//The address of the last device to change the status
	//! The type of message that activated the output
	unsigned char driver_output_type[20];	
	//! The address of the device that last changed the status of the output */
	unsigned char driver_output_new_owner[20];	//The address of the last device to change the status
	//! The type of message that activated the output
	unsigned char driver_output_new_type[20];
	//! The state of the driver output if it's high or low
	unsigned long driver_output_state;
} driver_status_struct;

#endif
