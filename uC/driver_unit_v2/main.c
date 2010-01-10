/*! \file main.c \brief Main file of the driver unit. 
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-03-16
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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

#include "main.h"
#include "board.h"
#include "init.h"
#include "../i2c.h"
#include "../delay.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//! A status structure of the driver unit outputs
driver_status_struct	driver_status;

//! The address of the LM76 temperature sensor connected to the I2C bus
#define LM76_ADDR 0x90

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of the time elapsed since the last sync message was sent
unsigned int counter_sync=0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval=0;

/*! \brief Activate a driver output
* This function is used to activate an output on the driver unit. It will remember
* which device that sent the request for an activation so that the driver_unit will
* remember it when that device goes offline so it can shut the outputs off.
* \param from_addr The device that sent the request of activating an output
* \param index The index of which output to activate */
void activate_output(unsigned char from_addr, unsigned char index, unsigned char type) {
	driver_status.driver_output_state |= (1<<(index-1));
	driver_status.driver_output_owner[index-1] = from_addr;

	driver_status.driver_output_type[index-1] = type;

	switch (index) {
		case 1 :	PORTB |= (1<<4);
							break;
		case 2 :	PORTB |= (1<<5);
							break;
		case 3 :	PORTB |= (1<<6);
							break;
		case 4 :	PORTB |= (1<<7);
							break;
		case 5 :	PORTG |= (1<<3);
							break;
		case 6 :	PORTG |= (1<<4);
							break;
		case 7 :	PORTD |= (1<<4);
							break;
		case 8 :	PORTD |= (1<<5);
							break;
		case 9 :	PORTD |= (1<<6);
							break;
		case 10 :	PORTD |= (1<<7);
							break;
		case 11 :	PORTC |= (1<<0);
							break;
		case 12 :	PORTC |= (1<<1);
							break;
		case 13 :	PORTC |= (1<<2);
							break;
		case 14 :	PORTC |= (1<<3);
							break;
		case 15 :	PORTC |= (1<<4);
							break;
		case 16 :	PORTC |= (1<<5);
							break;
		case 17 :	PORTC |= (1<<6);
							break;
		case 18 :	PORTC |= (1<<7);
							break;
		case 19 :	PORTG |= (1<<2);
							break;
		case 20 :	PORTA |= (1<<7);
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
	driver_status.driver_output_state &= ~(1<<(index-1));
	driver_status.driver_output_owner[index-1] = from_addr;
	driver_status.driver_output_type[index-1] = 0;

	switch (index) {
		case 1 :	PORTB &= ~(1<<4);
							break;
		case 2 :	PORTB &= ~(1<<5);
							break;
		case 3 :	PORTB &= ~(1<<6);
							break;
		case 4 :	PORTB &= ~(1<<7);
							break;
		case 5 :	PORTG &= ~(1<<3);
							break;
		case 6 :	PORTG &= ~(1<<4);
							break;
		case 7 :	PORTD &= ~(1<<4);
							break;
		case 8 :	PORTD &= ~(1<<5);
							break;
		case 9 :	PORTD &= ~(1<<6);
							break;
		case 10 :	PORTD &= ~(1<<7);
							break;
		case 11 :	PORTC &= ~(1<<0);
							break;
		case 12 :	PORTC &= ~(1<<1);
							break;
		case 13 :	PORTC &= ~(1<<2);
							break;
		case 14 :	PORTC &= ~(1<<3);
							break;
		case 15 :	PORTC &= ~(1<<4);
							break;
		case 16 :	PORTC &= ~(1<<5);
							break;
		case 17 :	PORTC &= ~(1<<6);
							break;
		case 18 :	PORTC &= ~(1<<7);
							break;
		case 19 :	PORTG &= ~(1<<2);
							break;
		case 20 :	PORTA &= ~(1<<7);
							break;
		default: 
							break;
	}
}

/*! \brief Retrieve the temperature from the LM76 sensor
* This function is used to retrieve the temperature from the LM76 sensor that
* does exist on the driver_unit.
* \return The temperature but not in float format */
unsigned int lm76_get_temp(void) {
	unsigned char temp[2];
	i2cMasterReceiveNI(LM76_ADDR,2,(unsigned char *)temp);

	return((((temp[0]<<8) | temp[1]) >> 3));
}

/*! \brief Parse a message and exectute the proper commands
* This function is used to parse a message that was receieved on the bus that is located
* in the RX queue. */
void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();

	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked();
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked();
	else if (bus_message.cmd == BUS_CMD_PING) {
		
	}
	else {
		if (bus_message.cmd == BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT) {
			for (unsigned char i=0;i<bus_message.length;i++)
				activate_output(bus_message.from_addr,bus_message.data[i], BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_DEACTIVATE_ANT_OUTPUT) {
			for (unsigned char i=0;i<bus_message.length;i++)
				deactivate_output(bus_message.from_addr,bus_message.data[i]);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT) {
			for (unsigned char i=0;i<bus_message.length;i++)
				activate_output(bus_message.from_addr,bus_message.data[i], BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT) {
			for (unsigned char i=0;i<bus_message.length;i++)
				activate_output(bus_message.from_addr,bus_message.data[i], BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_GET_STATUS) {
			
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_DEACTIVATE_ALL_OUTPUTS) {
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_owner[i-1] == bus_message.from_addr)
					deactivate_output(bus_message.from_addr,i);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS) {
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT)
					deactivate_output(bus_message.from_addr,i);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_DEACTIVATE_RX_ANT_OUTPUT) {
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT)
					deactivate_output(bus_message.from_addr,i);
		}
		else if (bus_message.cmd == BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS) {
			for (unsigned char i=1;i<=20;i++)
				if (driver_status.driver_output_type[i-1] == BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT)
					deactivate_output(bus_message.from_addr,i);
		}
		else if (bus_message.cmd == BUS_CMD_GET_TEMPERATURE) {
			unsigned int temp_t = lm76_get_temp();
			unsigned char temp[2];
			temp[0] = (temp_t & 0xFF00) >> 8;
			temp[1] = (temp_t & 0x00FF);

			bus_add_tx_message(bus_get_address(), bus_message.from_addr, 0, BUS_CMD_GET_TEMPERATURE,2, temp);
		}
		else if (bus_message.cmd == BUS_CMD_SYNC) {
		}
		else
			bus_send_nack(bus_message.from_addr);
	}
	
	//Drop the message
	rx_queue_drop();
}

/*! \brief Read the external DIP-switch.
* This function is used to read the external offset address on the driver_unit.
*	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
	return(~(PINE >> 2) & 0x0F);
}

unsigned char set_ptt_led_status(unsigned char state) {
	PORTF = state;
}

int main(void)
{
	cli();

	delay_ms(100);

	driver_status.driver_output_state = 0;

	/* Read the external address of the device */
	bus_set_address(BUS_BASE_ADDR+read_ext_addr());

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	delay_ms(7 * bus_get_address());

	/* Initialize various hardware resources */
	init_ports();

	//Initialize the communication bus	
	bus_init();
	
	//The device should not be master
	bus_set_is_master(0,0);

	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();	
	
	sei();

	for (unsigned char i=1;i<=20;i++)
		deactivate_output(0x00,i);

	while(1) {
		if (!rx_queue_is_empty()) {
			bus_parse_message();
		}

		if (!tx_queue_is_empty())
			bus_check_tx_status();
	}
	
	return (0);
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
		if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, (unsigned char *)DEVICE_ID_DRIVER_POS);

			counter_ping_interval = 0;
		}
	}

	counter_ping_interval++;
	counter_compare0++;
}