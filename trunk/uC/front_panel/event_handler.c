/*! \file front_panel/event_handler.c '
 *  \brief Event handler of various things
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/event_handler.c" \endcode
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
#include <avr/io.h>x
#include <avr/interrupt.h>
#include <string.h>

#include "event_handler.h"
#include "main.h"
#include "board.h"
#include "display.h"
#include "glcd.h"
#include "led_control.h"
#include "../delay.h"
#include "../i2c.h"
#include "../global.h"
#include "band_ctrl.h"
#include "antenna_ctrl.h"
#include "remote_control.h"
#include "eeprom_m24.h"
#include "rotary_encoder.h"
#include "menu.h"
#include "radio_interface.h"
#include "sequencer.h"
#include "interrupt_handler.h"
#include "glcd.h"
#include "../internal_comm.h"
#include "errors.h"

//#define DEBUG_WMV_BUS 1


//#define ERROR_DEBUG

extern unsigned int main_flags;

unsigned char flag_errors = 0;

/*! \brief Set that an error has occured 
 *  \param error_type The type of error that has occured, defines can be found in errors.h
 *  \param state State of the error */
void event_set_error(unsigned char error_type, unsigned char state) {
	if (state == 0)
		flag_errors &= ~(1<<error_type);
	else
		flag_errors |= (1<<error_type);
	
	#ifdef ERROR_DEBUG
		printf("ERROR_TYPE: %i - STATE: %i\n",error_type,state);
	#endif
}

/*! \brief Retrieve the state error flags */
unsigned char event_get_errors(void) {
	return(flag_errors);
}

/*! \brief Retrieve the state of a specific error type
 *  \param error_type Which kind of error we wish to check the state for
 *  \return The current state of this error */
unsigned char event_get_error_state(unsigned char error_type) {
	if (flag_errors & (1<<error_type))
		return(1);

	return(0);
}

/*! \brief Function which will parse the internal communication message 
 *  \param message The message that we wish to parse */
void event_internal_comm_parse_message(UC_MESSAGE message) {
	//Init the sequence of saving all data and disable all outputs activated by this unit
	switch(message.cmd) {
		case INT_COMM_TURN_DEVICE_OFF:
			if (!computer_interface_is_active()) {
				//TODO: Problem with delay here, need to wait until everything is shut off
				//This solution is pretty uggly...do it some other way?
				status.current_display = CURRENT_DISPLAY_SHUTDOWN_VIEW;
				display_shutdown_view();
				
				main_save_settings();
				
				band_ctrl_change_band(BAND_UNDEFINED);
				
				//TODO: Send global shutdown broadcast message three times
				
				send_ping();
				
				event_add_message((void *)shutdown_device,3000,0);
			}
			else {
				status.current_display = CURRENT_DISPLAY_SHUTDOWN_VIEW;
				display_shutdown_view();

				delay_s(2);

				shutdown_device();
			}
			
			break;
		case INT_COMM_PS2_KEYPRESSED:
			event_handler_process_ps2(message.data[0]);
			break;
		case INT_COMM_PC_CTRL:
			remote_control_parse_command(message.data[0],(unsigned char)message.data[1], (char *)(message.data+2));
			break;
		case INT_COMM_GET_BAND_BCD_STATUS:
			if (message.data[0] != radio_get_current_band()) {
				if ((message.data[0] >= BAND_UNDEFINED) && (message.data[0] <= BAND_10M))
					radio_set_current_band(message.data[0]);
			}
			break;
		default:
			break;
	}
}

/*! \brief Set an RX antenna. Will set the proper flags and call the antenna_ctrl_change_rx_ant function
 *  \param ant_index The index of the RX antenna we wish to chose */
void __inline__ event_set_rx_antenna(unsigned char ant_index) {
	status.selected_rx_antenna = ant_index;
	
	antenna_ctrl_change_rx_ant(status.selected_rx_antenna);
	main_flags |= (1<<FLAG_UPDATE_DISPLAY);
}

