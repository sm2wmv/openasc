/*! \file front_panel/main.c
 *  \brief Main file of the front panel
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/main.c" \endcode
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
#include "powermeter.h"
#include "errors.h"
#include "display_handler.h"
#include "remote_control.h"
#include "ethernet.h"
#include "../remote_commands.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_ping.h"
#include "../wmv_bus/bus_commands.h"
#include "../internal_comm.h"
#include "../internal_comm_commands.h"

//#define ERROR_DEBUG 1

//! Settings struct
struct_setting settings;

//! Used to disable/enable all polling of radios, buttons etc, used for shutdown
unsigned char device_online = 0;

//! Ping list, so we dont need to make space in the memory for it each time
//static bus_struct_ping_status main_ping_list;

//! The encoder value
static int encoder_val = 0;

//! Counter which is used to keep track of when we last received a sync message from the bus
static unsigned int counter_sync = 32000;
//! Counter which keeps track of when we should poll the buttons
static unsigned char counter_poll_buttons = 0;
//! Counter which keeps track of when we should poll the external inputs
static unsigned char counter_poll_ext_devices = 0;
//! Counter which keeps track of the screensaver timeout
static unsigned int counter_screensaver_timeout = 0;
//! Counter which keeps track of when we should send out a ping to the communication bus
static unsigned int counter_ping_interval = 0;
//! Counter which counts up each millisecond
static unsigned int counter_ms = 0;
//! Counter which keeps track when we should poll the rotary encoder
static unsigned char counter_poll_rotary_encoder = 0;
//! Counter which keeps track of when we should poll the radio
static unsigned int counter_poll_radio = 0;
//! Counter which keeps track of when the last pulse event did occur. This is used to sense if we should change rx antennas 
static unsigned int counter_last_pulse_event=0;
static unsigned int counter_powermeter_update_bargraph = 0;
static unsigned int counter_powermeter_update_text = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
static unsigned int counter_event_timer = 0;

//! Counter which we use to keep track of when to check the critical cmd list
static unsigned char counter_critical_cmd_check = 0;

//! The number of devices on the bus
static unsigned char device_count = 0;

//! Different flags, description is found in main.h
unsigned int main_flags = 0;

//! Ping message of the openASC device
static unsigned char ping_message[5];

//! Variable to check if the device has actually gone through all init steps
static unsigned char device_started = 0;

//! Clear the screensaver timer
void clear_screensaver_timer(void) {
	counter_screensaver_timeout = 0;
	
	if (display_handler_screensaver_mode()) {
		display_handler_disable_screensaver();
		display_handler_set_backlight(runtime_settings.lcd_backlight_value);

    display_handler_repaint();
  }
}

unsigned char main_get_ethernet_local_mode(void) {
  return(runtime_settings.ethernet_local_mode);
}

struct_setting* main_get_settings_ptr(void) {
  return((struct_setting *)&settings);
}

void main_update_mainbox_list(unsigned char from_addr) {
  unsigned char* band_info_ptr = bus_ping_get_mainbox_adresses();
  bus_struct_ping_status* ping_status_ptr;
  
  for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++) {
    if (band_info_ptr[i] == 0) {
      //If we reach the end of the ping list we know that our own box is not 
      //in the ping list, so we insert this information so it gets sent to the ethernet unit
      status.mainbox_status[i][0] = bus_get_address();
      status.mainbox_status[i][1] = status.selected_band;
	  status.mainbox_status[i][2] = status.selected_ant;
	  status.mainbox_status[i][3] = status.selected_rx_antenna;
	  
  	  //Attach the PTT status of the box
	  if (radio_get_ptt_status() != 0)
		status.mainbox_status[i][3] |= 0x80;
			
	  break;
    }
    else if (band_info_ptr[i] == from_addr) {
      ping_status_ptr = bus_ping_get_ping_data(band_info_ptr[i]-1);
      
      status.mainbox_status[i][0] = from_addr;
      status.mainbox_status[i][1] = ping_status_ptr->data[1];
	  status.mainbox_status[i][2] = ping_status_ptr->data[2];
	  status.mainbox_status[i][3] = ping_status_ptr->data[3];
    }
  }
  
  if (ethernet_is_active()) {// && (main_get_ethernet_local_mode() == 0)) {
    unsigned char data[MAINBOX_DEVICE_COUNT*4];
    
    for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++) {
      data[i] = status.mainbox_status[i][0];
      data[i+MAINBOX_DEVICE_COUNT*1] = status.mainbox_status[i][1];
	  data[i+MAINBOX_DEVICE_COUNT*2] = status.mainbox_status[i][2];
	  data[i+MAINBOX_DEVICE_COUNT*3] = status.mainbox_status[i][3];
	}
    
    ethernet_send_data(0, REMOTE_COMMAND_BAND_INFO, MAINBOX_DEVICE_COUNT*4, data);
  }
}

void forceHardReset(void) { 
  cli(); // disable interrupts 
  wdt_enable(WDTO_15MS); // enable watchdog 
  while(1); // wait for watchdog to reset processor 
}

//This function is just used at startup
void main_enable_device(void) {
  device_online = 1;
}

void main_set_band_change_mode(unsigned char mode) {
	runtime_settings.band_change_mode = mode;
}

unsigned char main_get_amp_func_flags(void) {
  return(settings.amp_func_conf);
}

unsigned char main_get_amp_sub_addr(void) {
  return(settings.amp_sub_addr);
}

unsigned char main_get_amp_addr(void) {
  return(settings.amp_addr);  
}
unsigned char main_get_amp_ctrl_enabled(void) {
  return(settings.amp_ctrl_enabled);
}

unsigned char main_get_current_band(void) {
	return(status.selected_band);
}

unsigned char __inline__ main_get_status_field_index(void) {
  return(settings.status_field_index);
}

void main_set_new_band(unsigned char band) {
  status.new_band = band;
  
  status.current_band_portion = BAND_LOW;
	band_ctrl_change_band(band);
					
	send_ping();
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

/*! Get the power meter address
 *  \param band Which band we wish to get the address of
 *  \return the address */
