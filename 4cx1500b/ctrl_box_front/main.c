/*! \file 4cx1500b/ctrl_box_front/main.c
 *  \brief Main file of the front panel
 *  \ingroup amp_front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/main.c" \endcode
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
#include "display.h"
#include "glcd.h"
#include "ks0108.h"
#include "led_control.h"
#include "../../delay.h"
#include "../../i2c.h"
#include "../../global.h"
#include "../../event_queue.h"
#include "ds1307.h"
#include "rotary_encoder.h"
#include "event_handler.h"

/* Include the bus headers */
#include "../../wmv_bus/bus.h"
#include "../../wmv_bus/bus_ping.h"
#include "../../wmv_bus/bus_rx_queue.h"
#include "../../wmv_bus/bus_tx_queue.h"
#include "../../wmv_bus/bus_commands.h"
#include "../../internal_comm.h"
#include "../../internal_comm_commands.h"

//#define ERROR_DEBUG 1

//! Used to disable/enable all polling of radios, buttons etc, used for shutdown
unsigned char device_online = 0;

//! Counter which counts up each time a compare0 interrupt has occured
unsigned int counter_compare0 = 0;
//! Counter which is used to keep track of when we last received a sync message from the bus
unsigned int counter_sync = 32000;
//! Counter which keeps track of the screensaver timeout
unsigned int counter_screensaver_timeout = 0;
//! Counter which keeps track of when we should send out a ping to the communication bus
unsigned int counter_ping_interval = 0;
//! Counter which counts up each millisecond
unsigned int counter_ms = 0;

unsigned int counter_poll_rotary_encoder = 0;
unsigned int counter_poll_buttons = 0;
unsigned int counter_poll_ext_devices = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
unsigned int counter_event_timer = 0;

//! The number of devices on the bus
unsigned char device_count = 0;

//! Different flags, description is found in main.h
unsigned int main_flags = 0;

//! Ping message of the openASC device
unsigned char ping_message[3];

//! Clear the screensaver timer
void clear_screensaver_timer(void) {
	counter_screensaver_timeout = 0;
	
	if (display_screensaver_mode()) {
		display_disable_screensaver();
		display_set_backlight(runtime_settings.lcd_backlight_value);
		
		main_update_display();
	}
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

/*! \brief Sets the flag that the display should be updated */
void main_update_display(void) {
	main_flags |= (1<<FLAG_UPDATE_DISPLAY);
}

/*! \brief Send a message to the motherboard that the openASC box should be shut off. Will deactivate the power supply relay. */
void shutdown_device(void) {
	led_set_error(LED_STATE_ON);
	
	internal_comm_add_tx_message(INT_COMM_PULL_THE_PLUG,0,0);
}

/*! \brief Send a ping message out on the bus */
void send_ping(void) {
	bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 3, ping_message);
}

void main_set_device_online(unsigned char val) {
  device_online = val; 
}

unsigned char main_get_ptt_status(void) {
  return(status.ptt_state);
}

/*! Main function of the front panel */
int main(void){
	cli();
	
	MCUSR = 0;
	wdt_disable();
	
	delay_ms(250);
	
	/* Initialize various hardware resources */
	init_ports();
	
  status.ptt_state = 0;
  
	delay_ms(250);
	
	//Initialize the 128x64 display
	glcd_init();
	glcd_clear();
	
	i2c_init();
	
	//Init the backlight PWM
	init_backlight();
	
	display_set_backlight(runtime_settings.lcd_backlight_value);
		
	//Init the communication between the uCs
	init_usart();

	//Init the communication between the motherboard uC and the front panel uC
	//The first argument will be called each time a message should be parsed
	//The second argument will be called each time a character should be sent
	internal_comm_init((void *)event_internal_comm_parse_message, (void *)usart0_transmit);

	//Init the realtime clock
	ds1307_init();
	
	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */	
	delay_ms(7 * settings.network_address);
	
	device_count = 3;
	
	//This must be done in this order for it to work properly!
	/* Read the external address of the device */
	bus_set_address(settings.network_address);
	
	bus_init();

	if (settings.network_device_is_master == 1) {
		bus_set_is_master(1,settings.network_device_count);
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

	//Print the startup picture
	glcd_print_picture();
	
	//Set all leds on at startup so it's possible to see everything works
	led_set_all(LED_STATE_ON);
	delay_ms(250);
	led_set_all(LED_STATE_OFF);
	
  led_set_ptt(LED_STATE_PTT_INHIBIT);

	sei();

  ping_message[0] = DEVICE_ID_AMP_CTRL_BOX;

	main_set_device_online(1);
	
	while(1) {
		if (!rx_queue_is_empty())
			event_bus_parse_message();
		
		if (!tx_queue_is_empty())
			bus_check_tx_status();
		
		if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, &device_count);

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
		
		//Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
		internal_comm_poll_rx_queue();
		
		//Poll the TX queue in the internal comm to see if we have any new messages to be SENT
		internal_comm_poll_tx_queue();

		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
      
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}
    
    display_update();
  }
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0A) {
	//If this device should act as master it should send out a SYNC command
	//and also the number of devices (for the time slots) that are active on the bus
	if ((device_online == 1) && (bus_is_master())) {
		counter_sync++;
	}

	if (counter_poll_rotary_encoder >= INTERVAL_POLL_ROTARY_ENCODER) {
		main_flags |= (1<<FLAG_POLL_PULSE_SENSOR);
		
		counter_poll_rotary_encoder = 0;
	}

	if (counter_poll_buttons == INTERVAL_POLL_BUTTONS) {
		main_flags |= (1<<FLAG_POLL_BUTTONS);
	
		counter_poll_buttons = 0;
	}
	
	if (counter_poll_ext_devices == INTERVAL_POLL_EXT_DEVICES) {
		main_flags |= (1<<FLAG_POLL_EXT_DEVICES);
		counter_poll_ext_devices = 0;
	}
	
	if (event_in_queue()) {
		if (counter_event_timer >= (event_queue_get()).time_target)
			main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
	}
	
	if (!display_screensaver_mode()) {
		if (counter_screensaver_timeout >= DISPLAY_SCREENSAVER_TIMEOUT) {
			display_enable_screensaver();

			event_add_message((void*)display_update_screensaver, 0,0);
			display_set_backlight(DISPLAY_SCREENSAVER_DEF_CONTRAST);
			
			counter_screensaver_timeout = 0;
		}
	}
	
	counter_poll_rotary_encoder++;
	counter_poll_buttons++;
	counter_poll_ext_devices++;	
	
	if ((counter_ms % 1000) == 0)
		counter_screensaver_timeout++;
	
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
	
	internal_comm_1ms_timer();
	
  display_1ms_tick();
  
	bus_ping_tick();
}

/*!Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
}