/*! \brief Process an PS2 event 
 *  \param key_code The key that was pressed */
void event_handler_process_ps2(unsigned char key_code) {
	char func_index = -1;
	
	if (key_code == KEYPAD_BTN_0)
		func_index = 0;
	else if (key_code == KEYPAD_BTN_1)
		func_index = 1;
	else if (key_code == KEYPAD_BTN_2)
		func_index = 2;
	else if (key_code == KEYPAD_BTN_3)
		func_index = 3;
	else if (key_code == KEYPAD_BTN_4)
		func_index = 4;
	else if (key_code == KEYPAD_BTN_5)
		func_index = 5;
	else if (key_code == KEYPAD_BTN_6)
		func_index = 6;
	else if (key_code == KEYPAD_BTN_7)
		func_index = 7;
	else if (key_code == KEYPAD_BTN_8)
		func_index = 8;
	else if (key_code == KEYPAD_BTN_9)
		func_index = 9;
	else if (key_code == KEYPAD_BTN_A)
		func_index = 10;
	else if (key_code == KEYPAD_BTN_B)
		func_index = 11;
	else if (key_code == KEYPAD_BTN_C)
		func_index = 12;
	else if (key_code == KEYPAD_BTN_D)
		func_index = 13;
	else if (key_code == KEYPAD_BTN_E)
		func_index = 14;
	else if (key_code == KEYPAD_BTN_F)
		func_index = 15;
	else if (key_code == KEYPAD_BTN_G)
		func_index = 16;
	
	unsigned char curr_task = ext_key_get_assignment(func_index);
	
	/* Requires that we dont change the order of the functions */
	if ((curr_task >= EXT_KEY_SEL_RX_ANT1) && (curr_task <= EXT_KEY_SEL_RX_ANT10)) {
		event_set_rx_antenna(curr_task);
		return;
	}	
	
	//TODO: Continue with implementation of ext keypad functions
	
	switch(curr_task) {
		case EXT_KEY_SEL_NONE:
			//Do nothing
			break;
		case EXT_KEY_TOGGLE_TX_ANT1:
			event_tx_button1_pressed();
			break;
		case EXT_KEY_TOGGLE_TX_ANT2:
			event_tx_button2_pressed();
			break;
		case EXT_KEY_TOGGLE_TX_ANT3:
			event_tx_button3_pressed();
			break;
		case EXT_KEY_TOGGLE_TX_ANT4:
			event_tx_button4_pressed();
			break;
		case EXT_KEY_TOGGLE_RX_ANT_MODE:
			event_rxant_button_pressed();
			break;
		default:
			break;
	}
}

/*! \brief The pulse sensor was turned up */
void event_pulse_sensor_up(void) {
	if (status.current_display == CURRENT_DISPLAY_MENU_SYSTEM) {
		menu_action(MENU_SCROLL_UP);
	}
	else {
		//If the knob function is RX ANT SELECT then go up the list of 
		//RX antennas
		if (status.knob_function == KNOB_FUNCTION_RX_ANT) {
			if (status.selected_rx_antenna < (antenna_ctrl_get_rx_antenna_count()))
				status.selected_rx_antenna++;
			else
				status.selected_rx_antenna = 1;
		
			//Set a flag that we wish to update the RX antenna, if the PULSE_SENSOR_RX_ANT_CHANGE_LIMIT time has passed
			main_flags |= (1<<FLAG_CHANGE_RX_ANT);
			main_flags |= (1<<FLAG_UPDATE_DISPLAY);
		}
		else if (status.knob_function == KNOB_FUNCTION_SELECT_BAND) {
			if (status.new_band < BAND_10M)
				status.new_band++;
		}
		else if (status.knob_function == KNOB_FUNCTION_SET_HEADING) {
			if (status.new_beamheading < (antenna_ctrl_get_start_heading(status.antenna_to_rotate-1) + antenna_ctrl_get_max_rotation(status.antenna_to_rotate-1)))
				status.new_beamheading += status.rotator_step_resolution;
			else
				status.new_beamheading = antenna_ctrl_get_start_heading(status.antenna_to_rotate-1);
					
	//		display_show_set_heading(status.new_beamheading, antenna_ctrl_get_360_deg_view(status.antenna_to_rotate-1));
		}
	}
}

