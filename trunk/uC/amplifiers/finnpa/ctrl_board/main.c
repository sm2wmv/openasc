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
#include "ext_control.h"

#include "../../../delay.h"
#include "../../../global.h"
#include "../../../event_queue.h"

/* Include the bus headers */
#include "../../../wmv_bus/bus.h"
#include "../../../wmv_bus/bus_ping.h"
#include "../../../wmv_bus/bus_commands.h"

//! Counter which is used to keep track of when we last received a sync message from the bus
static unsigned int counter_sync = 32000;
//! Counter which keeps track of when we should send out a ping to the communication bus
static unsigned int counter_ping_interval = 0;
//! Counter which counts up each millisecond
static unsigned int counter_ms = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
static unsigned int counter_event_timer = 0;

//! Ping message of the openASC device
static unsigned char ping_message[3];

static unsigned char main_flags = 0;

static unsigned int ad_curr_val[3] = {0,0,0};

main_struct_status main_status;
main_struct_settings main_settings;

#ifdef DEBUG
  unsigned char print_pos = 0;
#endif

void bus_parse_message(BUS_MESSAGE *bus_message) {

	if (bus_message->cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message->from_addr);
	else if (bus_message->cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message->from_addr, bus_message->data[0]);
	else if (bus_message->cmd == BUS_CMD_PING) {
		if (bus_message->length > 1)
			bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], bus_message->length-1, (unsigned char *)(bus_message->data+1));
		else
			bus_ping_new_stamp(bus_message->from_addr, bus_message->data[0], 0, 0);
	}
	
	else {
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

unsigned char main_get_band_index(unsigned char band) {
  unsigned char retval = 0;
  
  if (band == BAND_160M)
    retval = 0;
  else if (band == BAND_80M)
    retval = 1;
 else if (band == BAND_40M)
    retval = 2;
  else if (band == BAND_20M)
    retval = 3;
  else if (band == BAND_15M)
    retval = 4;
  else if (band == BAND_10M)
    retval = 5;
  
  return(retval);
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

void send_amp_status(void) {
  if (main_status.parent_addr != 0) {
    unsigned char message[4];
    message[0] = main_status.amp_flags;
    message[1] = main_status.amp_op_status;
    message[2] = main_status.curr_band;
    message[3] = main_status.curr_segment;
    
    bus_add_tx_message(bus_get_address(), main_status.parent_addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_AMPLIFIER_GET_STATUS, 4, message);
    
    #ifdef DEBUG
      printf("BUS >> SENT STATUS MSG\n");
    #endif
  }
}

/*! \brief Send a ping message out on the bus */
void send_ping(void) {
	bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 3, ping_message);
  
  #ifdef DEBUG
    printf("BUS >> SENT PING MSG\n");
  #endif
}

void event_handler_control_panel(unsigned int state) {
  unsigned int curr_state = ext_control_get_curr_state();
  
  #ifdef DEBUG
    printf("EVENT_HANDLER_CONTROL_CHANGED\n");
  #endif
    
  if (state & (1<<EXT_CONTROL_PTT_INPUT_BIT)) {
    if (curr_state  & (1<<EXT_CONTROL_PTT_INPUT_BIT)) {
      if (main_status.amp_op_status == AMP_OP_STATUS_READY) {
        ext_control_set_ptt_active();
        main_status.ptt_status = 0;
        
        #ifdef DEBUG
          printf("EVENT >> PTT ACTIVE\n");
        #endif
      }
    }
    else {
      ext_control_set_ptt_deactive();
      main_status.ptt_status = 0;
      
        #ifdef DEBUG
          printf("EVENT >> PTT DEACTIVE\n");
        #endif
    }
  }

  if (state & (1<<EXT_CONTROL_MODE_SW_BIT)) {
    if (curr_state & (1<<EXT_CONTROL_MODE_SW_BIT)) {
      main_status.mode = 1;
      
      #ifdef DEBUG
        printf("EVENT >> MODE: LOCAL\n");
      #endif        
    }
    else {
      main_status.mode = 0;
      
      #ifdef DEBUG
        printf("EVENT >> MODE: REMOTE\n");
      #endif
    }
  }
  
  if (state & (1<<EXT_CONTROL_DISPLAY_UP_BIT)) {
    if (curr_state & (1<<EXT_CONTROL_DISPLAY_UP_BIT)) {
      #ifdef DEBUG
        printf("EVENT >> DISPLAY UP PRESSED\n");
      #endif
    }
  }
  
  if (state & (1<<EXT_CONTROL_DISPLAY_DOWN_BIT)) {
    if (curr_state & (1<<EXT_CONTROL_DISPLAY_DOWN_BIT)) {
      #ifdef DEBUG
        printf("EVENT >> DISPLAY DOWN PRESSED\n");
      #endif
    }
  }

  if (main_status.mode == MODE_LOCAL) {
    if (state & (1<<EXT_CONTROL_SAVE_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_SAVE_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> SAVE PRESSED\n");
        #endif
         
        main_settings.tune_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] = motor_control_get_curr_pos(0);
        //main_settings.load_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] = motor_control_get_curr_pos(1);
      }
    }  

    if (state & (1<<EXT_CONTROL_RECALL_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_RECALL_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> RECALL PRESSED\n");
        #endif
          
        motor_control_goto(0,main_settings.tune_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment]);
        //motor_control_goto(1,main_settings.load_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment]);
      }
    }
    
    if (state & (1<<EXT_CONTROL_TUNE_UP_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_TUNE_UP_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> TUNE UP POSITION\n");
        #endif
          
        motor_control_stepper_turn_ccw(0);
      }
      else {
        #ifdef DEBUG
          printf("EVENT >> TUNE UP POSITION -> MID\n");
        #endif
          
        motor_control_stepper_off(0);
      }
    }
    
    if (state & (1<<EXT_CONTROL_TUNE_DOWN_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_TUNE_DOWN_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> TUNE DOWN POSITION\n");
        #endif
          
        motor_control_stepper_turn_cw(0);
      }
      else {
        #ifdef DEBUG
          printf("EVENT >> TUNE DOWN POSITION -> MID\n");
        #endif
          
        motor_control_stepper_off(0);
      }
    }
    
    if (state & (1<<EXT_CONTROL_SEGMENT_HIGH_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_SEGMENT_HIGH_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> SEGMENT HIGH POSITION\n");
        #endif
          
        main_status.curr_segment = BAND_SEGMENT_HIGH;
      }
      else {
        #ifdef DEBUG
          printf("EVENT >> SEGMENT MID POSITION\n");
        #endif
          
        main_status.curr_segment = BAND_SEGMENT_MID;
      }
    }
    
    if (state & (1<<EXT_CONTROL_SEGMENT_LOW_BIT)) {
      if (curr_state & (1<<EXT_CONTROL_SEGMENT_LOW_BIT)) {
        #ifdef DEBUG
          printf("EVENT >> SEGMENT LOW POSITION\n");
        #endif
        
        main_status.curr_segment = BAND_SEGMENT_LOW;
      }
      else {
        #ifdef DEBUG
          printf("EVENT >> SEGMENT MID POSITION\n");
        #endif
          
        main_status.curr_segment = BAND_SEGMENT_MID;
      }
    }    
  }
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
	
	init_usart_computer();
	
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
	
	init_timer_0();
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();

	sei();
	
  ping_message[0] = DEVICE_ID_AMP_CTRL_BOARD;

  main_flags |= (1<<FLAG_DEVICE_STARTED);

	a2dSetPrescaler(ADC_PRESCALE_DIV32);
	a2dSetReference(ADC_REFERENCE_AVCC);
  
	//Load the current position of the pots - IMPORTANT!!
	for (unsigned char i=0;i<2;i++)
		ad_curr_val[i] = a2dConvert10bit(i);
	
  #ifdef DEBUG
    printf("\n\r\n\rFinnPA control board started\n\r");
  #endif
	
  unsigned int motor_pos = 100;
  
  BUS_MESSAGE mess;

