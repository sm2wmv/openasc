/*! \file event_queue.c
 *  \brief Event queue
 *  \ingroup event_queue_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "event_queue.c" \endcode
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

/* event queue */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "event_queue.h"

/*! \brief Initialize the event queue */
void event_queue_init(void) {
	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		event_list[i].func = NULL;
		event_list[i].time_target = 0;
		event_list[i].id = 0;
	}
}

/*! \brief Insert a message into the event queue. It will end up on a position based on it's time_target
* \param event - The event that should be inserted into the queue 
* \return The position the event was inserted into, -1 means queue was full */
char event_queue_add(EVENT_MESSAGE event) {
	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		if (event_list[i].func == NULL) {
			event_list[i] = event;
			return(i);
		}
		else if (event_list[i].time_target > event.time_target) {
			EVENT_MESSAGE temp_event_list[EVENT_LIST_SIZE-i];

			for (unsigned char count=0;count<EVENT_LIST_SIZE-i;count++)
				temp_event_list[count] = event_list[count+i];

			event_list[i] = event;

			for (unsigned char count=0;count<EVENT_LIST_SIZE-i-1;count++)
				event_list[i+count+1] = temp_event_list[count];

			return(i);
		}
	}

	return(-1);
}

/*! \brief Removes a certain amount of numbers from the time_target
 *  \param remove_val The number we want to remove from all time targets */
void event_queue_wrap(unsigned int remove_val) {
	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		if (event_list[i].func != NULL)
			event_list[i].time_target -= remove_val;
	}
}

/*!\brief Retrieve the first message from the event queue.
* \return The first message in the queue */
EVENT_MESSAGE event_queue_get() {
	return(event_list[0]);
}

/*!\brief Checks if there is any event in the queue
* \return 1 if there is an event in the queue and 0 otherwise
*/
unsigned char event_in_queue(void) {	
	if (event_list[0].func != NULL)
		return(1);

	return(0);
}

/*! \brief Drops the first message in the queue */
void event_queue_drop(void) {
	if (event_list[0].func != NULL) {
		for (unsigned char i=1;i<EVENT_LIST_SIZE;i++)
			event_list[i-1] = event_list[i];
	
		event_list[EVENT_LIST_SIZE-1].func = NULL;
	}
}

/*! \brief Retrieve the number of items in the event queue
   * \return Numbers of items in the queue */
unsigned char event_queue_count(void) {
	unsigned char i;

	for (i=0;i<EVENT_LIST_SIZE;i++)
		if (event_list[i].func == NULL)
			break;

	return(i);
}

/*! \brief Erase all content in the event queue */
void event_queue_dropall(void) {
	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		event_list[i].func = NULL;
		event_list[i].time_target = 0;
		event_list[i].id = 0;
	}	
}


/*! \brief Drops all messages in the queue with a certain ID
 * \return the number of events that were dropped */
int event_queue_drop_id(unsigned char id) {
	EVENT_MESSAGE temp_event_list[EVENT_LIST_SIZE];
	
	unsigned char id_dropped_count=0;
	unsigned char count=0;

	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		if (event_list[i].id != id)
			temp_event_list[count++] = event_list[i];
		else
			id_dropped_count++;
	}

	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++) {
		if (i<(EVENT_LIST_SIZE-id_dropped_count))
			event_list[i] = temp_event_list[i];
		else {
			event_list[i].func = NULL;
			event_list[i].id = 0;
			event_list[i].time_target = 0;
		}	
	}

	return(id_dropped_count);
}

/*! \brief Check if a specific ID exist in the event queue
 * \param id The id we which to check for
 * \return 1 if it exist, 0 if it doesn't */
unsigned char event_queue_check_id(unsigned char id) {
	for (unsigned char i=0;i<EVENT_LIST_SIZE;i++)
		if (event_list[i].id == id)	//ID found, return 1
			return(1);

	//No id match found, return 0
	return(0);
}