/*! \brief The pulse sensor was turned down */
void event_pulse_sensor_down(void) {
	if (status.current_display == CURRENT_DISPLAY_MENU_SYSTEM) {
		menu_action(MENU_SCROLL_DOWN);
	}
	else {
		//If the knob function is RX ANT SELECT then go down the list of 
		//RX antennas
		if (status.knob_function == KNOB_FUNCTION_RX_ANT) {
			if (status.selected_rx_antenna > 1)
				status.selected_rx_antenna--;
			else
				status.selected_rx_antenna = antenna_ctrl_get_rx_antenna_count();
		
			//Set a flag that we wish to update the RX antenna, if the PULSE_SENSOR_RX_ANT_CHANGE_LIMIT time has passed
			main_flags |= (1<<FLAG_CHANGE_RX_ANT);
			main_flags |= (1<<FLAG_UPDATE_DISPLAY);
		}
		else if (status.knob_function == KNOB_FUNCTION_SELECT_BAND) {
			if (status.new_band > BAND_UNDEFINED)
				status.new_band--;
		}	//TODO: Fix all the rotator options properly
		else if (status.knob_function == KNOB_FUNCTION_SET_HEADING) {
			if (status.new_beamheading > antenna_ctrl_get_start_heading(status.antenna_to_rotate-1))
				status.new_beamheading -= status.rotator_step_resolution;
			else
				status.new_beamheading = antenna_ctrl_get_start_heading(status.antenna_to_rotate-1) + antenna_ctrl_get_max_rotation(status.antenna_to_rotate-1);
			
//			display_show_set_heading(status.new_beamheading, antenna_ctrl_get_360_deg_view(status.antenna_to_rotate-1));
		}
		
	}
}

/*! \brief Function to be called if we wish to update the display */
void event_update_display(void) {
	if (status.current_display == CURRENT_DISPLAY_ANTENNA_INFO) {
		//Should we show the RX antenna?
		if (status.function_status & (1<<FUNC_STATUS_RXANT))
			display_show_rx_ant(status.selected_rx_antenna);
		else
			display_show_rx_ant(0);
			
		display_update(status.selected_band, status.selected_ant);
	}
	else if (status.current_display == CURRENT_DISPLAY_LOGO) {
		glcd_print_picture();
	}
	else if (status.current_display == CURRENT_DISPLAY_MENU_SYSTEM) {
		menu_show();
	}
}

