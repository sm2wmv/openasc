/*! \file wmv_bus/bus_rx_queue.c 
 *  \ingroup bus_group
 *  \brief FIFO queue for the RXed messages.
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "bus_rx_queue.c" \endcode
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

#include "bus_rx_queue.h"
#include "bus.h"

#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#include "../front_panel/led_control.h"
	#include "../front_panel/event_handler.h"
	#include "../front_panel/errors.h"
#endif


//! Variable keeps track of how much of the queue that is currently used
unsigned char bus_rx_queue_size;

//! The rx queue
rx_queue_struct rx_queue;

/*! \brief Initialize the RX queue */
void rx_queue_init(void) {
	rx_queue.first = 0;
	rx_queue.last = 0;
	bus_rx_queue_size = 0;
}

/*! \brief Insert a message into the RX queue (FIFO)
* \param message - The message that should be inserted to the queue
*/
void rx_queue_add(BUS_MESSAGE message) {
	bus_rx_queue_size++;

	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		if (bus_rx_queue_size > BUS_RX_QUEUE_SIZE) {
			led_set_error(LED_STATE_ON);
			
			//Set the error flag
			event_set_error(ERROR_TYPE_BUS_RX_QUEUE_FULL,1);

		}	
	#endif

	
	rx_queue.message[rx_queue.last++] = message;
	
	if (rx_queue.last >= BUS_RX_QUEUE_SIZE)
		rx_queue.last = 0;
	
	if (rx_queue.last == rx_queue.first)
		rx_queue.first++;
	
	if (rx_queue.first >= BUS_RX_QUEUE_SIZE)
		rx_queue.first = 0;	
}

/*!\brief Retrieve the first message from the FIFO RX queue.
* \return The first message in the queue
*/
BUS_MESSAGE rx_queue_get() {
	//Return the message (content of the first node)
	return(rx_queue.message[rx_queue.first]);
}

/*! Drops the first message in the queue Frees up the memory space aswell.
*/
void rx_queue_drop(void) {
	rx_queue.first++;
	
	bus_rx_queue_size--;
	
	if (rx_queue.first >= BUS_RX_QUEUE_SIZE)
		rx_queue.first = 0;
}

/*! \brief Erase all content in the RX queue
 * \return The number of items that were cleared
 */
void rx_queue_dropall(void) {
	rx_queue.first = 0;
	rx_queue.last = 0;
	
	bus_rx_queue_size = 0;
}

/*! \brief Check if the queue is empty
 *	\return 1 if the queue is empty and 0 otherwise */
unsigned char rx_queue_is_empty(void) {
	if (rx_queue.first == rx_queue.last)
		return(1);
	else
		return(0);
}

/*! \brief Get how much size of the RX queue is used at the moment 
 *  \return The size of the queue that is used */
unsigned char rx_queue_size(void) {
	return(bus_rx_queue_size);
}