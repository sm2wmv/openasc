/*! \file wmv_bus/bus_ping.h 
 *  \brief The communication bus ping control
 *  \ingroup bus_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-04-22
 *  \code #include "wmv_bus/bus_ping.h" \endcode
 */


#ifndef _BUS_PING_H_
#define _BUS_PING_H_

typedef struct {
	unsigned char addr;
	unsigned char device_id;
	unsigned int time_last_ping;
	unsigned char data[2];
} bus_struct_ping_status;

void bus_ping_new_stamp(unsigned char from_addr, unsigned char device_id, unsigned char data_len, unsigned char *data);

#endif