/*! \brief Function which will poll all buttons and perform the proper action depending on their state */
void event_poll_buttons(void) {
	status.buttons_current_state = ih_poll_buttons();
	
	//Any change? If so then parse what change
	if (status.buttons_last_state != status.buttons_current_state) {
		unsigned int btn_status = status.buttons_current_state ^ status.buttons_last_state;
		
		//event_parse_button_pressed(status.buttons_current_state ^ status.buttons_last_state);
			//Checks if the status of this antenna button was changed
		if (btn_status & (1<<FLAG_BUTTON1_TX_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON1_TX_BIT))
				event_tx_button1_pressed();
		}

	//Checks if the status of this antenna button was changed
		if (btn_status & (1<<FLAG_BUTTON2_TX_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON2_TX_BIT))
				event_tx_button2_pressed();
		}
	
	//Checks if the status of this antenna button was changed
		if (btn_status & (1<<FLAG_BUTTON3_TX_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON3_TX_BIT))
				event_tx_button3_pressed();
		}
	
	//Checks if the status of this antenna button was changed
		if (btn_status & (1<<FLAG_BUTTON4_TX_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON4_TX_BIT))
				event_tx_button4_pressed();
		}
			if (btn_status & (1<<FLAG_BUTTON_ROTATE_BIT)) {
			event_rotate_button_pressed();
		}
		
		if (btn_status & (1<<FLAG_BUTTON_RXANT_BIT)) {
			event_rxant_button_pressed();
		}
	
	//Check if the MENU button has been pressed
		if (btn_status & (1<<FLAG_BUTTON_MENU_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON_MENU_BIT)) {
				if (status.current_display == CURRENT_DISPLAY_MENU_SYSTEM) {
					if (status.selected_band != BAND_UNDEFINED) {
						status.current_display = CURRENT_DISPLAY_ANTENNA_INFO;
						status.current_display_level = DISPLAY_LEVEL_BAND;
					}
					else
						status.current_display = CURRENT_DISPLAY_LOGO;
						
						led_set_menu(LED_STATE_OFF);
				}
				else {
					menu_reset();
					status.current_display = CURRENT_DISPLAY_MENU_SYSTEM;
					led_set_menu(LED_STATE_ON);
				}
			
				main_update_display();
			}
		}
	
		if (btn_status & (1<<	FLAG_BUTTON_PULSE_BIT)) {
			if (status.buttons_current_state & (1<<	FLAG_BUTTON_PULSE_BIT)) {
				if (status.current_display == CURRENT_DISPLAY_MENU_SYSTEM)
					menu_action(MENU_BUTTON_PRESSED);
				else if (status.knob_function == KNOB_FUNCTION_SELECT_BAND) {
					status.current_band_portion = BAND_LOW;
					band_ctrl_change_band(status.new_band);
					
					display_update_radio_freq();
					send_ping();
				}
				else if (status.knob_function == KNOB_FUNCTION_SET_HEADING) {
					if (status.antenna_to_rotate != 0) {
						antenna_ctrl_rotate(status.antenna_to_rotate-1,status.new_beamheading);
					
						led_set_rotate(LED_STATE_OFF);
						status.function_status &= ~(1<<FUNC_STATUS_ROTATE);
				
						status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
						status.antenna_to_rotate = 0;
				
						set_knob_function(KNOB_FUNCTION_AUTO);
					
						glcd_clear();
						main_update_display();
					}
				}
			}
		}
		
		if (btn_status & (1<<FLAG_BUTTON_AUX2_BIT)) {
			if (status.buttons_current_state & (1<<FLAG_BUTTON_AUX2_BIT))
				event_aux2_button_pressed();
		}
	}
	
	status.buttons_last_state = status.buttons_current_state;
}

/*! \brief Perform the actions that should be done when AUX 2 button is pressed */
void event_aux2_button_pressed(void) {
	if ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_MANUAL) || (radio_interface_get_interface() == RADIO_INTERFACE_BCD)) {
		if (status.current_band_portion == BAND_HIGH)
			status.new_band_portion = BAND_LOW;
		else
			status.new_band_portion = BAND_HIGH;

		if (status.new_band_portion != status.current_band_portion) {
			status.current_band_portion = status.new_band_portion;
				
			display_update_radio_freq();
			band_ctrl_change_band_portion(status.current_band_portion);
		}
	}
}

/*! \brief Function which will poll the external devices and perform the proper actions depending on their state */
void event_poll_ext_device(void) {
	status.ext_devices_current_state = ih_poll_ext_devices();
	
	if (status.ext_devices_current_state != status.ext_devices_last_state) {
		event_parse_ext_event(status.ext_devices_current_state ^ status.ext_devices_last_state);
	}
	
	status.ext_devices_last_state = status.ext_devices_current_state;
}

