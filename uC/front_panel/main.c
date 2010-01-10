/*! \file main.c \brief Main file of the front panel
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-30
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
#include "../delay.h"
#include "../i2c.h"
#include "../global.h"
#include "../event_queue.h"
#include "interrupt_handler.h"
#include "eeprom.h"
#include "ds1307.h"
#include "computer_interface.h"
#include "antenna_ctrl.h"
#include "eeprom_m24.h"
#include "radio_interface.h"
#include "menu.h"
#include "rotary_encoder.h"
#include "event_handler.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"
#include "../internal_comm.h"
#include "../internal_comm_commands.h"

//#define DEBUG_WMV_BUS 1

struct_setting settings;
struct_uc_com uc_com;

UC_MESSAGE uc_new_message;

/* Different counters */
unsigned char radio_rx_data_counter = 0;
unsigned int counter_compare0 = 0;
unsigned int counter_sync = 32000;
unsigned char counter_poll_buttons = 0;
unsigned char counter_poll_ext_devices = 0;
unsigned int counter_screensaver_timeout = 0;
unsigned int counter_ping_interval = 0;
unsigned int counter_ms = 0;
unsigned char counter_poll_rotary_encoder = 0;
unsigned int counter_poll_radio = 0;
unsigned int counter_last_pulse_event=0;

//After the counter reaches half of it's limit we remove that number from it by
//calling the function event_queue_wrap()
unsigned int counter_event_timer = 0;

//! Different flags, description is found in main.h
unsigned int main_flags = 0;

//! Clear the screensaver timer
void clear_screensaver_timer(void) {
	counter_screensaver_timeout = 0;
}

/*! Add a message to the event queue which will be run at the correct time
 *  \param func A function pointer to the function we want to run
 *  \param offset the time in ms when we want our function to be run
 */
void event_add_message(void (*func), unsigned int offset, unsigned char id) {
	EVENT_MESSAGE event;
	
	if (event_in_queue() == 0)
		counter_event_timer = 0;

	event.func = func;
	event.time_target = counter_event_timer + offset;
	event.id = id;
	
	event_queue_add(event);
}

unsigned char ext_key_get_assignment(unsigned char index) {
	return(settings.ext_key_assignments[index]);
}

void ext_key_set_assignment(unsigned char index, unsigned char func) {
	settings.ext_key_assignments[index] = func;
}

/*! Run the first function in the event queue
 */
void event_run(void) {
	if (event_in_queue()) {
		EVENT_MESSAGE event = event_queue_get();
		
		//Run the function in the event queue
		event.func();
	
		//When the function has been run we drop the message
		event_queue_drop();
	}
}

void main_update_display(void) {
	main_flags |= (1<<FLAG_UPDATE_DISPLAY);
}

void shutdown_device(void) {
	internal_comm_add_tx_message(INT_COMM_PULL_THE_PLUG,0,0);
}

/* This is a temporary thing */
void init_dummy_values(void) {
	antenna_ctrl_set_antenna_text("6/6/6el",0);
	antenna_ctrl_set_antenna_text("5el",1);
	antenna_ctrl_set_antenna_text("5el @JA",2);
	antenna_ctrl_set_antenna_text("4el @EU",3);
	
	antenna_ctrl_set_direction(310,0);
	antenna_ctrl_set_direction(185,1);
	
	antenna_ctrl_set_flags((1<<ANTENNA_EXIST_FLAG) | (1<<ANTENNA_ROTATOR_FLAG),0);
	antenna_ctrl_set_flags((1<<ANTENNA_EXIST_FLAG) | (1<<ANTENNA_ROTATOR_FLAG),1);
	antenna_ctrl_set_flags((1<<ANTENNA_EXIST_FLAG),2);	
	antenna_ctrl_set_flags((1<<ANTENNA_EXIST_FLAG),3);
	
	antenna_ctrl_set_comb_allowed(0X097E);
	
	unsigned char temp[12] = {3,4,5,OUTPUT_ADDR_DELIMITER,0x02,10,12,13,OUTPUT_ADDR_DELIMITER,0x03};
	
	antenna_ctrl_set_output_comb(temp,1,10);
}

