/*! \file rotator_unit/main.c \brief Main file of the rotator board
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-04-22
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

// BUS_BASE_ADDR can be found and be set in the makefile, this can be used
// if the current range (16 bits) is not enough.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "main.h"
#include "init.h"
#include "board.h"
#include "ext_control.h"
#include "rotator_ctrl.h"
#include "a2d.h"
#include "../event_queue.h"
#include "../delay.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

#define MAX_ASCII_CMD_ARGS  5

unsigned char device_id=0;

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
//! Counter to keep track of the time elapsed since the last sync message was sent
unsigned int counter_sync = 0;
//! Counter to keep track of when to send a ping out on the bus
unsigned int counter_ping_interval = 0;
//! Counter to keep track of how often we send out status information of rotation
unsigned int rotator_status_counter = 0;
//! Counter for the rotation delay
unsigned int rotation_delay_counter;
//! Counter for the rotation break delay
unsigned int rotation_break_delay_counter;

//After the counter reaches half of it's limit we remove that number from it by
//calling the function event_queue_wrap()
unsigned int counter_event_timer = 0;

unsigned char main_flags = 0;

unsigned char timer_flags = 0;

unsigned int counter_ad_poll = 0;

void rotator_set_no_rotation(void) {
  if (((main_flags & (1<<FLAG_ROTATION_CW)) == 0) && ((main_flags & (1<<FLAG_ROTATION_CW)) == 0))
    main_flags |= (1<<FLAG_NO_ROTATION);
}

static void send_ascii_data(unsigned char to_addr, const char *fmt, ...)
{
  char str[41];
  
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  if (len >= sizeof(str)-1) {
    strcpy(str + sizeof(str) - 6, "...\r\n");
    len = sizeof(str)-1;
  }
  
  char *ptr = str;
  while (len > 0) {
    unsigned char len_to_send = len < 15 ? len : 15;
    bus_add_tx_message(bus_get_address(),
                      to_addr,
                      (1<<BUS_MESSAGE_FLAGS_NEED_ACK),
                      BUS_CMD_ASCII_DATA,
                      len_to_send,
                      (unsigned char *)ptr
                      );
    len -= len_to_send;
    ptr += len_to_send;
  }
}


static void parse_ascii_cmd(BUS_MESSAGE *bus_message)
{
  char data[16];
  memcpy(data, bus_message->data, bus_message->length);
  data[bus_message->length] = '\0';
  
    // Get the command and its arguments
  unsigned char argc = 0;
  char *argv[MAX_ASCII_CMD_ARGS];
  argv[0] = NULL;
  unsigned char pos = 0;
  for (pos=0; pos < bus_message->length; ++pos) {
    if (argv[argc] == NULL) {
      if (data[pos] != ' ') {
        argv[argc] = (char*)(data + pos);
      }
    }
    else {
      if (data[pos] == ' ') {
        data[pos] = '\0';
        if (argc >= MAX_ASCII_CMD_ARGS-1) {
          break;
        }
        ++argc;
        argv[argc] = NULL;
      }
    }
  }
  if (argv[argc] != NULL) {
    ++argc;
  }
  
  send_ascii_data(bus_message->from_addr, "\r\n");
  
  if (argc > 0) {
    if (strcmp(argv[0], "help") == 0) {
      send_ascii_data(bus_message->from_addr,
                      "No help available\r\n");
    }
    else if (strcmp(argv[0], "cwl") == 0) {
      if (argc < 2) {
        send_ascii_data(bus_message->from_addr,"Too few arguments\r\n");
        send_ascii_data(bus_message->from_addr,"Usage: cwl <degree>\r\n");
      }
      else {
        rotator_settings.rotation_start_angle = atoi(argv[1]);
        
        send_ascii_data(bus_message->from_addr,"CWL set\r\n");
      }
    }
    else if (strcmp(argv[0], "ccwl") == 0) {
      if (argc < 2) {
        send_ascii_data(bus_message->from_addr,"Too few arguments\r\n");
        send_ascii_data(bus_message->from_addr,"Usage: ccwl <degree>\r\n");
      }
      else {
        rotator_settings.rotation_stop_angle = atoi(argv[1]);
        
        send_ascii_data(bus_message->from_addr,"CCWL set\r\n");
      }
    }
    else if (strcmp(argv[0], "brkdelay") == 0) {
      if (argc < 2) {
        send_ascii_data(bus_message->from_addr,"Too few arguments\r\n");
        send_ascii_data(bus_message->from_addr,"Usage: brkdelay <ms/100>\r\n");
      }
      else {
        rotator_settings.rotation_break_delay = atoi(argv[1]) ;
        send_ascii_data(bus_message->from_addr,"Rotator break delay set\r\n");
      }
    }
    else if (strcmp(argv[0], "rotdelay") == 0) {
      if (argc < 2) {
        send_ascii_data(bus_message->from_addr,"Too few arguments\r\n");
        send_ascii_data(bus_message->from_addr,"Usage: rotdelay <seconds>\r\n");
      }
      else {
        rotator_settings.rotation_delay = atoi(argv[1]) ;
        send_ascii_data(bus_message->from_addr,"Rotation delay set\r\n");
      }
    }
    else if (strcmp(argv[0], "save") == 0) {
      //This will cause a bus resend, how to fix it?
      eeprom_write_block(&rotator_settings, (const void *)1, sizeof(rotator_settings));
      send_ascii_data(bus_message->from_addr,"Saved settings\r\n");
    }
    else {
      send_ascii_data(bus_message->from_addr, "Huh?\r\n");
    }
  }

  send_ascii_data(bus_message->from_addr, "%d> ", bus_get_address());
}

/*! \brief Parse a message and exectute the proper commands
* This function is used to parse a message that was receieved on the bus that is located
* in the RX queue. */
void bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();
	unsigned char temp[6];
	
	switch(bus_message.cmd) {
		case BUS_CMD_ACK:
			bus_message_acked(bus_message.from_addr);
			break;
		case BUS_CMD_NACK:
			bus_message_nacked(bus_message.from_addr, bus_message.data[0]);
			break;
		case BUS_CMD_PING:
			break;
		case BUS_CMD_ROTATOR_SET_ANGLE:
			//1st byte = sub_addr (0x00 in our case)
			//2nd byte = new direction (high bytes)
			//3rd byte = new direction (low bytes)
			if (rotator_settings.rotator_mode == ROTATOR_MODE_HARDWIRED)
				rotator_rotate_to((bus_message.data[1]<<8) + bus_message.data[2]);
			break;
		case BUS_CMD_ROTATOR_GET_STATUS:
			temp[0] = 0x00;
			temp[1] = ((rotator_status.curr_heading >> 8) & 0xFF);
			temp[2] = (rotator_status.curr_heading & 0xFF);
			temp[3] = ((rotator_status.target_heading >> 8) & 0xFF);
			temp[4] = (rotator_status.target_heading & 0xFF);
			temp[5] = main_flags;
			bus_add_tx_message(bus_get_address(), bus_message.from_addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_ROTATOR_GET_STATUS, 6, temp);
			break;
		case BUS_CMD_ROTATOR_ROTATE_CW:
      if ((rotator_settings.rotator_mode == ROTATOR_MODE_HARDWIRED) && ((main_flags & (1<<FLAG_NO_ROTATION)) != 0)) {
				event_add_message(rotator_release_break,0,EVENT_QUEUE_RELEASE_BREAK_ID);
				event_add_message(rotator_rotate_cw,rotator_settings.rotation_break_delay,EVENT_QUEUE_ROTATE_CW_ID);
        
        main_flags &= ~(1<<FLAG_NO_ROTATION);
        main_flags |= (1<<FLAG_ROTATION_CW); 
			}
			break;
		case BUS_CMD_ROTATOR_ROTATE_CCW:
      if ((rotator_settings.rotator_mode == ROTATOR_MODE_HARDWIRED) && ((main_flags & (1<<FLAG_NO_ROTATION)) != 0)) {
				event_add_message(rotator_release_break,0,EVENT_QUEUE_RELEASE_BREAK_ID);
				event_add_message(rotator_rotate_ccw,rotator_settings.rotation_break_delay,EVENT_QUEUE_ROTATE_CW_ID);
        
        main_flags &= ~(1<<FLAG_NO_ROTATION);
        main_flags |= (1<<FLAG_ROTATION_CCW);
			}	
      		
			break;
		case BUS_CMD_ROTATOR_STOP:
			if (rotator_settings.rotator_mode == ROTATOR_MODE_HARDWIRED) {
        if (((main_flags & (1<<FLAG_ROTATION_CCW)) != 0) || ((main_flags & (1<<FLAG_ROTATION_CW)) != 0)) {
          rotator_stop();
          
				  event_add_message(rotator_activate_break,rotator_settings.rotation_break_delay,EVENT_QUEUE_ACTIVATE_BREAK_ID);
          event_add_message(rotator_set_no_rotation, rotator_settings.rotation_delay, EVENT_QUEUE_ROTATION_ALLOWED);
            
          main_flags &= ~(1<<FLAG_ROTATION_CW);
          main_flags &= ~(1<<FLAG_ROTATION_CCW);
        }
      }
			break;
    case BUS_CMD_ASCII_DATA:
      parse_ascii_cmd(&bus_message);
      break;
		default:
			break;
	}
	
	//Drop the message
	rx_queue_drop();
}

