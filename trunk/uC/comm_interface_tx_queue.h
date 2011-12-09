/*! \file comm_interface_tx_queue.h
 *  \brief The comm interface TX QUEUE
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface_tx_queue.h" \endcode
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

#ifndef _COMM_INTERFACE_TX_QUEUE_H_
#define _COMM_INTERFACE_TX_QUEUE_H_

#include "comm_interface.h"

//! The structure of the TX circular buffer
typedef struct tx_linked_list {
	//! A UC_MESSAGE
	struct_comm_interface_msg message[COMM_INTERFACE_TX_QUEUE_SIZE];
	//! first position in the list
	unsigned char first;
	//! last position in the list
	unsigned char last;
} comm_interface_tx_queue_struct;

unsigned char comm_interface_tx_queue_add(struct_comm_interface_msg message);
struct_comm_interface_msg comm_interface_tx_queue_get_pos(unsigned char pos);
struct_comm_interface_msg comm_interface_tx_queue_get(void);
void comm_interface_tx_queue_drop(void);
void comm_interface_tx_queue_dropall(void);
void comm_interface_tx_queue_init(void);
unsigned char comm_interface_tx_queue_is_empty(void);
unsigned char comm_interface_tx_queue_size(void);

#endif