unsigned char main_get_powermeter_address(unsigned char band) {
	return(settings.powermeter_address[band]);
}

/*! Get the key assignment index
 *  \param index The index of which task we wish to check
 *  \return The current task index, can be found in event_handler.h */
unsigned char ext_key_get_assignment(unsigned char index) {
  return(settings.ext_key_assignments[index]);
}

/*! Get information if a band change is OK to do
 *  \return 1 if the band change is OK, 0 if not */
unsigned char main_band_change_ok(unsigned char new_band) { 
  unsigned char* band_info_ptr = bus_ping_get_mainbox_adresses();
  bus_struct_ping_status* ping_status_ptr;
  
  #ifdef DEBUG_BAND_IN_USE
    printf("DEBUG -> BAND IN USE\r\n");
    printf("--------------------\r\n");
  
    for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++) {
      //We have reached the end of the list
      if (band_info_ptr[i] == 0)
        break;
      
      ping_status_ptr = bus_ping_get_ping_data(band_info_ptr[i]-1);
            
      printf("Box %i [0x%02X]: Band -> %i\r\n",i+1,ping_status_ptr->addr,ping_status_ptr->data[1]);
    }  
  #endif
  
    if (new_band != 0) {
      //Load the new band, so we can read out data from the settings
      band_ctrl_load_band(new_band);
      
      for (unsigned char i=0;i<MAINBOX_DEVICE_COUNT;i++) {
        //We have reached the end of the list
        if (band_info_ptr[i] == 0)
          break;
        				
        ping_status_ptr = bus_ping_get_ping_data(band_info_ptr[i]-1);
        
        if ((ping_status_ptr->addr != bus_get_address()) && (ping_status_ptr->data[1] != 0)) {
					#ifdef DEBUG_BAND_IN_USE
						printf("\r\n");
						printf("COMP BANDLOCK\r\n");
						printf("BANDLOCK: %i\r\n",band_ctrl_get_lock_conf());
						printf("PING_STATUS1: %i\r\n",(unsigned int)(1<<(ping_status_ptr->data[1]-1)));
						printf("ADDR: 0x%02X\r\n",ping_status_ptr->addr);
					#endif

				
          //Compare against the band_lock conf to see if we are allowed onto this band, or if some other
          //openASC box is using any of the bands in the band lock variable
					if (((unsigned int)(1<<(ping_status_ptr->data[1]-1)) & band_ctrl_get_lock_conf()) != 0)
						return(0);
        }
      }
    }

  //The band isn't in use, OK to change
  return(1);
}

/*! Get the key assignment index of the aux buttons
 *  \param buttonIndex Which button we wish to get the function index from
 *  \return The current task index, can be found in event_handler.h */
unsigned char main_get_aux_button(unsigned char buttonIndex) {
	if (buttonIndex == 1)
		return(settings.aux1_button_func);
	else if (buttonIndex == 2)
		return(settings.aux2_button_func);
	
	return(0);
}

/*! Set the key assignment task
 *  \param index The index of which task we wish to set
 *  \param func The function we wish to assign to the assignment index */
void ext_key_set_assignment(unsigned char index, unsigned char func) {
	settings.ext_key_assignments[index] = func;
}

