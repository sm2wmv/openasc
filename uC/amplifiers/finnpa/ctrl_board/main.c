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
#include <avr/eeprom.h>

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

//! Counter which keeps track of how long time ago it was since we toggled the amplifier stdby/operate
static unsigned int counter_ms_toggle_amp_stdby = 0;
//! Counter which keeps track of how long time ago it was since we toggled the mains of the amplifier
static unsigned int counter_ms_toggle_amp_mains = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
static unsigned int counter_event_timer = 0;

//! Ping message of the openASC device
static unsigned char ping_message[3];

static unsigned char main_flags = 0;

static unsigned int ad_curr_val[3] = {0,0,0};

main_struct_status main_status;
main_struct_settings main_settings;

unsigned char flag_start_tune;

unsigned char send_status_update = 0;

unsigned char flag_run_event = 0;

unsigned int counter_100us;

#ifdef DEBUG
  unsigned char print_pos = 0;
#endif

unsigned char flag_broadcast_amp_status = 0;
  
unsigned char curr_ptt_input_state = 0;
unsigned char prev_ptt_input_state = 0;
  
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
	else if (bus_message->cmd == BUS_CMD_AMPLIFIER_TOGGLE_MAINS_STATUS) {
    if (counter_ms_toggle_amp_mains > OPERATE_MAINS_LIMIT) {
      counter_ms_toggle_amp_mains = 0;
      
      //Check that PTT is not enabled
      if (ext_control_get_ptt_status() == 0) {
        if (main_status.amp_flags & (1<<AMP_STATUS_MAINS)) {
          ext_control_set_mains_off();
          main_status.amp_flags &= ~(1<<AMP_STATUS_MAINS);
          main_status.amp_flags &= ~(1<<AMP_STATUS_STDBY_OP);
          
          main_status.amp_op_status = AMP_OP_STATUS_OFF;
          
          if (main_status.parent_addr != 0) {
            send_amp_status();
            main_status.parent_addr = 0;
          }
          #ifdef DEBUG
            printf("BUS >> TURN MAINS OFF\n");
          #endif
        }
        else {
          main_status.parent_addr = bus_message->from_addr;
          ext_control_set_mains_on();
          main_status.amp_flags |= (1<<AMP_STATUS_MAINS);
          
          main_status.amp_flags &= ~(1<<AMP_STATUS_STDBY_OP);
          main_status.amp_op_status = AMP_OP_STATUS_STDBY;
          
          send_status_update = 1;
          #ifdef DEBUG
            printf("BUS >> TURN MAINS ON\n");
          #endif
        }
      }
    }
  }
  else if (bus_message->cmd == BUS_CMD_AMPLIFIER_TOGGLE_OPERATE_STBY_STATUS) {
    if (counter_ms_toggle_amp_stdby > OPERATE_STBY_OPERATE_LIMIT) {
      counter_ms_toggle_amp_stdby = 0;

      //Check that PTT is not enabled
      if (ext_control_get_ptt_status() == 0) {
        if (main_status.amp_flags & (1<<AMP_STATUS_MAINS)) {
          if (main_status.amp_flags & (1<<AMP_STATUS_STDBY_OP)) {
            main_status.amp_flags &= ~(1<<AMP_STATUS_STDBY_OP);
            
            ext_control_set_hv_off();
            send_status_update = 1;
            #ifdef DEBUG
              printf("BUS >> ENTERED STDBY MODE\n");
            #endif
          }
          else {
            main_status.amp_flags |= (1<<AMP_STATUS_STDBY_OP);
            
            ext_control_set_hv_on();
            send_status_update = 1;
            #ifdef DEBUG
              printf("BUS >> ENTERED READY MODE\n");
            #endif        
          }
        }
      }
    }
  }
  else if (bus_message->cmd == BUS_CMD_AMPLIFIER_TUNE) {
    //Check that PTT is not enabled
    if (ext_control_get_ptt_status() == 0) {
      if (main_status.mode == MODE_REMOTE) {
        if (main_status.amp_flags & (1<<AMP_STATUS_MAINS)) {
          main_status.curr_band = bus_message->data[0];
          main_status.curr_segment = bus_message->data[1];
          
          flag_start_tune = 1;
        }
      }
    }
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

void __inline__ send_amp_status(void) {
  if (main_status.parent_addr != 0) {
    unsigned char message[4];
    message[0] = main_status.amp_flags;
    message[1] = main_status.amp_op_status;
    message[2] = main_status.curr_band;
    message[3] = main_status.curr_segment;
    
    bus_add_tx_message(bus_get_address(), main_status.parent_addr , (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_AMPLIFIER_GET_STATUS, 4, message);
    
    #ifdef DEBUG
      printf("BUS >> SENT STATUS MSG\n");
    #endif
  }
}

void __inline__ broadcast_amp_status(void) {
  unsigned char message[4];
  message[0] = main_status.amp_flags;
  message[1] = main_status.amp_op_status;
  message[2] = main_status.curr_band;
  message[3] = main_status.curr_segment;
  
  bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR , 0, BUS_CMD_AMPLIFIER_GET_STATUS, 4, message);
  
  #ifdef DEBUG
    printf("BUS >> BROADCAST STATUS MSG\n");
  #endif
}

/*! \brief Send a ping message out on the bus */
void send_ping(void) {
	bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 3, ping_message);
  
  #ifdef DEBUG
    printf("BUS >> SENT PING MSG\n");
  #endif
}

