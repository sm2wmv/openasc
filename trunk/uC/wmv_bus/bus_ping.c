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
#include "../global.h"

//TODO: Change the ping list to be dynamic, responding to the length which the master sends out, how many number of units on the bus

//! The ping list
static bus_struct_ping_status ping_list[DEF_NR_DEVICES];

static unsigned char mainbox_band_info[MAINBOX_DEVICE_COUNT];

/*! \brief Initialize the ping functions of the bus communication interface */
void bus_ping_init(void) {
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
		ping_list[i].addr = 0;
		ping_list[i].device_type = 0;
		ping_list[i].time_last_ping = 0;
		ping_list[i].flags = 0;
	}
	
	for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++)
    mainbox_band_info[i] = 0;
}

/*! \brief This function will update the ping list with the sent in arguments and reset the counter to 0 
 *  \param from_addr The address which the PING message was sent from
 *  \param device_type Which type of device this is
 *  \param data_len The number of bytes the data is
 *  \param data Additional data which might be used for status, for example current band information */
void bus_ping_new_stamp(unsigned char from_addr, unsigned char device_type, unsigned char data_len, unsigned char *data) {
/*	#ifdef DEBUG_COMPUTER_USART_ENABLED
		if (ping_list[from_addr-1].addr != from_addr) {
			printf("BUS_PING->ADDR CHANGED\n\r");
			
			printf("BUS_PING->OLD[%i]: %i\n\r",from_addr,ping_list[from_addr-1].addr);
			printf("BUS_PING->NEW[%i]: %i\n\r",from_addr,from_addr);
		}
		
		if (ping_list[from_addr-1].device_type != device_type) {
			printf("BUS_PING->DEVICE_TYPE CHANGED\n\r");
			
			printf("BUS_PING->OLD[%i]: %i\n\r",from_addr,ping_list[from_addr-1].device_type);
			printf("BUS_PING->NEW[%i]: %i\n\r",from_addr,device_type);
		}
		
		if (data_len > 0)
			if (ping_list[from_addr-1].data[0] != data[0]) {
				printf("BUS_PING->DATA CHANGED\n\r");
				printf("BUS_PING->OLD[%i][0]: %i\n\r",from_addr,ping_list[from_addr-1].data[0]);
				printf("BUS_PING->NEW[%i][0]: %i\n\r",from_addr,data[0]);
			}
			
		if (data_len > 1)
			if (ping_list[from_addr-1].data[1] != data[1]) {
				printf("BUS_PING->DATA CHANGED\n\r");
				printf("BUS_PING->OLD[%i][1]: %i\n\r",from_addr,ping_list[from_addr-1].data[1]);
				printf("BUS_PING->NEW[%i][1]: %i\n\r",from_addr,data[1]);
			}
	#endif
	*/
	ping_list[from_addr-1].addr = from_addr;
  ping_list[from_addr-1].device_type = device_type;
  ping_list[from_addr-1].flags |= (1<<PING_FLAG_DEV_PRESENT);
	
  //This will populate the array with details of which addresses are of DEVICE_ID_MAINBOX
  if (device_type == DEVICE_ID_MAINBOX) {
    for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++) {
      //If we find an unpopulated bit of the array, we put our device address here 
      if (mainbox_band_info[i] == 0)
        mainbox_band_info[i] = from_addr;
      
      //This will automatically break the loop if we already found a place where
      //our address is located
      if (mainbox_band_info[i] == data[1])
        break;
    }
  }
  
	if (data_len > 0) {
		for (unsigned char i=0;i<data_len;i++)
      if (data_len <= sizeof(ping_list[from_addr-1].data))
        ping_list[from_addr-1].data[i] = data[i];
      else
        break;
	}
	
	ping_list[from_addr-1].time_last_ping = 0;
	
	//Reset the flags which need to be reset
	ping_list[from_addr-1].flags &= ~(1<<PING_FLAG_PROCESSED);
}

/*! \brief This function will update the time counter which keeps track of the time stamps of the ping message. Should be called every ms */
void bus_ping_tick(void) {
	//Goes through the ping list and increase their last ping_time with 1ms
	for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
    bus_struct_ping_status *ps = &ping_list[i];
    if (ps->addr != 0) {
      if (ps->flags & (1<<PING_FLAG_DEV_PRESENT)) {
        ps->time_last_ping++;
        if (ps->time_last_ping > BUS_PING_TIMEOUT_LIMIT) {
          ps->flags &= ~(1<<PING_FLAG_DEV_PRESENT);
        }
      }
    }
	}
}

/*! \brief Check if the device with the specified from address is present or not
 * \return Return 1 if the device is present or 0 if it's not */
int8_t bus_ping_device_is_present(uint8_t fm_addr) {
  if (fm_addr > DEF_NR_DEVICES) {
    return 0;
  }
  return (ping_list[fm_addr-1].flags & (1<<PING_FLAG_DEV_PRESENT));
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
		if ((ping_list[i].time_last_ping > BUS_PING_TIMEOUT_LIMIT) && ((ping_list[i].flags & (1<<PING_FLAG_PROCESSED)) == 0))
      count++;
	}
	
	return(count);
}

/*! \brief Returns a ping data structure
 *  \param index The index of the ping structure we wish to retrieve from the list
 *  \return The ping data structure */
bus_struct_ping_status* bus_ping_get_ping_data(unsigned char index) {
	return(&ping_list[index]);
}

/*! \brief Returns a pointer to the array which contains information of what the DEVICE_ID_MAINBOX unit addresses are 
 *  \return A pointer to the char array which contains the units addresses */
unsigned char* bus_ping_get_mainbox_adresses(void) {
  return((unsigned char*)mainbox_band_info);
}

/*! \brief Returns the device type of a certain ping data structure
 *  \param index The index of the device type we wish to retrieve from the list
 *  \return The device type of the specified index */
unsigned char bus_ping_get_device_type(unsigned char index) {
	unsigned char device_type = ping_list[index].device_type;

	return(device_type);
}

/*! \brief Clears a ping address, used when a unit is shut down
 *  \param addr The address of the unit */
void bus_ping_clear_device(unsigned char addr) {
	ping_list[addr-1].addr = 0;
	ping_list[addr-1].device_type = 0;
	ping_list[addr-1].time_last_ping = 0;
	ping_list[addr-1].flags = 0;  
}

#ifdef DEBUG_COMPUTER_USART_ENABLED
	void bus_ping_print_list(void) {
		for (unsigned char i=0;i<DEF_NR_DEVICES;i++) {
			printf("======= %i =======\n\r",i);
			printf("ping_list.addr: %i\n\r",ping_list[i].addr);
			printf("ping_list.device_type: %i\n\r",ping_list[i].device_type);
			printf("ping_list.time_last_ping: %i\n\r",ping_list[i].time_last_ping);
			printf("ping_list.flags: %i\n\r",ping_list[i].flags);
			printf("ping_list.data[0]: %i\n\r",ping_list[i].data[0]);
			printf("ping_list.data[1]: %i\n\r",ping_list[i].data[1]);
			printf("----------------------\n\r");
		}
	}
#endif
