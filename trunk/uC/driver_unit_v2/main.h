/*! \file driver_unit_v2/main.h 
 *  \defgroup driver_unit_v2_group Driver unit version 2
 *  \brief Main file of the driver unit. 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2009-03-16
 *  \code #include "driver_unit_v2/main.h" \endcode
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


#ifndef _MAIN_H_
#define _MAIN_H_

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	10

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
	//! Flags
	unsigned char flags;
} driver_status_struct;

 /*! Flag to indicate if the TX/RX mode is enabled */
 #define FLAG_TXRX_MODE_ENABLED	0

//! Driver status for output OFF
#define DRIVER_STATUS_OFF	0
//! Driver status for output ON
#define DRIVER_STATUS_ON	1

#endif
