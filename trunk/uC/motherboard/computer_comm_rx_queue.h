/*! \file computer_comm_rx_queue.h
 *  \brief The computer communication RX QUEUE
 *  \ingroup computer_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-22
 *  \code #include "computer_comm_rx_queue.h" \endcode
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

#ifndef _COMPUTER_COMM_RX_QUEUE_H_
#define _COMPUTER_COMM_RX_QUEUE_H_

#include "computer_comm.h"

//! The size of the RX QUEUE
#define COMPUTER_COMM_RX_QUEUE_SIZE	20

//! The structure of the RX circular buffer
typedef struct rx_linked_list {
	//! The list of messages
	COMM_MESSAGE message[COMPUTER_COMM_RX_QUEUE_SIZE];
	//! The index of the first message in the list
	unsigned char first;
	//! The index of the last message in the list
	unsigned char last;
  //! The number of messages in the queue
  unsigned char count;
} computer_comm_rx_queue_struct;

void computer_comm_rx_queue_add(COMM_MESSAGE message);
COMM_MESSAGE computer_comm_rx_queue_get(void);
void computer_comm_rx_queue_drop(void);
void computer_comm_rx_queue_dropall(void);
void computer_comm_rx_queue_init(void);
unsigned char computer_comm_rx_queue_is_empty(void);
unsigned char computer_comm_rx_queue_count(void);

#endif
