/*! \file xbee/main.c 
 *  \defgroup xbee_group XBEE interface version 1
 *  \brief Main file of the XBEE 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2016-10-12
 *  \code #include "xbee/main.c" \endcode
 */
//    Copyright (C) 2016  Mikael Larsmark, SM2WMV
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "xbee_interface.h"
#include "usart.h"
#include "../delay.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_ping.h"
#include "../wmv_bus/bus_commands.h"

volatile uint8_t dev_addr_HiZ[8] = {0x00, 0x13, 0xA2, 0x00, 0x40, 0xD5, 0x0A, 0x4D};


//! Contains info if the module is a positive or negative driver
volatile unsigned char device_id;

//! Counter to keep track of the numbers of ticks from timer0
volatile unsigned int counter_compare0 = 0;
//! Counter to keep track of the time elapsed since the last sync message was sent
volatile unsigned int counter_sync=0;
//! Counter to keep track of when to send a ping out on the bus
volatile unsigned int counter_ping_interval=0;

volatile unsigned int counter_ms = 0;

//! A status structure of the driver unit outputs
driver_status_struct	driver_status;

unsigned char deactivate_output_list[20];
unsigned char deactivate_output_list_len = 0;

static struct_xbee_api_frame new_frame;
static uint8_t flag_xbee_frame_available = 0;

/*! \brief Activate a driver output
* This function is used to activate an output on the driver unit. It will remember
* which device that sent the request for an activation so that the driver_unit will
* remember it when that device goes offline so it can shut the outputs off.
* \param from_addr The device that sent the request of activating an output
* \param index The index of which output to activate 
* \param type The type of output this is, usually is the bus command */
void activate_output(unsigned char from_addr, unsigned char index, unsigned char type) {
	driver_status.driver_output_state |= (1<<(index-1));
	driver_status.driver_output_owner[index-1] = from_addr;

	driver_status.driver_output_type[index-1] = type;

	switch (index) {
		case 1 :	//DIR NW, both relays off
      xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x44, 0x34, 0x04);
      xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x50, 0x32, 0x04);
			break;
		case 2 : //DIR NE, Relay 1 On, Relay 2 off
      xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x44, 0x34, 0x04);
			xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x50, 0x32, 0x05);
      break;
		case 3 :  //DIR SE, Relay 1 Off, Relay 2 On
      xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x44, 0x34, 0x05);
			xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x50, 0x32, 0x04);
			break;
		case 4 :  //DIR SW, Relay 1 On, Relay 2 On
      xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x44, 0x34, 0x05);
			xbee_interface_transmit_remote_at_cmd((uint8_t *)dev_addr_HiZ, 0x50, 0x32, 0x05);
      break;
		case 5 :
			break;
		case 6 :
			break;
		case 7 :
			break;
		case 8 :
			break;
		case 9 :
			break;
		case 10 :
			break;
		case 11 :
			break;
		case 12 :
			break;
		case 13 :
			break;
		case 14 :
			break;
		case 15 :
			break;
		case 16 :
			break;
		case 17 :
			break;
		case 18 :
			break;
		case 19 :
			break;
		case 20 :
			break;
		default: 
			break;
	}
}

/*! \brief Deactivate a driver output
* This function is used to deactivate an output on the driver unit. It will remember
* which device that sent the request for an deactivation so that the driver_unit will
* remember it when that device goes offline so it can shut the outputs off.
* \param from_addr The device that sent the request of deactivating the output
* \param index The index of which output to deactivate */
void deactivate_output(unsigned char from_addr, unsigned char index) {
	if (from_addr == driver_status.driver_output_owner[index-1]) {
		driver_status.driver_output_state &= ~(1<<(index-1));
		driver_status.driver_output_owner[index-1] = 0;
		driver_status.driver_output_type[index-1] = 0;

		switch (index) {
			case 1 :
				break;
			case 2 :	
				break;
			case 3 :	
				break;
			case 4 :
				break;
			case 5 :
				break;
			case 6 :
				break;
			case 7 :
				break;
			case 8 :
				break;
			case 9 :
				break;
			case 10 :
				break;
			case 11 :
				break;
			case 12 :
				break;
			case 13 :
				break;
			case 14 :
				break;
			case 15 :
				break;
			case 16 :
				break;
			case 17 :
				break;
			case 18 :
				break;
			case 19 :
				break;
			case 20 :
				break;
		}
	}
}

