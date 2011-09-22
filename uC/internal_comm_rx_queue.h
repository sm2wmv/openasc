/*! \file internal_comm_rx_queue.h
 *  \brief The internal communication RX QUEUE
 *  \ingroup internal_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm_rx_queue.h" \endcode
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

#ifndef _INTERNAL_COMM_RX_QUEUE_H_
#define _INTERNAL_COMM_RX_QUEUE_H_

#include "internal_comm.h"

//! The size of the RX QUEUE
#define INTERNAL_COMM_RX_QUEUE_SIZE	20

//! The structure of the RX circular buffer
typedef struct rx_linked_list {
	//! The list of messages
	UC_MESSAGE message[INTERNAL_COMM_RX_QUEUE_SIZE];
	//! The index of the first message in the list
	unsigned char first;
	//! The index of the last message in the list
	unsigned char last;
  //! The number of messages in the queue
  unsigned char count;
} int_comm_rx_queue_struct;

void int_comm_rx_queue_add(UC_MESSAGE message);
UC_MESSAGE int_comm_rx_queue_get(void);
void int_comm_rx_queue_drop(void);
void int_comm_rx_queue_dropall(void);
void int_comm_rx_queue_init(void);
unsigned char int_comm_rx_queue_is_empty(void);
unsigned char int_comm_rx_queue_count(void);

#endif