void set_tx_ant_leds(void) {
	for (unsigned char i=0;i<4;i++)
		if (status.selected_ant & (1<<i))
			led_set_tx_ant(i+1,LED_STATE_ON);
	else
		led_set_tx_ant(i+1,LED_STATE_OFF);
}

void set_knob_function(unsigned char function) {
	if (function == KNOB_FUNCTION_NONE) {
		if (radio_interface_get_interface() == RADIO_INTERFACE_MANUAL)
			status.knob_function = KNOB_FUNCTION_SELECT_BAND;
		else
			status.knob_function = KNOB_FUNCTION_NONE;
	}
	else
		status.knob_function = function;
}

void load_settings(void) {
		/* Read the EEPROM table */
	eeprom_read_table();
	
	//Load the radio settings from the eeprom
	radio_interface_load_eeprom();
	
	//Load antenna details for the current band
	//antenna_ctrl_ant_read_eeprom(radio_get_current_band());
	antenna_ctrl_ant_read_eeprom(BAND_20M);
	
	//Load the antenna RX data from the eeprom
	antenna_ctrl_rx_ant_read_eeprom();
	//init_dummy_values();

	//Load the runtime settings from the EEPROM
	eeprom_get_runtime_settings(&runtime_settings);
	
	settings.network_device_is_master = 0;
	
	//Load the settings struct
	eeprom_get_settings_structure(&settings);
}