/*! \brief Perform the action of TX antenna button 1 if it was pressed */
void event_tx_button1_pressed(void) {
	if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
		if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_EXIST_FLAG)) {
			unsigned char new_ant_comb = status.selected_ant;
			
			if ((status.function_status & (1<<FUNC_STATUS_SELECT_ANT_ROTATE)) == 0) {
				if (status.selected_ant & (1<<0)) {
					new_ant_comb &= ~(1<<0);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
						
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(1,LED_STATE_OFF);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF0)) {
							status.selected_ant = new_ant_comb & 0xF0;
								
							antenna_ctrl_send_ant_data_to_bus();
			
							led_set_tx_ant(0,LED_STATE_OFF);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
				else {
					new_ant_comb |= (1<<0);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
		
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(1,LED_STATE_ON);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF1)) {
							status.selected_ant = new_ant_comb & 0xF1;
		
							antenna_ctrl_send_ant_data_to_bus();
			
							led_set_tx_ant(0,LED_STATE_OFF);
							led_set_tx_ant(1,LED_STATE_ON);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
			}
			else {
				status.antenna_to_rotate = 1;
				status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
				
				set_tx_ant_leds();
				
				//event_handler_enable_rotator_interface();
			}
		}
		
		main_update_ptt_status();
	}
}

/*! \brief Perform the action of TX antenna button 2 if it was pressed */
void event_tx_button2_pressed(void) {
	if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
		if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_EXIST_FLAG)) {
			unsigned char new_ant_comb = status.selected_ant;
			
			if ((status.function_status & (1<<FUNC_STATUS_SELECT_ANT_ROTATE)) == 0) {
				if (status.selected_ant & (1<<1)) {
					new_ant_comb &= ~(1<<1);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
		
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(2,LED_STATE_OFF);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF0)) {
							status.selected_ant = new_ant_comb & 0xF0;
			
							antenna_ctrl_send_ant_data_to_bus();
								
							led_set_tx_ant(0,LED_STATE_OFF);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
				else {		
					new_ant_comb |= (1<<1);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
		
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(2,LED_STATE_ON);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF2)) {
							status.selected_ant = new_ant_comb & 0xF2;
			
							antenna_ctrl_send_ant_data_to_bus();
								
							led_set_tx_ant(0,LED_STATE_OFF);
							led_set_tx_ant(2,LED_STATE_ON);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
			} else {
				status.antenna_to_rotate = 2;
				status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
				
				set_tx_ant_leds();
				//event_handler_enable_rotator_interface();
			}
		}
		
		main_update_ptt_status();
	}
}

/*! \brief Perform the action of TX antenna button 3 if it was pressed */
void event_tx_button3_pressed(void) {
	if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
		if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_EXIST_FLAG)) {
			unsigned char new_ant_comb = status.selected_ant;
	
			if ((status.function_status & (1<<FUNC_STATUS_SELECT_ANT_ROTATE)) == 0) {
				if (status.selected_ant & (1<<2)) {
					new_ant_comb &= ~(1<<2);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
		
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(3,LED_STATE_OFF);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF0)) {
							status.selected_ant = new_ant_comb & 0xF0;
								
							antenna_ctrl_send_ant_data_to_bus();
								
							led_set_tx_ant(0,LED_STATE_OFF);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
				else {
					new_ant_comb |= (1<<2);
						
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
		
						antenna_ctrl_send_ant_data_to_bus();
							
						led_set_tx_ant(3,LED_STATE_ON);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF4)) {
							status.selected_ant = new_ant_comb & 0xF4;
			
							antenna_ctrl_send_ant_data_to_bus();
								
							led_set_tx_ant(0,LED_STATE_OFF);
							led_set_tx_ant(3,LED_STATE_ON);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
			}
			else {
				status.antenna_to_rotate = 3;
				status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
				
				set_tx_ant_leds();
				//event_handler_enable_rotator_interface();
			}
		}
		
		main_update_ptt_status();
	}
}

