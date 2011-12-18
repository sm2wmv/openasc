/*! \file internal_comm_tx_queue.c
 *  \brief The internal communication TX QUEUE
 *  \ingroup internal_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm_tx_queue.c" \endcode
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

#include "internal_comm_tx_queue.h"

volatile static char data_changed = 0;

//! The TX queue
static int_comm_tx_queue_struct int_comm_tx_queue;

unsigned char int_comm_tx_queue_curr_size = 0;

/*! Initialize the internal communication TX queue */
void int_comm_tx_queue_init(void) {
	int_comm_tx_queue.first = 0;
	int_comm_tx_queue.last = 0;
  int_comm_tx_queue_curr_size = 0;
}

/*! \brief Insert a message into the TX queue (FIFO)
 * \param message - The message that should be inserted to the queue
 */
void int_comm_tx_queue_add(UC_MESSAGE message) {
  data_changed = 1;
  
  int_comm_tx_queue.message[int_comm_tx_queue.last++] = message;
	
	if (int_comm_tx_queue.last >= INTERNAL_COMM_TX_QUEUE_SIZE)
		int_comm_tx_queue.last = 0;
	
	if (int_comm_tx_queue.last == int_comm_tx_queue.first)
		int_comm_tx_queue.first++;
	
	if (int_comm_tx_queue.first >= INTERNAL_COMM_TX_QUEUE_SIZE)
		int_comm_tx_queue.first = 0;
  
  int_comm_tx_queue_curr_size++;
}

/*!\brief Retrieve the pos message from the FIFO TX queue.
 * \return The [pos] message in the queue
 */
UC_MESSAGE int_comm_tx_queue_get_pos(unsigned char pos) {
  data_changed = 0;
  
  UC_MESSAGE mess = int_comm_tx_queue.message[int_comm_tx_queue.first+pos];
  
  /*if (data_changed) {
    disable_int_comm_interrupt();
    mess = int_comm_tx_queue.message[int_comm_tx_queue.first+pos];
    enable_int_comm_interrupt();
  }*/
  
  //Return the message (content of the first node)
  return(mess);  
}

/*!\brief Retrieve the first message from the FIFO TX queue.
 * \return The first message in the queue
 */
UC_MESSAGE int_comm_tx_queue_get(void) {	
  data_changed = 0;
  
  UC_MESSAGE mess = int_comm_tx_queue.message[int_comm_tx_queue.first];

  /*if (data_changed) {
    disable_int_comm_interrupt();
    mess = int_comm_tx_queue.message[int_comm_tx_queue.first];
    enable_int_comm_interrupt();
  }*/
  
  //Return the message (content of the first node)
  return(mess);
}

/*! Drops the first message in the queue Frees up the memory space aswell.
 */
void int_comm_tx_queue_drop(void) {
  int_comm_tx_queue.first++;
	
  if (int_comm_tx_queue.first >= INTERNAL_COMM_TX_QUEUE_SIZE)
    int_comm_tx_queue.first = 0;

  if (int_comm_tx_queue_curr_size > 0)
    int_comm_tx_queue_curr_size--;
}

/*! \brief Erase all content in the TX queue
 * \return The number of items that were cleared
 */
void int_comm_tx_queue_dropall(void) {
  int_comm_tx_queue.first = 0;
	int_comm_tx_queue.last = 0;
  int_comm_tx_queue_curr_size = 0;
}

/*! \brief Check if the queue is empty
 *	\return 1 if the queue is empty and 0 otherwise
 */
unsigned char int_comm_tx_queue_is_empty(void) {
  unsigned char state = 0;
  
  if (int_comm_tx_queue.first == int_comm_tx_queue.last)
		state = 1;
  
  return(state);
}

unsigned char int_comm_tx_queue_size(void) {
  unsigned char size = int_comm_tx_queue_curr_size;
  
  return(size);
}