/*! \brief Run the first function in the event queue */
void event_run(void) {
	if (event_in_queue()) {
		EVENT_MESSAGE event = event_queue_get();
    
    event_queue_drop();
    
		//Run the function in the event queue
		event.func();
	}
}

void main_process_lock(unsigned char lock_status) {
  if (lock_status == 1) {
    sequencer_lock_all();
    
    main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND);
    
    if (status.alarm_output == 0) {
      PORTB |= (1<<ALARM_OUTPUT_BIT);
      status.alarm_output = 1;
    }    
    
    led_set_ptt(LED_STATE_PTT_INHIBIT); 
  }
}

/*! \brief This function will set the device_online variable 
 *  \param state What the state of the device_online variable should be */
void main_set_device_online(unsigned char state) {
	device_online = state;
}

/*! \brief Send a message to the motherboard that the openASC box should be shut off. Will deactivate the power supply relay. */
void shutdown_device(void) {
	led_set_error(LED_STATE_ON);
	
	internal_comm_add_tx_message(INT_COMM_PULL_THE_PLUG,0,0);
}

/*! \brief Set the TX antenna leds according to the status of status.selected_ant */
void set_tx_ant_leds(void) {
	for (unsigned char i=0;i<4;i++)
		if (status.selected_ant & (1<<i))
			led_set_tx_ant(i+1,LED_STATE_ON);
	else
		led_set_tx_ant(i+1,LED_STATE_OFF);
}

/*! \brief Set the rotary knob function 
 *  \param function Which type of action should occur when the knob is turned */
void set_knob_function(unsigned char function) {
	if (function == KNOB_FUNCTION_AUTO) {
		if ((status.knob_prev_function == KNOB_FUNCTION_NONE) || (status.knob_prev_function == status.knob_function)) {
			if (runtime_settings.band_change_mode == BAND_CHANGE_MODE_MANUAL)
				status.knob_function = KNOB_FUNCTION_SELECT_BAND;
		}
		else if (status.knob_prev_function != KNOB_FUNCTION_SET_SUBMENU)  //We don't want to change sub menu options by misstake
			status.knob_function = status.knob_prev_function;
    else
      status.knob_function = KNOB_FUNCTION_NONE;
	} 
	else {
		status.knob_prev_function = status.knob_function;
		status.knob_function = function;
	}
}

/*! \brief Save runtime settings etc to the EEPROM */
void main_save_settings(void) {
	eeprom_save_runtime_settings(&runtime_settings);
}

/*! \brief Load all settings from the EEPROM */
void load_settings(void) {
		/* Read the EEPROM table */
	eeprom_read_table();
	
	//Load the radio settings from the eeprom
	radio_interface_load_eeprom();
	
	//Load the antenna RX data from the eeprom
	antenna_ctrl_rx_ant_read_eeprom();

	//Load the runtime settings from the EEPROM
	eeprom_get_runtime_settings(&runtime_settings);
	
	settings.network_device_is_master = 0;
	
	//Load the settings struct
	eeprom_get_settings_structure(&settings);
	
	//Load all the sequencer settings
	sequencer_load_eeprom();
  
  status.txrx_mode = 0;
}

void main_update_critical_list(void) {
  status.curr_critical_cmd_state = event_check_critical_cmd_list();

  //Checks the message queues if we are allowed to PTT or not
  if (status.curr_critical_cmd_state != status.last_critical_cmd_state) {
    status.last_critical_cmd_state = status.curr_critical_cmd_state;

    if (status.curr_critical_cmd_state == 0) {
      event_add_message((void *)main_update_ptt_status, CRITICAL_CMD_CHANGE_TAIL_TIME, EVENT_TYPE_CRITICAL_CMD_UPDATE);
  }
  else
    main_update_ptt_status();
  }
}

/*! \brief Function which updates the status of the PTT
 *  This function will check various sources if it is for example OK to transmit or not. This function
 *  also updates the color of the PTT led. It does also set the main_set_inhibit_state() status which
 *  is used at various places to make the sequencing etc safe. */
void main_update_ptt_status(void) {
	unsigned char state = 0;
    
	//Check that there is an antenna on the current band
	if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_EXIST_FLAG))
		state = 0;
	if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_EXIST_FLAG))
		state = 0;
	if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_EXIST_FLAG))
		state = 0;
	if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_EXIST_FLAG))
		state = 0;
	
	//Check that an antenna is selected
	if (status.selected_ant != 0)
		state = 0;
	else
		state = 1;
	
	//Check that a band is selected
	if (status.selected_band == BAND_UNDEFINED)
		state = 1;
	
  if (radio_get_ptt_status() != 0)
    state = 2;
  
  if (status.curr_critical_cmd_state != 0)
    state = 3;

  if (main_get_amp_ctrl_enabled()) {
    if ((status.amp_flags & (1<<AMP_STATUS_MAINS)) && (status.amp_flags & (1<<AMP_STATUS_OPR_STBY))) {
      if (status.amp_op_status != AMP_OP_STATUS_READY)
        state = 3;
      
      if (status.amp_band != status.selected_band)
        state = 3;
    }
  }

	if (error_handler_is_ptt_locked() == 1) {
		state = 3;
	}
  	//Comment this part for testing outside the bus
	//if (event_get_errors() != 0)
	//	state = 1;
	
