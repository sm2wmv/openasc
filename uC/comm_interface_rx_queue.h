/*! \file comm_interface_rx_queue.h
 *  \brief The comm interface RX QUEUE
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface_rx_queue.h" \endcode
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

#ifndef _COMM_INTERFACE_RX_QUEUE_H_
#define _COMM_INTERFACE_RX_QUEUE_H_

#include "comm_interface.h"

//! The structure of the RX circular buffer
typedef struct rx_linked_list {
	//! The list of messages
	struct_comm_interface_msg message[COMM_INTERFACE_RX_QUEUE_SIZE];
	//! The index of the first message in the list
	unsigned char first;
	//! The index of the last message in the list
	unsigned char last;
  //! The number of messages in the queue
  unsigned char count;
} comm_interface_rx_queue_struct;

unsigned char comm_interface_rx_queue_add(struct_comm_interface_msg message);
struct_comm_interface_msg comm_interface_rx_queue_get(void);
void comm_interface_rx_queue_drop(void);
void comm_interface_rx_queue_dropall(void);
void comm_interface_rx_queue_init(void);
unsigned char comm_interface_rx_queue_is_empty(void);
unsigned char comm_interface_rx_queue_count(void);

#endif