/*! \brief Perform the action of TX antenna button 4 if it was pressed */
void event_tx_button4_pressed(void) {
	if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
		if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_EXIST_FLAG)) {
			unsigned char new_ant_comb = status.selected_ant;	
			
			if ((status.function_status & (1<<FUNC_STATUS_SELECT_ANT_ROTATE)) == 0) {
				if (status.selected_ant & (1<<3)) {
					new_ant_comb &= ~(1<<3);
							
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
			
						antenna_ctrl_send_ant_data_to_bus();
								
						led_set_tx_ant(4,LED_STATE_OFF);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF0)) {
							status.selected_ant = new_ant_comb & 0xF0;
				
							antenna_ctrl_send_ant_data_to_bus();
									
							led_set_tx_ant(0,LED_STATE_OFF);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
				else {		
					new_ant_comb |= (1<<3);
							
					if (antenna_ctrl_comb_allowed(new_ant_comb)) {
						status.selected_ant = new_ant_comb;
			
						antenna_ctrl_send_ant_data_to_bus();
								
						led_set_tx_ant(4,LED_STATE_ON);
						display_update(status.selected_band, status.selected_ant);
					}
					else {
						if (antenna_ctrl_comb_allowed(new_ant_comb & 0xF8)) {
							status.selected_ant = new_ant_comb & 0xF8;
				
							antenna_ctrl_send_ant_data_to_bus();
									
							led_set_tx_ant(0,LED_STATE_OFF);
							led_set_tx_ant(4,LED_STATE_ON);
							display_update(status.selected_band, status.selected_ant);
						}
					}
				}
			}
			else {
				status.antenna_to_rotate = 4;
				status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
					
				set_tx_ant_leds();				
				//event_handler_enable_rotator_interface();
			}
		}
		
		main_update_ptt_status();
	}
}

/*! \brief Perform the action of RX antenna button if it was pressed */
void event_rxant_button_pressed(void) {
	if (status.buttons_current_state & (1<<FLAG_BUTTON_RXANT_BIT)) {
		if (antenna_ctrl_get_rx_antenna_count() != 0) {
				
				//If the RX ANT isn't active we enter this part, also if the rx antenna is active but knob selection is on another function
			if (((status.function_status & (1<<FUNC_STATUS_RXANT)) == 0) || ((status.function_status & (1<<FUNC_STATUS_RXANT)) && (status.knob_function != KNOB_FUNCTION_RX_ANT))) {
				status.selected_rx_antenna = status.last_rx_antenna;
				
				//If no antenna is selected, then we select the first one, or if the selected rx antenna for some reason is more than the number of antennas available.
				if ((status.selected_rx_antenna == 0) || (status.selected_rx_antenna > antenna_ctrl_get_rx_antenna_count()))
					status.selected_rx_antenna = 1;
				
				status.function_status |= (1<<FUNC_STATUS_RXANT);
				
				led_set_rxant(LED_STATE_ON);
					
				main_flags |= (1<<FLAG_UPDATE_DISPLAY);
					
				antenna_ctrl_change_rx_ant(status.selected_rx_antenna);
					
				set_knob_function(KNOB_FUNCTION_RX_ANT);
			}
			else {
				led_set_rxant(LED_STATE_OFF);
				led_set_rotate(LED_STATE_OFF);
					
				set_knob_function(KNOB_FUNCTION_AUTO);
					
				status.last_rx_antenna = status.selected_rx_antenna;
				status.selected_rx_antenna = 0;
				antenna_ctrl_change_rx_ant(0);
					
				status.function_status &= ~(1<<FUNC_STATUS_RXANT);
				main_flags |= (1<<FLAG_UPDATE_DISPLAY);
			}
		}
		else	//If we don't have any antennas to choose we always have the LED off
			led_set_rxant(LED_STATE_OFF);
	}
}