//	printf("UPDATE PTT, state = %i\n",state);
	
	if (state == 0) {
		main_set_inhibit_state(INHIBIT_OK_TO_SEND);
		led_set_ptt(LED_STATE_PTT_OK);
    
    if (status.alarm_output == 1)
      PORTB &= ~(1<<ALARM_OUTPUT_BIT);
	}
	else if (state == 1) {
    main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND);

    if (status.alarm_output == 0) {
      PORTB |= (1<<ALARM_OUTPUT_BIT);
      status.alarm_output = 1;
    }
    
		led_set_ptt(LED_STATE_PTT_INHIBIT);
	}
	else if (state == 2) {
		main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND_RADIO_TX);
		led_set_ptt(LED_STATE_PTT_ACTIVE);

    if (status.alarm_output == 1) {
      PORTB &= ~(1<<ALARM_OUTPUT_BIT);
      status.alarm_output = 0;
    }
	}
	else if (state == 3) {
		main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND);
		
    if (status.alarm_output == 0) {
      PORTB |= (1<<ALARM_OUTPUT_BIT);
      status.alarm_output = 1;
    }
    
    led_set_ptt(LED_STATE_PTT_INHIBIT);
	}
}

/*! \brief Set the inhibit state 
 *  This function is used to set the inhibit state, which is used to check at various places if it is safe for example
 *  to transmit, change band or change antennas. 
 *  \param state The state we wish to set the inhibit status to */
void main_set_inhibit_state(enum enum_inhibit_state state) {
	runtime_settings.inhibit_state = state;
}

/*! \brief Get the inhibit state 
 *  This function is used to get the inhibit state, which is used to check at various places if it is safe for example
 *  to transmit, change band or change antennas. 
 *  \return The current inhibit status */
enum enum_inhibit_state main_get_inhibit_state(void) {
	return(runtime_settings.inhibit_state);
}

/*! \brief Send a ping message out on the bus */
void send_ping(void) {
	//Set which is the current selected band
	ping_message[2] = status.selected_band;
	ping_message[3] = status.selected_ant;
	ping_message[4] = status.selected_rx_antenna;
	
	//Attach the PTT status of the box
	if (radio_get_ptt_status() != 0)
		ping_message[4] |= 0x80;
				
	bus_add_tx_message(bus_get_address(), BUS_BROADCAST_ADDR, 0, BUS_CMD_PING, 5, ping_message);
}