/*! \brief Parse a message and exectute the proper commands
* This function is used to parse a message that was receieved on the bus that is located
* in the RX queue. */
void bus_parse_message(BUS_MESSAGE *bus_message) {
	if (bus_message->cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message->from_addr);
	else if (bus_message->cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message->from_addr, bus_message->data[0]);
	else if (bus_message->cmd == BUS_CMD_PING) {
		//ADD the ping to the ping_list
		if (bus_message->length > 1)
			bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], bus_message->length-1, (unsigned char *)(bus_message->data+1));
		else
			bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], 0, 0);
	}
	switch (bus_message->cmd) {
		case BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT);
			break;
		case BUS_CMD_DRIVER_GET_STATUS:
			
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_owner[i-1] == bus_message->from_addr)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_RX_BAND_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT1_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT1_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT2_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT2_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT3_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT3_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				activate_output(bus_message->from_addr,bus_message->data[i], BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT4_OUTPUT:
			for (unsigned char i=0;i<bus_message->length;i++)
				deactivate_output(bus_message->from_addr,bus_message->data[i]);
			break;
		case BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT4_OUTPUTS:
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT)
					deactivate_output(bus_message->from_addr,i);
			break;
		case BUS_CMD_SYNC:
			break;
		default:
			break;
	}
}

void main_parse_xbee_message(uint8_t *rx_data, uint8_t length, uint8_t *source_addr) {
  uint8_t rf_data[20];

  if (length < 6)
    return;

  
}

void main_xbee_frame_rxed(struct_xbee_api_frame frame) {
  new_frame = frame;
  flag_xbee_frame_available = 1;
}


void main_parse_xbee_frame(struct_xbee_api_frame frame) {
  if (frame.frame_type == XBEE_API_FRAME_RX_PACKAGE) {
    //Extract the data command that was sent
    uint8_t rx_data[frame.length-12];
    uint8_t source_addr[8];
    memcpy(source_addr, frame.frame_data, 8);

    for (uint8_t i=11;i<frame.length-1;i++) {
      rx_data[i-11] = frame.frame_data[i];
    }

    main_parse_xbee_message((uint8_t *)&rx_data, sizeof(rx_data), (uint8_t *)&source_addr);
  }
}

/*! \brief Read the external DIP-switch.
* This function is used to read the external offset address on the driver_unit.
*	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
	return(~(PINE >> 2) & 0x0F);
}


/*! Main function of the driver unit */
int main(void)
{
	cli();
	
	delay_ms(250);

	init_ports();

	delay_ms(250);

	/* Initialize various hardware resources */

	/* Read the external address of the device */
	bus_set_address(22);

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	for (unsigned char i=0;i<bus_get_address();i++)
		delay_ms(90);

	//Initialize the communication bus	
	bus_init();
	
	if ((BUS_BASE_ADDR+read_ext_addr()) == 0x01)
		bus_set_is_master(1,DEF_NR_DEVICES);
	else
		bus_set_is_master(0,0);

	
	usart0_init(95);
	 
	xbee_interface_init(usart0_transmit, main_xbee_frame_rxed);
	
  //Reset XBEE module
  PORTA &= ~(1<<2);
  delay_ms(100);
  PORTA |= (1<<2);
  delay_ms(100);
  
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();
	
	sei();

	unsigned char device_count = bus_get_device_count();

	BUS_MESSAGE mess;
  
	while(1) {
		if (bus_check_rx_status(&mess)) {
		  bus_parse_message(&mess);
		}

		bus_check_tx_status();  
				
		//If this device should act as master it should send out a SYNC command
		//and also the number of devices (for the time slots) that are active on the bus
		if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, &device_count);
				counter_sync = 0;
			}
		}

		if (bus_allowed_to_send()) {
			//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, &device_id);

				counter_ping_interval = 0;
			}
		}
		
		
		if (flag_xbee_frame_available) {
		  main_parse_xbee_frame(new_frame);

		  flag_xbee_frame_available = 0;
		}
	}

	return (0);
}

ISR(SIG_USART0_RECV) {
  xbee_interface_rx_char(UDR0);
}

ISR(SIG_USART0_DATA) {
}

uint8_t count_pos = 0;

/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_ms++;
	counter_sync++;
	counter_ping_interval++;
	counter_compare0++;
  
	if ((counter_ms % 100) == 0)
		bus_ping_tick();
	
	xbee_interface_ms_tick();
}