void main_update_tune_sequence_status(unsigned char sequence_index) {
  main_status.tune_sequence_flags &= ~(1<<sequence_index);

  if (main_status.tune_sequence_flags == 0) {
    main_status.amp_op_status = AMP_OP_STATUS_READY;
    send_status_update = 1;
    
    #ifdef DEBUG
      printf("EVENT >> TUNE PROCESS DONE\n");
    #endif      
  }
}

void start_tune_amp(void) {
  //Check that PTT is not enabled
  if (ext_control_get_ptt_status() == 0) {
    if ((main_settings.tune_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] > MOTOR1_LIMIT_CW) && (main_settings.tune_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] < MOTOR1_LIMIT_CCW)) {// & (main_settings.load_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] > MOTOR2_LIMIT_CW) && (main_settings.load_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment] <MOTOR2_LIMIT_CCW)) {
      //TODO: Change to 1 on LOAD
      main_status.tune_sequence_flags = (1<<TUNE_SEQUENCE_TUNE_DONE) | (0<<TUNE_SEQUENCE_LOAD_DONE) | (1<<TUNE_SEQUENCE_RELAY_DONE);
      main_status.amp_op_status = AMP_OP_STATUS_TUNING;
      
      ext_control_activate_band_relay(main_status.curr_band);
      motor_control_goto(0,main_settings.tune_cap_pos[main_get_band_index(main_status.curr_band)][main_status.curr_segment]);
      
      send_status_update = 1;
      
      #ifdef DEBUG
        printf("EVENT >> TUNING\n");
      #endif
    }
    else {
      main_status.amp_op_status = AMP_OP_STATUS_ERROR;
      
      #ifdef DEBUG
        printf("ERROR >> TUNE FAILURE\n");
      #endif
      
      send_status_update = 1;
    }
  }
}

