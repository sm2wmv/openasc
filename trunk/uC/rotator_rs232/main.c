//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
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
#include <avr/wdt.h>

#include "main.h"
#include "init.h"
#include "../i2c.h"
#include "../delay.h"
#include "usart.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_ping.h"
#include "../wmv_bus/bus_commands.h"

static unsigned char rx_buffer[5];

unsigned char ping_msg[2];

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of the time elapsed since the last sync message was sent
unsigned int counter_sync=0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval=0;

unsigned char char_count = 0;
unsigned int curr_angle = 0;

void rotator_rs232_get_status(void) {
  usart0_transmit(0x57);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x1F);
  usart0_transmit(0x20);
}

void rotator_rs232_send_status(void) {
  unsigned char data[7];

  data[1] = 0;                    /* Sub address */
  data[2] = (curr_angle >> 8);    /* Hi current heading */
  data[3] = (curr_angle & 0xff);  /* Lo current heading */
  data[4] = (curr_angle >> 8);    /* Hi target heading */
  data[5] = (curr_angle & 0xff);  /* Lo target heading */
  data[6] = 0;                    /* Status flags */

  bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0,
                      BUS_CMD_ROTATOR_STATUS_UPDATE, sizeof(data), data);
}

void rotator_rs232_send_stop(void) {
  usart0_transmit(0x57);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x0F);
  usart0_transmit(0x20);
}

void rotator_rs232_set_heading(unsigned int target) {
  char temp[5];
  
  //Convert the target heading into characters
  itoa(target,temp,10);
    
  usart0_transmit(0x57);
  usart0_transmit(temp[0]);
  usart0_transmit(temp[1]);
  usart0_transmit(temp[2]);
  usart0_transmit(0x30);
  usart0_transmit(0x01);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x00);
  usart0_transmit(0x2F);
  usart0_transmit(0x20);
}

/*! \brief Parse a message and exectute the proper commands
* This function is used to parse a message that was receieved on the bus that is located
* in the RX queue. */
void rotator_rs232_bus_parse_message(BUS_MESSAGE *bus_message) {
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
    case BUS_CMD_ROTATOR_SET_ANGLE: {
      uint16_t angle = bus_message->data[1] << 8;
      angle |= bus_message->data[2];
      delay_ms(100);
      rotator_rs232_set_heading(angle+360);
      delay_ms(100);
      break;
    }
    case BUS_CMD_ROTATOR_ROTATE_CW: {
      //Does not work with the Alfa spid
      break;
    }
    case BUS_CMD_ROTATOR_ROTATE_CCW: {
      //Does not work with the alfa spid
      break;
    }
    case BUS_CMD_ROTATOR_STOP: {
      delay_ms(100);
      rotator_rs232_send_stop();
      delay_ms(100);
      break;
    }
    case BUS_CMD_SYNC:
			break;
		default:
			
			break;
	}
}

/*! \brief Read the external DIP-switch.
* This function is used to read the external offset address on the rotator_unit
*	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
  return(~(PINE>>2) & 0x0F);
}

/*! Main function of the driver unit */
int main(void)
{
	cli();
	
	delay_ms(250);

	init_ports();

	delay_ms(250);

  /* Read the external address of the device */
	bus_set_address(BUS_BASE_ADDR+read_ext_addr());

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

	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();
	
  usart0_init(382); //Init the USART at 1200 baud
  
	wdt_enable(WDTO_1S);
	
	sei();

	unsigned char device_count = bus_get_device_count();
  ping_msg[0] = DEVICE_ID_ROTATOR_UNIT;

  BUS_MESSAGE mess;
  
	curr_angle = 210;
	
	
  while(1) {
    if (bus_check_rx_status(&mess)) {
      rotator_rs232_bus_parse_message(&mess);
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
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, ping_msg);

				counter_ping_interval = 0;
			}
		}
		
		if ((counter_compare0 % 1000) == 0) {
			rotator_rs232_get_status();
      rotator_rs232_send_status();
    }
		
		wdt_reset();
	}

	return (0);
}

/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_sync++;
	counter_ping_interval++;
	counter_compare0++;
	
  bus_check_ack_list();
	
  if ((counter_compare0 % 100) == 0)
    bus_ping_tick();
}

ISR(SIG_USART0_DATA) {
}

/*! Interrupt which is called when a byte has been received */
ISR(SIG_USART0_RECV) {
  rx_buffer[char_count++] = UDR0;
  
  if (char_count == 5) {
    if (rx_buffer[0] == 0x57) {
      curr_angle = rx_buffer[1] * 100 + rx_buffer[2] * 10 + rx_buffer[3];
      
			while (curr_angle >= 360)
        curr_angle -= 360;
    }
    
    char_count = 0;
  }
}