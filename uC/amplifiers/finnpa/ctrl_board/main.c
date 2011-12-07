/*! \file 4cx1500b/ctrl_board/main.c
 *  \brief Main file of the 4cx1500b amp control board
 *  \ingroup ctrl_board_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/main.c" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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
#include <avr/wdt.h>

#include "main.h"
#include "board.h"
#include "usart.h"
#include "init.h"
#include "motor_control.h"
#include "a2d.h"

#include "../../../delay.h"
#include "../../../global.h"
#include "../../../event_queue.h"

/* Include the bus headers */
#include "../../../wmv_bus/bus.h"
#include "../../../wmv_bus/bus_ping.h"
#include "../../../wmv_bus/bus_rx_queue.h"
#include "../../../wmv_bus/bus_tx_queue.h"
#include "../../../wmv_bus/bus_commands.h"

//#define ERROR_DEBUG 1

//! Counter which counts up each time a compare0 interrupt has occured
static unsigned int counter_compare0 = 0;
//! Counter which is used to keep track of when we last received a sync message from the bus
static unsigned int counter_sync = 32000;
//! Counter which keeps track of when we should send out a ping to the communication bus
static unsigned int counter_ping_interval = 0;
//! Counter which counts up each millisecond
static unsigned int counter_ms = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
static unsigned int counter_event_timer = 0;

static unsigned char counter_ad_conversion = 0;

//! Ping message of the openASC device
static unsigned char ping_message[3];

static unsigned char main_flags = 0;

static unsigned char ad_curr_ch = 0;
static unsigned char ad_conv_started = 0;
static unsigned int ad_curr_val[8] = {0,0,0,0,0,0,0,0};

void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();

	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message.from_addr);
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message.from_addr, bus_message.data[0]);
	else if (bus_message.cmd == BUS_CMD_PING) {
		if (bus_message.length > 1)
			bus_ping_new_stamp(bus_message.from_addr, bus_message.data[0], bus_message.length-1, (unsigned char *)(bus_message.data+1));
		else
			bus_ping_new_stamp(bus_message.from_addr, bus_message.data[0], 0, 0);
	}
	else {
	}
	
	//Drop the message from the RX queue
	rx_queue_drop();
}

/*! Add a message to the event queue which will be run at the correct time
 *  \param func A function pointer to the function we want to run
 *  \param offset the time in ms when we want our function to be run
 *  \param id Which type of event this is */
void event_add_message(void (*func), unsigned int offset, unsigned char id) {
	EVENT_MESSAGE event;
	
	if (event_in_queue() == 0)
		counter_event_timer = 0;

	event.func = func;
	event.time_target = counter_event_timer + offset;
	event.id = id;
	
	event_queue_add(event);
}

/*! \brief Run the first function in the event queue */
void event_run(void) {
	if (event_in_queue()) {
		EVENT_MESSAGE event = event_queue_get();
		
		//Run the function in the event queue
		event.func();
	
		//When the function has been run we drop the message
		event_queue_drop();
	}
}

/*! \brief Send a ping message out on the bus */
void send_ping(void) {
	bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 3, ping_message);
}

/*! Main function of the front panel */
int main(void){
	cli();
	
	MCUSR = 0;
	wdt_disable();
	
	delay_ms(250);
	
	/* Initialize various hardware resources */
	init_ports();

	//Initialize the A/D converter
	a2dInit();
	
	delay_ms(250);

	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */	
  delay_ms(7 * BUS_ADDR);
	
  //This must be done in this order for it to work properly!
	/* Read the external address of the device */
	bus_set_address(BUS_ADDR);
	
	bus_init();
	
  if (BUS_ADDR == 0x01) {
    bus_set_is_master(1,DEF_NR_DEVICES);
	}
	else {
		bus_set_is_master(0,0);	
	}
	
	if (bus_is_master()) {
		tx_queue_dropall();
	}
	
	init_timer_0();
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();

	sei();
	
  ping_message[0] = DEVICE_ID_AMP_CTRL_BOARD;

  main_flags |= (1<<FLAG_DEVICE_STARTED);
  
	//Load the current position of the pots - IMPORTANT!!
	for (unsigned char i=0;i<2;i++)
		ad_curr_val[i] = a2dConvert10bit(i);
	
	while(1) {
		if (!rx_queue_is_empty())
			bus_parse_message();
		
		if (!tx_queue_is_empty())
			bus_check_tx_status();
		
		if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, (unsigned char *)DEF_NR_DEVICES);

				counter_sync = 0;
			}
		}

		if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				send_ping();
				
				counter_ping_interval = 0;
			}
		}
    
    motor_control_process();
/*		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}*/
		
		//Read the values of the A/D
		if (ad_conv_started == 0) {
			if (counter_ad_conversion >= AD_CONV_INTERVAL) {
				a2dSetChannel(ad_curr_ch);
				a2dStartConvert();
				ad_conv_started = 1;
			}
		}
		else {
			if (a2dIsComplete() == 1) {
				ad_curr_val[ad_curr_ch] = a2dGet10bitVal();
				ad_conv_started = 0;
				
				if (ad_curr_ch < 2)
					ad_curr_ch++;
				else {
					ad_curr_ch = 0;
					counter_ad_conversion = 0;
				}
			}
		}
	}
}

unsigned int get_ad_curr_val(unsigned char ch) {
	return(ad_curr_val[ch]);
}

unsigned char step_period = 3;
unsigned char phase = 7;
unsigned char dir = 0;

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0A) {
	//If this device should act as master it should send out a SYNC command
	//and also the number of devices (for the time slots) that are active on the bus
  if ((main_flags & (1<<FLAG_DEVICE_STARTED)) && (bus_is_master())) {
		counter_sync++;
	}

	if (event_in_queue()) {
		if (counter_event_timer >= (event_queue_get()).time_target)
      event_run();
      //main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
	}
	
	if ((counter_ms % 5000) == 0) {
		if (dir == 1)
			dir = 0;
		else
			dir = 1;
	}
	
	if ((counter_ms % step_period) == 0) {
		if (dir == 0) {
			phase--;
			motor_control_set_phase(0,phase);
			
			motor_control_step_motor1();
			
			if (phase == 0)
				phase = 4;
		}
		else if (dir == 1) {
			motor_control_set_phase(0,phase);
			phase++;
			
			motor_control_step_motor1();
			
			if (phase > 3)
				phase = 0;	
		}
	}
	
	counter_ad_conversion++;
		
	counter_ping_interval++;
	counter_ms++;
	counter_event_timer++;

	//If the value equals the half of it's range then
	//we remove that amount from all target times in the event
	//queue and set the counter to 0 again
	if (counter_event_timer >= 32767) {
		event_queue_wrap(32767);
		
		counter_event_timer = 0;
	}
	
	bus_ping_tick();
  
  motor_control_tick();
}

/*!Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
}