/*! \file internal_comm_rx_queue.c
 *  \brief The internal communication RX QUEUE
 *  \ingroup internal_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm_rx_queue.c" \endcode
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

/* FIFO queue */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "internal_comm_rx_queue.h"

//! The RX queue
int_comm_rx_queue_struct int_comm_rx_queue;

void int_comm_rx_queue_init(void) {
	int_comm_rx_queue.first = 0;
	int_comm_rx_queue.last = 0;
}

/*! \brief Insert a message into the TX queue (FIFO)
 * \param message - The message that should be inserted to the queue
 */
void int_comm_rx_queue_add(UC_MESSAGE message) {
	int_comm_rx_queue.message[int_comm_rx_queue.last++] = message;
	
	if (int_comm_rx_queue.last >= INTERNAL_COMM_RX_QUEUE_SIZE)
		int_comm_rx_queue.last = 0;
	
	if (int_comm_rx_queue.last == int_comm_rx_queue.first)
		int_comm_rx_queue.first++;
	
	if (int_comm_rx_queue.first >= INTERNAL_COMM_RX_QUEUE_SIZE)
		int_comm_rx_queue.first = 0;	
}

/*!\brief Retrieve the first message from the FIFO TX queue.
 * \return The first message in the queue
 */
UC_MESSAGE int_comm_rx_queue_get(void) {
	//Return the message (content of the first node)
	return(int_comm_rx_queue.message[int_comm_rx_queue.first]);
}

/*! Drops the first message in the queue Frees up the memory space aswell.
 */
void int_comm_rx_queue_drop(void) {
	int_comm_rx_queue.first++;
	
	if (int_comm_rx_queue.first >= INTERNAL_COMM_RX_QUEUE_SIZE)
		int_comm_rx_queue.first = 0;
}

/*! \brief Erase all content in the TX queue
 * \return The number of items that were cleared
 */
void int_comm_rx_queue_dropall(void) {
	int_comm_rx_queue.first = 0;
	int_comm_rx_queue.last = 0;
}

/*! \brief Check if the queue is empty
 *	\return 1 if the queue is empty and 0 otherwise
 */
unsigned char int_comm_rx_queue_is_empty(void) {
	if (int_comm_rx_queue.first == int_comm_rx_queue.last)
		return(1);
	else
		return(0);
}