/*! Add a message to the event queue which will be run at the correct time
 *  \param func A function pointer to the function we want to run
 *  \param offset the time in 10th of a second when we want our function to be run
 *  \param id An identifier of the message, can be used to for example drop messages of a certain type
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

/*! \brief Read the external DIP-switch.
* This function is used to read the external offset address on the rotator unit
*	\return The address of the external DIP-switch */
unsigned char read_ext_addr(void) {
	return(~(PINE>>2) & 0x0F);
}

void main_convert_ad_vals(void) {
	rotator_status.curr_heading = (int)((double)rotator_status.curr_heading_ad_val * 0.5625f);
}

void init_dummy_values(void) {
	rotator_settings.heading_input = HEADING_INPUT_POT1;
	rotator_settings.rotator_mode = ROTATOR_MODE_HARDWIRED;
	rotator_settings.cw_output = (1<<ROTATION_OUTPUT_RELAY1);
	rotator_settings.ccw_output = (1<<ROTATION_OUTPUT_RELAY2);
	rotator_settings.break_output = (1<<ROTATION_OUTPUT_RELAY3);
	
	rotator_settings.rotation_delay = 10;	//Rotation delay is 10 seconds before any action after a rotation
	rotator_settings.rotation_start_angle = 20;
  rotator_settings.rotation_stop_angle = 500;
	rotator_settings.rotation_min = 100;
	rotator_settings.rotation_max = 900;
	rotator_settings.rotation_break_delay = 10;
	
	main_flags = 0;	
	main_flags |= (1<<FLAG_NO_ROTATION) | (1<<FLAG_ROTATION_ALLOWED);
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

int main(void)
{
	cli();

	delay_ms(100);

	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV32);
	a2dSetReference(ADC_REFERENCE_AREF);
		
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
	
	
	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */
	for (unsigned char i=0;i<bus_get_address();i++)
		delay_ms(90);


	if ((BUS_BASE_ADDR+read_ext_addr()) == 0x01)
		bus_set_is_master(1,DEF_NR_DEVICES);
	else
		bus_set_is_master(0,0);


	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();	
	
	//Timer with interrupts each ms
	init_timer_0();	
	
	event_queue_init();
	
	sei();
	
	//init_dummy_values();
  
  eeprom_read_block(&rotator_settings, (const void *)1, sizeof(rotator_settings));
	
	/*if (rotator_settings.rotator_mode == ROTATOR_MODE_HARDWIRED) {
		event_add_message(rotator_release_break,0,EVENT_QUEUE_RELEASE_BREAK_ID);
		event_add_message(rotator_rotate_ccw,rotator_settings.rotation_break_delay*10,EVENT_QUEUE_ROTATE_CW_ID);
	}*/
	
	unsigned char device_count = bus_get_device_count();
	
	unsigned char send_status[7];
	
	device_id = DEVICE_ID_ROTATOR_UNIT;
	
	while(1) {	
		if (!rx_queue_is_empty()) {
			bus_parse_message();
		}

		if (!tx_queue_is_empty())
			bus_check_tx_status();
		
		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
			
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}
		
		if ((timer_flags & (1<<FLAG_SEND_STATUS)) != 0) {
			//We only update if the beaming heading has changed with > 1 degree from the last update
			if (rotator_status.curr_heading != rotator_status.last_heading) {
				send_status[0] = DEVICE_ID_ROTATOR_UNIT;
				send_status[1] = 0;
				send_status[2] = ((rotator_status.curr_heading >> 8) & 0xFF);
				send_status[3] = (rotator_status.curr_heading & 0xFF);
				send_status[4] = ((rotator_status.target_heading >> 8) & 0xFF);
				send_status[5] = (rotator_status.target_heading & 0xFF);
				send_status[6] = main_flags;
	
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_ROTATOR_STATUS_UPDATE, 7, send_status);
			
				rotator_status.last_heading = rotator_status.curr_heading;
			}
			
			timer_flags &= ~(1<<FLAG_SEND_STATUS);
		}
		
		if ((timer_flags & (1<<FLAG_POLL_AD)) != 0) {
			if (rotator_settings.heading_input == HEADING_INPUT_POT1)
				rotator_status.curr_heading_ad_val = a2dConvert10bit(ADC_CH_ADC0);
			else if (rotator_settings.heading_input == HEADING_INPUT_POT2)
				rotator_status.curr_heading_ad_val = a2dConvert10bit(ADC_CH_ADC1);
	
			main_convert_ad_vals();
			
			timer_flags &= ~(1<<FLAG_POLL_AD);
		}

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
				//Check if the device is a POS or NEG driver module
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 1, &device_id);

				counter_ping_interval = 0;
			}
		}
	}
	
	return (0);
}

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	if (counter_ad_poll >= AD_VAL_POLL_INTERVAL) {
		timer_flags |= (1<<FLAG_POLL_AD);
		
		counter_ad_poll = 0;
	}

	//This limits the update interval of the rotator status when the rotator is turning to max
	//ROTATOR_STATUS_UPDATE_INTERVAL in milliseconds, so if ROTATOR_STATUS_UPDATE_INTERVAL is set
	//to 1000 it means that maximum update interval of beam heading is one second.
	//ROTATOR_STATUS_UPDATE_INTERVAL can be found in main.h
	if (rotator_status_counter >= ROTATOR_STATUS_UPDATE_INTERVAL) {
		timer_flags |= (1<<FLAG_SEND_STATUS);
		
		rotator_status_counter = 0;
	}
	
	if (event_in_queue()) {
		if (counter_event_timer >= (event_queue_get()).time_target)
      main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
	}
	
	//If the value equals the half of it's range then
	//we remove that amount from all target times in the event
	//queue and set the counter to 0 again
	if (counter_event_timer >= 32767) {
		event_queue_wrap(32767);
		
		counter_event_timer = 0;
	}
	
	if ((counter_compare0 % 100) == 0)
		counter_event_timer++;
	
	rotation_delay_counter++;
	rotation_break_delay_counter++;
	rotator_status_counter++;
	counter_ping_interval++;
	counter_compare0++;
	counter_sync++;
	counter_ad_poll++;
}

ISR(SIG_USART0_RECV) {
	//Read the character from the buffer
	unsigned char data = UDR0;
	
	if (rotator_settings.rotator_mode != ROTATOR_MODE_HARDWIRED) {
		//TODO: Write RX code for rotators
		
		//Just to get rid of warning
		data = data;
	}
}

ISR(SIG_USART0_DATA) {
	
}