int main(void){
	delay_ms(250);
	cli();
	
	MCUSR = 0;
	wdt_disable();
	
	/* Initialize various hardware resources */
	init_ports();
	

	//BEGIN TEMPORARY
	//runtime_settings.lcd_backlight_value = 70;
	//runtime_settings.amplifier_ptt_output = 1;
	//runtime_settings.radio_ptt_output = 1;
	//END TEMPORORARY

	//Init the status structure
	status.buttons_last_state = 0;
	status.buttons_current_state = 0;
	status.ext_devices_current_state = ih_poll_ext_devices();
	status.ext_devices_last_state = status.ext_devices_current_state;
	status.selected_rx_antenna = -1;
	status.new_band = BAND_UNDEFINED;
	
	//TEMPORARY
	//sequencer_init_dummy();
	runtime_settings.rotator_small_step = 1;
	runtime_settings.rotator_big_step = 5;
	status.rotator_step_resolution = runtime_settings.rotator_big_step;
	
	//Init the computer communication
	init_usart_computer();
	//Init the communication between the uCs
	init_usart();

	//Init the backlight PWM
	init_backlight();
	i2c_init();
	//Init the realtime clock
	ds1307_init();
	
	//Init the communication routines between the computer and the openASC box
	computer_interface_init();

	//Init the communication between the motherboard uC and the front panel uC
	//The first argument will be called each time a message should be parsed
	//The second argument will be called each time a character should be sent
	internal_comm_init((void *)event_internal_comm_parse_message, (void *)usart0_transmit);

	//Initialize the 128x64 display
	glcd_init();
	
	//Print the startup picture
	glcd_print_picture();
	
	//Load all settings from the EEPROM	
	load_settings();
	
	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */	
	delay_ms(7 * settings.network_address);
	
	//This must be done in this order for it to work properly!
	/* Read the external address of the device */
	bus_set_address(settings.network_address);
	
	bus_init();
	
	if (settings.network_device_is_master == 1)
		bus_set_is_master(1,settings.network_device_count);
	else
		bus_set_is_master(0,0);	
	
	if (bus_is_master()) {
		tx_queue_dropall();
	}

	init_timer_0();
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();
		
	sei();

	//DEBUG data
//	printf("openASC started\n");

	//TEMPORARY
	//radio_interface_set_interface(RADIO_INTERFACE_MANUAL);
	
	//eeprom_save_runtime_settings(&runtime_settings);

	//Set all leds on at startup so it's possible to see everything works
	led_set_all(LED_STATE_ON);
	delay_ms(250);
	delay_ms(250);
	led_set_all(LED_STATE_OFF);
	
	//TEMP
	//PORTC |= (1<<6);
	
	//Check if the computer interface should have full control of the box, setup mode
	if (PIND & (1<<5)) {
		computer_interface_deactivate_setup();
	}
	else {
		PORTC |= (1<<7);
		computer_interface_activate_setup();
	}
	
	//Initialize the menu system
	menu_init();

	status.current_display = CURRENT_DISPLAY_ANTENNA_INFO;
	status.current_display_level = DISPLAY_LEVEL_BAND;
	
	display_set_backlight(runtime_settings.lcd_backlight_value);
	
	led_set_ptt(LED_STATE_GREEN);
	
	//At startup the knob function should be MANUAL BAND CHANGE
	if (radio_interface_get_interface() == RADIO_INTERFACE_MANUAL)
		set_knob_function(KNOB_FUNCTION_SELECT_BAND);
	
	while(1) {
		if (tx_queue_is_empty())
			PORTC |= (1<<7);
		else
			PORTC &= ~(1<<7);

		computer_interface_send_data();
		computer_interface_parse_data();
		
		if (!rx_queue_is_empty())
			event_bus_parse_message();
		
		if (!tx_queue_is_empty())
			bus_check_tx_status();
		
		if (radio_interface_get_interface() != RADIO_INTERFACE_MANUAL) {
			if (radio_get_current_band() != status.selected_band) {
				status.new_band = radio_get_current_band();
				band_ctrl_change_band(status.new_band);
			}
		}
		
		//Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
		internal_comm_poll_rx_queue();
		
		//Poll the TX queue in the internal comm to see if we have any new messages to be SENT
		internal_comm_poll_tx_queue();
		
		//DISABLE_TIMER0_INT();
		
		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}
	
		//Poll the buttons
		if (main_flags & (1<<FLAG_POLL_BUTTONS)) {
			event_poll_buttons();
			main_flags &= ~(1<<FLAG_POLL_BUTTONS);
		}
		
		//Poll the external devices, such as footswitch, radio sense etc
		if (main_flags & (1<<FLAG_POLL_EXT_DEVICES)) {
			event_poll_ext_device();
			main_flags &= ~(1<<FLAG_POLL_EXT_DEVICES);
		}
		
		//ENABLE_TIMER0_INT();
	
		if (main_flags & (1<<FLAG_PULSE_SENSOR_UP)) {
			counter_last_pulse_event=0;
			
			event_pulse_sensor_up();
			main_flags &= ~(1<<FLAG_PULSE_SENSOR_UP);
		}

		if (main_flags & (1<<FLAG_PULSE_SENSOR_DOWN)) {
			counter_last_pulse_event=0;
			
			event_pulse_sensor_down();
			main_flags &= ~(1<<FLAG_PULSE_SENSOR_DOWN);
		}

		//Update the display
		if (main_flags & (1<<FLAG_UPDATE_DISPLAY)) {
			event_update_display();
			main_flags &= ~(1<<FLAG_UPDATE_DISPLAY);
		}
	}
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0A) {
	//If this device should act as master it should send out a SYNC command
	//and also the number of devices (for the time slots) that are active on the bus
	if (bus_is_master()) {
		if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
			unsigned char temp = bus_get_device_count();
			
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, &temp);
			counter_sync = 0;
		}
		
		counter_sync++;
	}

	if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
		if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
			bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, (unsigned char *)DEVICE_ID_MAINBOX);

			counter_ping_interval = 0;
		}
	}
	
	/*if (radio_rx_data_counter >= RADIO_RX_DATA_TIMEOUT) {
//		radio_communicaton_timeout();
		radio_rx_data_counter = 0;
	}*/

	if (counter_poll_rotary_encoder >= INTERVAL_POLL_ROTARY_ENCODER) {
		int val = rotary_encoder_poll();
		
		if (val != 0) {
			if (val == 1)
				main_flags |= (1<<FLAG_PULSE_SENSOR_UP);
			else if (val == -1)
				main_flags |= (1<<FLAG_PULSE_SENSOR_DOWN);
		}
		
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
			event_run();
			//main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
	}
	
	if (!display_screensaver_mode()) {
		if (counter_screensaver_timeout == DISPLAY_SCREENSAVER_TIMEOUT) {
			//event_add_message((void*)display_update_screensaver, 0);
			//display_set_contrast(DISPLAY_SCREENSAVER_DEF_CONTRAST);
			
			counter_screensaver_timeout = 0;
		}
	}
	
	//This will blink the NEW BAND LED, if the new band is not the same as the old one
	if ((counter_ms % 250) == 0) {
		if (status.new_band != status.selected_band) {
			if (main_flags & (1<<FLAG_LAST_BAND_BLINK)) {
				led_set_band_none();
				//led_set_band(status.selected_band);
				led_set_band(status.new_band);
				
				main_flags &= ~(1<<FLAG_LAST_BAND_BLINK);
			}
			else {
				led_set_band_none();
				led_set_band(status.selected_band);
				
				main_flags |= (1<<FLAG_LAST_BAND_BLINK);
			}
		}
		else if ((main_flags & (1<<FLAG_LAST_BAND_BLINK)) == 0) {
			led_set_band_none();
			led_set_band(status.selected_band);
				
			main_flags |= (1<<FLAG_LAST_BAND_BLINK);
		}
	}
	
	//Update every 500 ms
	if ((counter_ms % 500) == 0) {
		unsigned char flags = 0;
		
		for (int i=0;i<4;i++) {
			flags |= antenna_ctrl_get_rotator_flags(i);
		}
		
		if (flags & (1<<FLAG_NO_ROTATION))
			led_set_rotation_active(LED_STATE_OFF);
		else
			led_set_rotation_active(LED_STATE_ON);
		
		if (status.function_status & (1<<FUNC_STATUS_SELECT_ANT_ROTATE)) {
			if (main_flags & (1<<FLAG_LAST_ANTENNA_BLINK)) {
				main_flags &= ~(1<<FLAG_LAST_ANTENNA_BLINK);
				
				for (unsigned char i=0;i<4;i++)
					if (antenna_ctrl_get_rotatable() & (1<<i))
						led_set_tx_ant(i+1,LED_STATE_ON);
			}
			else {
				for (unsigned char i=0;i<4;i++)
					if (antenna_ctrl_get_rotatable() & (1<<i))
						led_set_tx_ant(i+1,LED_STATE_OFF);
				
				main_flags |= (1<<FLAG_LAST_ANTENNA_BLINK);
			}
		}
	}
	
	if (counter_last_pulse_event > PULSE_SENSOR_RX_ANT_CHANGE_LIMIT) {
		if (main_flags & (1<<FLAG_CHANGE_RX_ANT)) {
			antenna_ctrl_change_rx_ant(status.selected_rx_antenna);
			
			main_flags &= ~(1<<FLAG_CHANGE_RX_ANT);
		}
	}
	
	if (counter_poll_radio >= radio_interface_get_poll_interval()*10) {
		radio_poll_status();
		
		counter_poll_radio = 0;
	}
	
	//If the value equals the half of it's range then
	//we remove that amount from all target times in the event
	//queue and set the counter to 0 again
	if (counter_event_timer >= 32767) {
		event_queue_wrap(32767);
		
		counter_event_timer = 0;
	}
	
	counter_last_pulse_event++;
	counter_poll_rotary_encoder++;
	counter_poll_buttons++;
	counter_poll_ext_devices++;	
	counter_screensaver_timeout++;
	counter_ping_interval++;
	counter_ms++;
	counter_event_timer++;
	
	counter_poll_radio++;	
}

/*!Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
}