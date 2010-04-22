/*! \file wmv_bus/bus_ping.c
 *  \brief The communication bus ping control
 *  \ingroup bus_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-04-22
 *  \code #include "wmv_bus/bus_ping.c" \endcode
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DEVICE_TYPE_COMPUTER
	#include <avr/io.h>
	#include <avr/interrupt.h>
#endif

#include "bus.h"
#include "bus_ping.h"

//TODO: Change the ping list to be dynamic, responding to the length which the master sends out, how many number of units on the bus
//TODO: Add functions to retrieve failed PINGs

bus_struct_ping_status ping_list[DEF_NR_DEVICES];

/*! \brief This function will update the ping list with the sent in arguments and reset the counter to 0 
 *  \param from_addr The address which the PING message was sent from
 *  \param device_id Which type of device this is
 *  \param data Additional data which might be used for status */
void bus_ping_new_stamp(unsigned char from_addr, unsigned char device_id, unsigned char data_len, unsigned char *data) {
	ping_list[from_addr-1].addr = from_addr;
	ping_list[from_addr-1].device_id = device_id;
	
	if (data_len > 0) {
		for (unsigned char i=0;i<data_len;i++)
			ping_list[from_addr-1].data[i] = data[i];
	}
	
	ping_list[from_addr-1].time_last_ping = 0;
}

/*! \brief This function will update the time counter which keeps track of the time stamps of the ping message. Should be called every ms */
void bus_ping_tick(void) {
	//Goes through the ping list and increase their last ping_time with 1ms
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
		ping_list[i].time_last_ping++;
	}
}
