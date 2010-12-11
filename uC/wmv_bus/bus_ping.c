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

//! The ping list
bus_struct_ping_status ping_list[DEF_NR_DEVICES];

/*! \brief Initialize the ping functions of the bus communication interface */
void bus_ping_init(void) {
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
		ping_list[i].addr = 0;
		ping_list[i].device_type = 0;
		ping_list[i].time_last_ping = 0;
		ping_list[i].flags = 0;
	}
}

/*! \brief This function will update the ping list with the sent in arguments and reset the counter to 0 
 *  \param from_addr The address which the PING message was sent from
 *  \param device_type Which type of device this is
 *  \param data_len The number of bytes the data is
 *  \param data Additional data which might be used for status, for example current band information */
void bus_ping_new_stamp(unsigned char from_addr, unsigned char device_type, unsigned char data_len, unsigned char *data) {
	ping_list[from_addr-1].addr = from_addr;
	ping_list[from_addr-1].device_type = device_type;
	
	if (data_len > 0) {
		for (unsigned char i=0;i<data_len;i++)
			ping_list[from_addr-1].data[i] = data[i];
	}
	
	ping_list[from_addr-1].time_last_ping = 0;
	
	//Reset the flags which need to be reset
	ping_list[from_addr-1].flags &= ~(1<<PING_FLAG_PROCESSED);
}

/*! \brief This function will update the time counter which keeps track of the time stamps of the ping message. Should be called every ms */
void bus_ping_tick(void) {
	//Goes through the ping list and increase their last ping_time with 1ms
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
		ping_list[i].time_last_ping++;
	}
}

/*! \brief This function will return a ping which has failed and will mark it that it has been reported 
 *  \return A pointer to a structure of type bus_struct_ping_status which contains information of the failed ping */
bus_struct_ping_status bus_ping_get_failed_ping(void) {
	unsigned char temp=255;
	
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++)
		if ((ping_list[i].time_last_ping > BUS_PING_TIMEOUT_LIMIT) && ((ping_list[i].flags & (1<<PING_FLAG_PROCESSED)) == 0)) {
			
			temp = i;
			ping_list[i].flags |= (1<<PING_FLAG_PROCESSED);
		}
		
	if (temp != 255)
		return(ping_list[temp]);
}

/*! \brief Goes through the ping list and checks how many has timed out
 *  \return The number of failed pings */
unsigned char bus_ping_get_failed_count(void) {
	unsigned char count=0;
	
	//Goes through the ping list and check how many failed pings there is
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
		if (ping_list[i].time_last_ping > BUS_PING_TIMEOUT_LIMIT)
			count++;
	}
	
	return(count);
}

/*! \brief Returns a ping data structure
 *  \param index The index of the ping structure we wish to retrieve from the list
 *  \return The ping data structure */
bus_struct_ping_status bus_ping_get_ping_data(unsigned char index) {
	return(ping_list[index]);
}

/*! \brief Returns the device type of a certain ping data structure
 *  \param index The index of the device type we wish to retrieve from the list
 *  \return The device type of the specified index */
unsigned char bus_ping_get_device_type(unsigned char index) {
	return(ping_list[index].device_type);
}
