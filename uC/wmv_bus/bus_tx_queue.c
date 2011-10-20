/*! \file wmv_bus/bus_tx_queue.c \brief FIFO queue for the TXed messages.
 *  \ingroup bus_group 
 *  \brief FIFO queue for the TXed messages.
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus_tx_queue.c" \endcode
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

#include "bus_tx_queue.h"
#include "bus.h"

#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#include "../front_panel/led_control.h"
	#include "../front_panel/event_handler.h"
	#include "../front_panel/errors.h"
#endif


//! Variable that keeps track of how much of the TX queue that is being used
unsigned char bus_tx_queue_size;

//! The tx queue
tx_queue_struct tx_queue;

/*! \brief Initialize the TX queue */
void tx_queue_init(void) {
	tx_queue.first = 0;
	tx_queue.last = 0;
	
	bus_tx_queue_size = 0;
}

/*! \brief Insert a message into the TX queue (FIFO)
* \param message - The message that should be inserted to the queue
*/
void tx_queue_add(BUS_MESSAGE message) {
	disable_bus_interrupt();
	
  bus_tx_queue_size++;
	
	//printf("SIZE: %i\n",bus_tx_queue_size);	
	
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		if (bus_tx_queue_size > BUS_TX_QUEUE_SIZE) {
			led_set_error(LED_STATE_ON);
			
			//Set the error flag
			error_handler_set(ERROR_TYPE_BUS_TX_QUEUE_FULL,1,0);

		}	
	#endif
	
	tx_queue.message[tx_queue.last++] = message;
		
	if (tx_queue.last >= BUS_TX_QUEUE_SIZE)
		tx_queue.last = 0;
	
	if (tx_queue.last == tx_queue.first)
		tx_queue.first++;
	
	if (tx_queue.first >= BUS_TX_QUEUE_SIZE)
		tx_queue.first = 0;

  enable_bus_interrupt();
}

BUS_MESSAGE tx_queue_get_pos(unsigned char pos) {
  disable_bus_interrupt();

  BUS_MESSAGE mess = tx_queue.message[tx_queue.first+pos];
  
  enable_bus_interrupt();
  
  return(mess);
}

/*!\brief Retrieve the first message from the FIFO TX queue.
* \return The first message in the queue
*/
BUS_MESSAGE tx_queue_get(void) {
	disable_bus_interrupt();
  
  BUS_MESSAGE mess = tx_queue.message[tx_queue.first];
  
  enable_bus_interrupt();
  
  //Return the message (content of the first node)
	return(mess);
}

/*! Drops the first message in the queue Frees up the memory space aswell.
*/
void tx_queue_drop(void) {
  disable_bus_interrupt();
  
  tx_queue.first++;
	bus_tx_queue_size--;

	if (tx_queue.first >= BUS_TX_QUEUE_SIZE)
		tx_queue.first = 0;
  
  enable_bus_interrupt();  
}

/*! \brief Erase all content in the TX queue
 * \return The number of items that were cleared
 */
void tx_queue_dropall(void) {
  disable_bus_interrupt();
  
  tx_queue.first = 0;
	tx_queue.last = 0;
	bus_tx_queue_size = 0;
  
  enable_bus_interrupt();  
}

/*! \brief Check if the queue is empty
 *	\return 1 if the queue is empty and 0 otherwise */
unsigned char tx_queue_is_empty(void) {
	disable_bus_interrupt();
  
  unsigned char state = 0;
  
  if (tx_queue.first == tx_queue.last)
		state = 1;
  
  enable_bus_interrupt();
  
  return(state);
}

/*! \brief Get how much of the TX queue that is currently being used 
 *  \return How much of the queue is being used */
unsigned char tx_queue_size(void) {
	disable_bus_interrupt();
  
  unsigned char size = bus_tx_queue_size;
  
  enable_bus_interrupt();
  
  return(size);
}