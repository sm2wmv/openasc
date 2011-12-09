/*! \file comm_interface.h
 *  \brief Communication interface
 *  \defgroup comm_interface_group Communication interface group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface.h" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

#ifndef _COMM_INTERFACE_
#define _COMM_INTERFACE_

//! \brief Size of the TX queue
#define COMM_INTERFACE_TX_QUEUE_SIZE	5

//! \brief Size of the RX queue
#define COMM_INTERFACE_RX_QUEUE_SIZE	5

#define	COMM_INTERFACE_DATA_LENGTH	15

//! The number of times the interface should try to resend before giving up
#define COMM_INTERFACE_RESEND_COUNT	10

typedef struct {
	unsigned char checksum;
	unsigned char cmd;
	unsigned char length;
	unsigned char data[COMM_INTERFACE_DATA_LENGTH];
} struct_comm_interface_msg;

void comm_interface_init(void (*func_ptr_rx)(struct_comm_interface_msg), void (*func_ptr_tx)(char));
void comm_interface_poll_tx_queue(void);
void comm_interface_poll_rx_queue(void);
void comm_interface_1ms_tick(void);

unsigned char comm_interface_add_tx_message(unsigned char cmd, unsigned char length, unsigned char *data);

void enable_comm_interface_interrupt(void);
void disable_comm_interface_interrupt(void);

#endif