void event_handler_control_panel(unsigned int state) {
  unsigned int curr_state = ext_control_get_curr_state();
  
  #ifdef DEBUG
    printf("EVENT_HANDLER_CONTROL_CHANGED\n");
  #endif
    
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
        
        eeprom_write_block(&main_settings,(void *)1,sizeof(main_settings));
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
	
  init_timer_1();
  
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();

  event_queue_init();
  
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

  unsigned int ext_control_state = 0;

  eeprom_read_block(&main_settings,(void *)1,sizeof(main_settings));
  
  main_status.amp_op_status = AMP_OP_STATUS_OFF;
  main_status.amp_flags = 0;
  main_status.parent_addr = 0;
  main_status.curr_band = 0;
  main_status.curr_segment = 0;

  ext_control_init();
  
  PORTJ &= ~(1<<2);
  PORTJ &= ~(1<<3);
  PORTJ &= ~(1<<4);
  PORTJ &= ~(1<<5);
  PORTJ &= ~(1<<6);
  PORTG &= ~(1<<2);
  PORTC &= ~(1<<7);
  PORTC &= ~(1<<6);
  PORTC &= ~(1<<5);
  PORTC &= ~(1<<4);
  PORTC &= ~(1<<3);
  PORTC &= ~(1<<2);
    
  while(1) {
    if (bus_check_rx_status(&mess)) {
      bus_parse_message(&mess);
    }
    
    bus_check_tx_status();		
	
    if (flag_start_tune) {
      start_tune_amp();
      
      flag_start_tune = 0;
    }
    
    curr_ptt_input_state = ext_control_get_ptt_input_state();

    //The PTT input status has changed
    if (curr_ptt_input_state != prev_ptt_input_state) {
      if (curr_ptt_input_state) {
        if ((main_status.amp_flags & (1<<AMP_STATUS_MAINS)) && (main_status.amp_flags & (1<<AMP_STATUS_STDBY_OP)) && (main_status.amp_op_status == AMP_OP_STATUS_READY)) {
          ext_control_set_ptt_active();
        
          #ifdef DEBUG
            printf("PTT >> PTT ACTIVE\n");
          #endif  
        }
      }
      else {
        ext_control_set_ptt_deactive();
        
        #ifdef DEBUG
          printf("PTT >> PTT DEACTIVE\n");   
        #endif
      }
      
      prev_ptt_input_state = curr_ptt_input_state;
    }
    
    if (bus_is_master()) {
			if (counter_sync >= BUS_MASTER_SYNC_INTERVAL) {
				bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_SYNC, 1, (unsigned char *)DEF_NR_DEVICES);

				counter_sync = 0;
			}
		}

		if (flag_run_event) {
      event_run();
      flag_run_event = 0; 
    }
		
    #ifdef DEBUG
      if (print_pos == 1) {
        printf("CURR_POS: %i\n",a2dConvert10bit(1)); 
        printf("CURR_BAND: %i\n",main_status.curr_band);
        
        print_pos = 0;
      }
    #endif
    
    if ((counter_ms % 5) == 0) {
      ext_control_state = ext_control_poll_inputs();
      
      if (main_status.mode == MODE_LOCAL)
        main_status.curr_band = ext_control_get_current_band_pos();
    
      if (ext_control_state != 0) {
        event_handler_control_panel(ext_control_state);
      }
    }
		
		if (bus_allowed_to_send()) {
      if (send_status_update) {
        send_amp_status();
        
        send_status_update = 0;
      }
      
      if (flag_broadcast_amp_status) {
        broadcast_amp_status();
        
        flag_broadcast_amp_status = 0;
      }
      
		//Check if a ping message should be sent out on the bus
			if (counter_ping_interval >= BUS_DEVICE_STATUS_MESSAGE_INTERVAL) {
				send_ping();
        
				counter_ping_interval = 0;
			}
		}
	}
}

unsigned char step_period = 2;
unsigned char phase = 7;
unsigned char dir = 1;
unsigned char out = 0;
/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0A) {
	//If this device should act as master it should send out a SYNC command
	//and also the number of devices (for the time slots) that are active on the bus
  if ((main_flags & (1<<FLAG_DEVICE_STARTED)) && (bus_is_master())) {
		counter_sync++;
	}

	if (event_in_queue()) {
		if (counter_event_timer >= (event_queue_get()).time_target)
      flag_run_event = 1;
	}
	
	if ((counter_ms % 1500) == 0) {
    flag_broadcast_amp_status = 1;
  }
	
  #ifdef DEBUG
    if ((counter_ms % 1000) == 0) {
      print_pos = 1;
    }
  #endif
	
	counter_ping_interval++;
	counter_ms++;
	counter_event_timer++;

  counter_ms_toggle_amp_stdby++;
  counter_ms_toggle_amp_mains++;
  
  if (counter_ms_toggle_amp_stdby > 65000)
    counter_ms_toggle_amp_stdby = OPERATE_STBY_OPERATE_LIMIT+1;
  
  if (counter_ms_toggle_amp_mains > 65000)
    counter_ms_toggle_amp_mains = OPERATE_MAINS_LIMIT+1;  
  
	//If the value equals the half of it's range then
	//we remove that amount from all target times in the event
	//queue and set the counter to 0 again
	if (counter_event_timer >= 32767) {
		event_queue_wrap(32767);
		
		counter_event_timer = 0;
	}
	
	bus_ping_tick();
}

/*!Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
}



ISR(SIG_OUTPUT_COMPARE1A) {
  motor_control_process();
  motor_control_tick();

  //Clear the counters
  TCNT1L = 0;
  TCNT1H = 0;
}

/*!Output overflow 1 interrupt */
ISR(SIG_OVERFLOW1) {
}