//  motor_control_goto(0,100);

  unsigned int ext_control_state = 0;

  //TEMPORARY
  main_status.parent_addr = 7;
  main_status.amp_op_status = AMP_OP_STATUS_READY;
  
  while(1) {
    if (bus_check_rx_status(&mess)) {
      bus_parse_message(&mess);
    }
    
    bus_check_tx_status();		
		
    if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, (unsigned char *)DEF_NR_DEVICES);

				counter_sync = 0;
			}
		}

		if (main_status.mode == MODE_LOCAL)
      main_status.curr_band = ext_control_get_current_band_pos();

		
    #ifdef DEBUG
      if (print_pos == 1) {
        printf("CURR_POS: %i\n",a2dConvert10bit(1)); 
        printf("CURR_BAND: %i\n",ext_control_get_current_band_pos());
        
        print_pos = 0;
      }
    #endif
    
    ext_control_state = ext_control_poll_inputs();
    
    if (ext_control_state != 0) {
      event_handler_control_panel(ext_control_state);
    }
		
		if (bus_allowed_to_send()) {
		//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				send_ping();
        
        send_amp_status();
        
				counter_ping_interval = 0;
			}
		}

/*		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}*/
	}
}

unsigned char step_period = 2;
unsigned char phase = 7;
unsigned char dir = 1;

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
	
  #ifdef DEBUG
    if ((counter_ms % 1000) == 0) {
      print_pos = 1;
    }
  #endif
	
	motor_control_process();
	
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