/*! \brief Perform the action of Rotate button if it was pressed */
void event_rotate_button_pressed(void) {
/*	if (status.buttons_current_state & (1<<FLAG_BUTTON_ROTATE_BIT)) {
			
		if (((status.function_status & (1<<FUNC_STATUS_ROTATE)) == 0) || ((status.function_status & (1<<FUNC_STATUS_ROTATE)) && (status.knob_function != KNOB_FUNCTION_SET_HEADING))) {
			led_set_rotate(LED_STATE_ON);
			status.function_status |= (1<<FUNC_STATUS_ROTATE);
			set_knob_function(KNOB_FUNCTION_SET_HEADING);
				
			status.function_status |= (1<<FUNC_STATUS_SELECT_ANT_ROTATE);
		}
		else {
			led_set_rotate(LED_STATE_OFF);
			status.function_status &= ~(1<<FUNC_STATUS_ROTATE);
				
			status.function_status &= ~(1<<FUNC_STATUS_SELECT_ANT_ROTATE);
			status.antenna_to_rotate = 0;
			set_tx_ant_leds();				
				
			set_knob_function(KNOB_FUNCTION_AUTO);
				
			glcd_clear();
			main_update_display();
		}
	}*/
}

/*! \brief Parse a message from the communication bus */
void event_bus_parse_message(void) {
	BUS_MESSAGE bus_message = rx_queue_get();
	
	#ifdef DEBUG_WMV_BUS
		printf("DEBUG-> From addr: 0x%02X\n",bus_message.from_addr);
		printf("DEBUG-> To addr:   0x%02X\n",bus_message.to_addr);
		printf("DEBUG-> Command:   0x%02X\n",bus_message.cmd);
		printf("DEBUG-> Length:    0x%02X\n",bus_message.length);
	
		for (unsigned char i=0;i<bus_message.length;i++)
			printf("DEBUG-> Data #%02i:  0x%02X\n",i,bus_message.data[i]);
	#endif
	
	if (bus_message.cmd == BUS_CMD_ACK)
		bus_message_acked(bus_message.from_addr);
	else if (bus_message.cmd == BUS_CMD_NACK)
		bus_message_nacked(bus_message.from_addr, bus_message.data[0]);

	//Drop the message
	rx_queue_drop();
	
	#ifdef DEBUG_WMV_BUS
		printf("DEBUG-> Message dropped\n");
	#endif	
}

/*! \brief Parse an external event and perform the proper action
 *  \param ext_event_status The status of the external "hardware" event flags */
void event_parse_ext_event(unsigned int ext_event_status) {
	if (ext_event_status & (1<<STATUS_FOOTSWITCH_BIT)) {
		if (status.ext_devices_current_state & (1<<STATUS_FOOTSWITCH_BIT))
			sequencer_footsw_released();
		else
			sequencer_footsw_pressed();
	}
	
	if (ext_event_status & (1<<STATUS_USB2_RTS_BIT)) {
		if (status.ext_devices_current_state & (1<<STATUS_USB2_RTS_BIT)) {
			if (sequencer_get_rts_polarity() == 0)
				sequencer_computer_rts_activated();
			else
				sequencer_computer_rts_deactivated();
		}
		else {
			if (sequencer_get_rts_polarity() == 0)
				sequencer_computer_rts_deactivated();
			else
				sequencer_computer_rts_activated();
		}
	}
	
	if (sequencer_get_radio_sense() == 0) {
		if (ext_event_status & (1<<STATUS_RADIO_SENSE1_BIT)) {
			if (status.ext_devices_current_state & (1<<STATUS_RADIO_SENSE1_BIT)) {
				if (sequencer_get_sense_polarity() == 0)
					sequencer_radio_sense_deactivated();
				else
					sequencer_radio_sense_activated();
			}
			else {
				if (sequencer_get_sense_polarity() == 0)
					sequencer_radio_sense_activated();
				else
					sequencer_radio_sense_deactivated();
			}
		}	
	}
	else {
		if (ext_event_status & (1<<STATUS_RADIO_SENSE2_BIT)) {
			if (status.ext_devices_current_state & (1<<STATUS_RADIO_SENSE2_BIT)) {
				if (sequencer_get_sense_polarity() == 0)
					sequencer_radio_sense_deactivated();
				else
					sequencer_radio_sense_activated();
			}
			else {
				if (sequencer_get_sense_polarity() == 0)
					sequencer_radio_sense_activated();
				else
					sequencer_radio_sense_deactivated();
			}
		}
	}
}