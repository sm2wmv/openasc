/*! \file wmv_bus/bus_ping.h 
 *  \brief The communication bus ping control
 *  \ingroup bus_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-04-22
 *  \code #include "wmv_bus/bus_ping.h" \endcode
 */


#ifndef _BUS_PING_H_
#define _BUS_PING_H_

//! The timeout for the bus ping. After this time has passed a device is considered "dead"
#define BUS_PING_TIMEOUT_LIMIT	6000

//! Bit is set if the ping timeout has been processed
#define PING_FLAG_PROCESSED	0

//! Struct which contains information of the bus ping information	
typedef struct {
	//! The address of the device
	unsigned char addr;
	//! The type of device it is, see bus.h for details
	unsigned char device_type;
	//! The time since the last ping did occur in ms
	unsigned int time_last_ping;
	//! Flags, see defines above
	unsigned char flags;
	//! Data from the device, content varies depending on device_id
	unsigned char data[2];
} bus_struct_ping_status;

	void bus_ping_init(void);
	void bus_ping_tick(void);
	void bus_ping_new_stamp(unsigned char from_addr, unsigned char device_type, unsigned char data_len, unsigned char *data);
	bus_struct_ping_status bus_ping_get_failed_ping(void);
	unsigned char bus_ping_get_failed_count(void);
	
	bus_struct_ping_status bus_ping_get_ping_data(unsigned char index);
	unsigned char bus_ping_get_device_type(unsigned char index);
#endif