void main_update_status_field(void ) {
  if (main_get_status_field_index() == STATUS_FIELD_AMP_ON_OFF_STATUS) {
    if (status.amp_flags & (1<<AMP_STATUS_MAINS))
      display_handler_set_status_field_text_P(13,PSTR("Amp: Mains On"));
    else
      display_handler_set_status_field_text_P(14,PSTR("Amp: Mains Off"));
  }
    
  if (main_get_status_field_index() == STATUS_FIELD_AMP_OP_STBY_STATUS) {
    if (status.amp_flags & (1<<AMP_STATUS_OPR_STBY))
      display_handler_set_status_field_text_P(12,PSTR("Amp: Operate"));
    else
      display_handler_set_status_field_text_P(12,PSTR("Amp: Standby"));    
  }

  if (main_get_status_field_index() == STATUS_FIELD_AMP_STATUS) {
    /*! Amp status */
    if (status.amp_op_status == AMP_OP_STATUS_READY) {
      display_handler_set_status_field_text_P(10,PSTR("Amp: Ready"));
    }
    else if (status.amp_op_status == AMP_OP_STATUS_ERROR) {
      display_handler_set_status_field_text_P(10,PSTR("Amp: ERROR"));
    }
    else if (status.amp_op_status == AMP_OP_STATUS_TUNING) {
      display_handler_set_status_field_text_P(11,PSTR("Amp: Tuning"));      
    }
    else if (status.amp_op_status == AMP_OP_STATUS_OFF) {
      display_handler_set_status_field_text_P(8,PSTR("Amp: Off"));      
    }
    else if (status.amp_op_status == AMP_OP_STATUS_WARMUP) {
      display_handler_set_status_field_text_P(12,PSTR("Amp: Warmup"));
    }
    else if (status.amp_op_status == AMP_OP_STATUS_COOLDOWN) {
      display_handler_set_status_field_text_P(13,PSTR("Amp: Cooldown"));      
    }
    else {
      display_handler_set_status_field_text_P(12,PSTR("Amp: Standby"));      
    }    
  }

  if (main_get_status_field_index() == STATUS_FIELD_AMP_BAND_SEGMENT_STATUS) {
    char temp[15];

    if (status.amp_band == BAND_UNDEFINED)
      sprintf_P(temp,PSTR("Band: None"));
    else if (status.amp_band == BAND_160M)
      sprintf_P(temp,PSTR("Band: 160m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_80M)
      sprintf_P(temp,PSTR("Band: 80m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_40M)
      sprintf_P(temp,PSTR("Band: 40m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_30M)
      sprintf_P(temp,PSTR("Band: 30m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_20M)
      sprintf_P(temp,PSTR("Band: 20m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_17M)
      sprintf_P(temp,PSTR("Band: 17m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_15M)
      sprintf_P(temp,PSTR("Band: 15m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_12M)
      sprintf_P(temp,PSTR("Band: 12m S%i"),status.amp_segment);
    else if (status.amp_band == BAND_10M)
      sprintf_P(temp,PSTR("Band: 10m S%i"),status.amp_segment);
    
    display_handler_set_status_field_text(strlen(temp),temp);
  }
  
  if (main_get_status_field_index() == STATUS_FIELD_SHOW_RADIO_PTT_ON_OFF_STATUS) {
    if (runtime_settings.radio_ptt_output)
      display_handler_set_status_field_text_P(13,PSTR("Radio PTT: On"));
    else
      display_handler_set_status_field_text_P(14,PSTR("Radio PTT: Off"));
  }
  
  if (main_get_status_field_index() == STATUS_FIELD_SHOW_AMP_PTT_ON_OFF_STATUS) {
    if (runtime_settings.amplifier_ptt_output)
      display_handler_set_status_field_text_P(11,PSTR("Amp PTT: On"));
    else
      display_handler_set_status_field_text_P(12,PSTR("Amp PTT: Off"));
  }
  
  if (main_get_status_field_index() == STATUS_FIELD_SHOW_ROTATOR_FOCUS) {
    char index = antenna_ctrl_get_antenna_to_rotate();
    char temp[ANTENNA_TEXT_SIZE+9];
    
        
    if ((index > -1) && (index < 4)) {
      sprintf_P(temp,PSTR("Rotate: %s"),antenna_ctrl_get_antenna_text(index));
      temp[8+antenna_ctrl_get_antenna_text_length(index)] = 0;

      display_handler_set_status_field_text(strlen(temp),temp);
    }
    else
      display_handler_set_status_field_text_P(12,PSTR("Rotate: None"));
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
	
	delay_ms(250);
	
	//Initialize the 128x64 display
	glcd_init();
	glcd_clear();
	
	i2c_init();

	//Check if the computer interface should have full control of the box, setup mode
	if (PIND & (1<<5)) {
		computer_interface_deactivate_setup();
	}
	else {
		PORTC |= (1<<7);
		computer_interface_activate_setup();
	}

  //Check if this is the first time we start the device, if so we need to initiate some
	//data structures. To force this at startup just change the value that should be read and
	//written to the EEPROM
	if (eeprom_read_startup_byte() != 0x06) {
		eeprom_create_table();
		
		eeprom_read_table();
		
		//write the default runtime settings
		runtime_settings.lcd_backlight_value = 20;
		runtime_settings.amplifier_ptt_output = 1;
		runtime_settings.radio_ptt_output = 1;
		runtime_settings.inhibit_state = 0;
		runtime_settings.band_change_mode = BAND_CHANGE_MODE_MANUAL;
		runtime_settings.ethernet_local_mode = 0;
    
    for (unsigned char i=0;i<9;i++)
      runtime_settings.antenna_disabled[i] = 0;
    
		//Write the settings to the EEPROM
		eeprom_save_runtime_settings(&runtime_settings);
		
		eeprom_write_startup_byte(0x06);

		//The first time the box is started, we need to setup the settings
		computer_interface_activate_setup();
	}
	
	//Init the status structure
	status.buttons_last_state = 0;
	status.buttons_current_state = 0;
	status.ext_devices_current_state = ih_poll_ext_devices();
	status.ext_devices_last_state = status.ext_devices_current_state;
	status.selected_rx_antenna = 0;
	status.new_band = BAND_UNDEFINED;
	status.current_band_portion = BAND_LOW;
	status.new_band_portion = BAND_LOW;
	status.sub_menu_antenna_index = 0;
	status.rotator_step_resolution = 5;
  status.last_critical_cmd_state = 99;	
  status.curr_critical_cmd_state = 1;
  
  status.alarm_output = (1<<ALARM_OUTPUT_BIT);
  PORTB |= (1<<ALARM_OUTPUT_BIT);
  
  
  //!! Erase the external EEPROM. uncomment to do so !!
  //eeprom_m24_write_block(0,32000,255);
  //eeprom_write_startup_byte(0x00);  //Will force a configuration of the box at startup
    
	//Load all settings from the EEPROM	
	load_settings();
        
	//Init the communzication routines between the computer and the openASC box
	computer_interface_init();

	//Init the backlight PWM
	init_backlight();
        
	//Set powermeter address to AUTO, always is configured this way at startup
	runtime_settings.powermeter_address = 0; 
	
	//This will go through the antenna_disabled variable and check so there is no faulty value
	//This is useful mostly the first time a box gets the upgrade supporting this feature.
	unsigned char ant_dis_changed = 0;
	
	for (unsigned char i=0;i<9;i++)
		if (runtime_settings.antenna_disabled[i] > 15) {
			runtime_settings.antenna_disabled[i] = 0;
			ant_dis_changed = 1;
	}
	
	if (ant_dis_changed != 0)
		eeprom_save_runtime_settings(&runtime_settings);
	
	display_handler_set_backlight(runtime_settings.lcd_backlight_value);
		
	//Check to see if the radio interface is configured as manual or automatic as default
	if (radio_interface_get_interface() == RADIO_INTERFACE_MANUAL)
		runtime_settings.band_change_mode = BAND_CHANGE_MODE_MANUAL;
	else
		runtime_settings.band_change_mode = BAND_CHANGE_MODE_AUTO;

	//Init the communication between the uCs
	init_usart();

	//Init the communication between the motherboard uC and the front panel uC
	//The first argument will be called each time a message should be parsed
	//The second argument will be called each time a character should be sent
	internal_comm_init((void *)event_internal_comm_parse_message, (void *)usart0_transmit);

	//Init the realtime clock
	ds1307_init();
	
	if (!computer_interface_is_active()) {
		//Initialize the radio interface
    #ifndef DEBUG_COMPUTER_USART_ENABLED
      radio_interface_init();
    #endif
      
    #ifdef DEBUG_COMPUTER_USART_ENABLED
		  init_usart_computer();
    #endif
      
    #ifdef ETHERNET_DEBUG_ENABLED
      init_usart_computer();
    #endif
	}
	else {
		//Init the computer communication
		init_usart_computer();
		
		sei();
		
		display_handler_new_view(DISPLAY_HANDLER_VIEW_CONFIG_SPLASH);
    display_handler_repaint();
    
    //Forced to generate the tick ourselfs
    display_handler_tick();
    
		while(1) {
			computer_interface_send_data();
			
			computer_interface_parse_data();
			
			//Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
			internal_comm_poll_rx_queue();
		
			//Poll the TX queue in the internal comm to see if we have any new messages to be SE
			internal_comm_poll_tx_queue();
		}
	}
	
	band_ctrl_load_band_limits();
			
	/* This delay is simply so that if you have the devices connected to the same power supply
	all units should not send their status messages at the same time. Therefor we insert a delay
	that is based on the external address of the device */	
	delay_ms(19 * settings.network_address);
	
	device_count = settings.network_device_count;
	
	//This must be done in this order for it to work properly!
  bus_set_address(settings.network_address);
	
	bus_init();

	//Init the power meter
	powermeter_init(settings.powermeter_update_rate_text, settings.powermeter_update_rate_bargraph);
	
	if (settings.network_device_is_master == 1) {
		bus_set_is_master(1,settings.network_device_count);
	}
	else {
		bus_set_is_master(0,0);	
	}
	
	init_timer_0();
	
	//Timer used for the communication bus. The interrupt is caught in bus.c
	init_timer_2();

	//Print the startup picture
	display_handler_new_view(DISPLAY_HANDLER_VIEW_OPENASC_LOGO);
	display_handler_repaint();
  
	//Set all leds on at startup so it's possible to see everything works
	led_set_all(LED_STATE_ON);
	delay_ms(250);
	led_set_all(LED_STATE_OFF);

  wdt_enable(WDTO_8S);

  ethernet_init();
  
	//Initialize the menu system
	menu_init();
	
	main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND);
	led_set_ptt(LED_STATE_PTT_INHIBIT);
	
	set_knob_function(KNOB_FUNCTION_AUTO);
	
	sei();
	
	//TEMPORARY
	DDRB |= (1<<1);
	
  main_update_status_field();
  
	ping_message[0] = DEVICE_ID_MAINBOX;
	ping_message[1] = settings.ptt_interlock_input;
  
  //This will make the box enabled after 3 seconds, this is so that we have recieved all the pings from different units
  //so that we know which band they are on, before we enter a band ourself.
  event_add_message((void *)main_enable_device,3000,0);
	
  #ifdef ETHERNET_DEBUG_ENABLED
    printf("openASC started in ethernet debug mode\r\n");
  #endif
  
  #ifdef DEBUG_COMPUTER_USART_ENABLED
    printf("openASC started in USART debug mode\n");
    
    /*printf("POWER METER DEBUG\n\r");
    printf("-----------------\n\r");
    for (unsigned char i=0;i<9;i++) {
      printf("Band[%i] addr: 0x%02X\n\r",i,settings.powermeter_address[i]);
      printf("Band[%i] vswr: %i\n\r",i,settings.powermeter_vswr_limit[i]);
      printf("Band[%i] refp: %i\n\r",i,settings.powermeter_ref_power_limit[i]);
    }*/
  #endif

  BUS_MESSAGE mess;
  
	while(1) {
    wdt_reset();
    
    if (bus_check_rx_status(&mess)) {
      event_bus_parse_message(mess);
    }
    
    bus_check_tx_status();

		//This variable can be set to 0 to disable all the control of the device
		//For example this is used when the box is about to shut down
		if (device_online == 1) {
			if (runtime_settings.band_change_mode != BAND_CHANGE_MODE_MANUAL) {
				if ((counter_ms % (radio_interface_get_poll_interval()*10)) == 0) {
					if (radio_get_current_band() != status.selected_band) {
						if (main_band_change_ok(radio_get_current_band()) == 1) {
							main_set_new_band(radio_get_current_band());
						}
						else {
							if (status.selected_band != BAND_UNDEFINED)
								main_set_new_band(BAND_UNDEFINED);
						}
					}
				}
			}
			
			//Check the VSWR and reflected power values, if they are too high we lock the PTT
      if (status.selected_band > 0) {
        if ((settings.powermeter_vswr_limit[status.selected_band-1] != 0) && (powermeter_get_vswr() >= settings.powermeter_vswr_limit[status.selected_band-1])) {
          //Check so that we are actually transmitting
          if (main_get_inhibit_state() == INHIBIT_NOT_OK_TO_SEND_RADIO_TX)
            error_handler_set(ERROR_TYPE_HIGH_VSWR,1,0);
        }
          
        if ((settings.powermeter_ref_power_limit[status.selected_band-1] != 0) && (powermeter_get_ref_power() >= settings.powermeter_ref_power_limit[status.selected_band-1])) {
          //Check so that we are actually transmitting
          if (main_get_inhibit_state() == INHIBIT_NOT_OK_TO_SEND_RADIO_TX)
            error_handler_set(ERROR_TYPE_HIGH_REF_POWER,1,0);
        }
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
		
			//if (main_flags & (1<<FLAG_POLL_PULSE_SENSOR)) {
      //Testing if the rotary encoder can be polled quickly, if it does have
      //any other impact. If not the if(1) can be removed 
      encoder_val = rotary_encoder_poll();
    
      if (encoder_val != 0) {
        if (encoder_val == 1)
          event_pulse_sensor_up();
        else if (encoder_val == -1)
          event_pulse_sensor_down();
      
        counter_last_pulse_event = 0;
      }
		
			if (main_flags & (1<<FLAG_BLINK_BAND_LED)) {
				if (status.new_band != status.selected_band) {
					if (main_flags & (1<<FLAG_LAST_BAND_BLINK)) {
						led_set_band_none();
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
		
				main_flags &= ~(1<<FLAG_BLINK_BAND_LED);
			}
		
			if (main_flags & (1<<FLAG_POLL_RADIO)) {
				radio_poll_status();
			
				main_flags &= ~(1<<FLAG_POLL_RADIO);
			}
		
			if (main_flags & (1<<FLAG_PROCESS_RX_ANT_CHANGE)) {
				antenna_ctrl_change_rx_ant(status.selected_rx_antenna);
        
				main_flags &= ~(1<<FLAG_CHANGE_RX_ANT);
				main_flags &= ~(1<<FLAG_PROCESS_RX_ANT_CHANGE);
			}
		
			if (main_flags & (1<<FLAG_PROCESS_SUBMENU_CHANGE)) {
				sub_menu_send_data_to_bus(status.sub_menu_antenna_index, sub_menu_get_current_pos(status.sub_menu_antenna_index));
			
				main_flags &= ~(1<<FLAG_CHANGE_SUBMENU);
				main_flags &= ~(1<<FLAG_PROCESS_SUBMENU_CHANGE);
			}
		
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
		
      if (ethernet_is_active()) {
        if (led_status_changed()) {
          remote_control_send_status();
        }
      }
		
			radio_process_tasks();
      
      //TODO: FIX BAND IN USE???
      //Check that we aren't on the same band as another box every 250 ms
      /*if ((counter_ms % 250) == 0) {
        if (main_band_change_ok(status.selected_band) == 0) {
            if (error_handler_get_state(ERROR_TYPE_BAND_IN_USE) == 0) {
              error_handler_set(ERROR_TYPE_BAND_IN_USE,1,0);
            }
        }
      }*/
		}

    //Poll the RX queue in the internal comm to see if we have any new messages to be PARSED
    internal_comm_poll_rx_queue();

    //Poll the TX queue in the internal comm to see if we have any new messages to be SENT
    internal_comm_poll_tx_queue();

    if (ethernet_get_chip_enabled())
      ethernet_process();

    if ((counter_ms % DISPLAY_HANDLER_TICK_INTERVAL) == 0) {
      display_handler_tick();
    }

    
    if (counter_powermeter_update_text >= powermeter_get_text_update_rate()) {
      counter_powermeter_update_text = 0;
      display_handler_powermeter_text_tick();
    }

    if (counter_powermeter_update_bargraph >= powermeter_get_bargraph_update_rate()) {
      counter_powermeter_update_bargraph = 0;
      display_handler_powermeter_bargraph_tick();
    }
    
    //Check every 10 ms if we are allowed to PTT or not
    if ((counter_critical_cmd_check % 10) == 0) {
      main_update_critical_list();
      
      counter_critical_cmd_check = 0;
    }

		if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
			//Run the event in the queue
			event_run();
			main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
		}
		
    event_check_pings();
	}
}

unsigned int main_get_ms_counter() {
  return(counter_ms);  
}

unsigned char te=0;

/*!Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0A) {
  bus_check_ack_list();
  
	//If this device should act as master it should send out a SYNC command
	//and also the number of devices (for the time slots) that are active on the bus
	if ((device_started == 1) && (bus_is_master())) {
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
	
	if (!display_handler_screensaver_mode()) {
		if (counter_screensaver_timeout >= DISPLAY_SCREENSAVER_TIMEOUT) {
			display_handler_enable_screensaver();

			event_add_message((void*)display_handler_update_screensaver, 0,0);
			display_handler_set_backlight(DISPLAY_SCREENSAVER_DEF_CONTRAST);
			
			counter_screensaver_timeout = 0;
		}
	}
	
	counter_last_pulse_event++;
	counter_poll_rotary_encoder++;
	counter_poll_buttons++;
	counter_poll_ext_devices++;	
  counter_critical_cmd_check++;
  counter_powermeter_update_bargraph++;  
  counter_powermeter_update_text++;
	
	if ((counter_ms % 1000) == 0)
		counter_screensaver_timeout++;
	
	counter_ping_interval++;
	counter_ms++;
	counter_event_timer++;

	radio_interface_1ms_tick();
	
	//This will blink the NEW BAND LED, if the new band is not the same as the old one
	if ((counter_ms % 250) == 0) {
		main_flags |= (1<<FLAG_BLINK_BAND_LED);
	}
	
	//Update every 500 ms
	if ((counter_ms % 500) == 0) {

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
		if (main_flags & (1<<FLAG_CHANGE_RX_ANT))
			main_flags |= (1<<FLAG_PROCESS_RX_ANT_CHANGE);
	}
	
	if (counter_last_pulse_event > PULSE_SENSOR_SUBMENU_CHANGE_LIMIT) {
		if (main_flags & (1<<FLAG_CHANGE_SUBMENU))
			main_flags |= (1<<FLAG_PROCESS_SUBMENU_CHANGE);
	}
	
	if (runtime_settings.band_change_mode == BAND_CHANGE_MODE_AUTO) {
    if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
      if (counter_poll_radio >= (radio_interface_get_poll_interval() * 10)) {
        main_flags |= (1<<FLAG_POLL_RADIO);
        
        counter_poll_radio = 0;
      }
			
      counter_poll_radio++;
    }
	}
	
	//If the value equals the half of it's range then
	//we remove that amount from all target times in the event
	//queue and set the counter to 0 again
	if (counter_event_timer >= 32767) {
		event_queue_wrap(32767);
		
		counter_event_timer = 0;
	}
	
	internal_comm_1ms_timer();
	
  if ((counter_ms % 100) == 0)
    bus_ping_tick();
}

/*!Output overflow 0 interrupt */
ISR(SIG_OVERFLOW0) {
}
