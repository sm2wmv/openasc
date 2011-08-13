/*! \file front_panel/band_ctrl.c
 *  \ingroup front_panel_group 
 *  \brief Band control functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/band_ctrl.c" \endcode
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

#include "band_ctrl.h"
#include "main.h"
#include "eeprom.h"
#include "led_control.h"
#include "radio_interface.h"
#include "sub_menu.h"
#include "display.h"
#include "../global.h"
#include "../event_queue.h"

#include "../internal_comm.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//! \brief Contains the current band information
struct_band current_band;

//! Array which we use to keep track of which devices we have been activating outputs on
unsigned char current_band_activated_outputs[BAND_OUTPUT_STR_SIZE];
//! The number of devices we have activated outputs on
unsigned char current_band_activated_outputs_length = 0;

/*! \brief Function which goes through the different addresses of cards that have been activated and compares it to the sent in address
    \param addr The address we wish to check if it is activated 
    \return 1 if the address is in use, 0 if not */
unsigned char band_ctrl_check_address_in_use(unsigned char addr) {
	unsigned char ret_val = 0;
	
	for (unsigned char i=0;i<current_band_activated_outputs_length;i++) {
		if (current_band_activated_outputs[i] == addr)
			ret_val = 1;
	}
	
	return(ret_val);
}

/*! \brief Send the output string for the current band to the bus */
void band_ctrl_send_band_data_to_bus(unsigned char band_portion) {
	if (status.selected_band != BAND_UNDEFINED) {
		unsigned char length = 0;
		
		if (band_portion == BAND_HIGH)
			length = current_band.band_high_output_str_length;
		else
			length = current_band.band_low_output_str_length;
		
		unsigned char temp[length];

		unsigned char addr = 0;
		unsigned char start_pos = 0;
		unsigned char count = 0;
		unsigned char addr_count = 0;

		unsigned char i=0;

		band_ctrl_deactivate_all();

		while(i<length) {
			if (band_portion == BAND_HIGH) {
				if (current_band.band_high_output_str[i] == OUTPUT_ADDR_DELIMITER) {
					addr = current_band.band_high_output_str[i+1];

					current_band_activated_outputs[addr_count++] = addr;
					current_band_activated_outputs_length++;

					if (addr != 0x00) {
						bus_add_tx_message(bus_get_address(), addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, temp+start_pos);
					}
					else {									
						internal_comm_add_tx_message(BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, (char *)(temp+start_pos));
					}
					
					start_pos += count;
					i++;
				}
				else {
					temp[count] = current_band.band_high_output_str[i];
					count++;
				}
				
				i++;
			}
			else {
				if (current_band.band_low_output_str[i] == OUTPUT_ADDR_DELIMITER) {
					addr = current_band.band_low_output_str[i+1];
					
					current_band_activated_outputs[addr_count++] = addr;
					current_band_activated_outputs_length++;

					if (addr != 0x00) {
						bus_add_tx_message(bus_get_address(), addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, temp+start_pos);
					}
					else {
						internal_comm_add_tx_message(BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, (char *)(temp+start_pos));
					}
					
					start_pos = count;
					i++;
				}
				else {
					temp[count] = current_band.band_low_output_str[i];
					count++;
				}
				
				i++;
			}
		}
	}
	else
		band_ctrl_deactivate_all();
}

/*! \brief Function will load a band from the EEPROM into the current_band struct
 *  \param band The index of the band we wish to load from the EEPROM */
void band_ctrl_load_band(unsigned char band) {
	//Retrieve the band data from the EEPROM
	eeprom_get_band_data(band,&current_band);
	
	//Load the antenna settings for this band
	antenna_ctrl_ant_read_eeprom(band);
	
	//Load the sub menus for this band
	sub_menu_load(band);
}

/*! \brief Function will send out new band portion settings for the current selected band *
    \param band_portion The current band portion */
void band_ctrl_change_band_portion(unsigned char band_portion) {
	band_ctrl_send_band_data_to_bus(band_portion);
}

/*! \brief Function used to change band
 *  \param band The band we wish to change to */
void band_ctrl_change_band(unsigned char band) {
	if (main_get_inhibit_state() != INHIBIT_NOT_OK_TO_SEND_RADIO_TX) {
    main_set_inhibit_state(INHIBIT_NOT_OK_TO_SEND);
    led_set_ptt(LED_STATE_PTT_INHIBIT);
    
    //Check if the band change is allowed
		if (main_band_change_ok(band) == 1) {
			status.selected_ant = 0;
			
			status.selected_band = band;
			status.new_band = band;
			
			led_set_band_none();
			led_set_band(band);
			
			//Turn off antenna leds
			for (unsigned char i=0;i<4;i++) {
				led_set_tx_ant(i,LED_STATE_OFF);
				led_set_rx_ant(i,LED_STATE_OFF);
			}
			
			if (band != BAND_UNDEFINED) {
				band_ctrl_load_band(band);
				
				if (status.current_display != CURRENT_DISPLAY_MENU_SYSTEM) {
					status.prev_display = status.current_display;
          status.current_display = CURRENT_DISPLAY_ANTENNA_INFO;
					status.current_display_level = DISPLAY_LEVEL_BAND;
				}
			}
			else {
				if ((status.current_display != CURRENT_DISPLAY_SHUTDOWN_VIEW) && (status.current_display != CURRENT_DISPLAY_MENU_SYSTEM))
          status.prev_display = status.current_display;
					status.current_display = CURRENT_DISPLAY_LOGO;
			}		
			
			antenna_ctrl_deactivate_all_rx_band();
			antenna_ctrl_deactivate_all();
			band_ctrl_deactivate_all();
		
			sub_menu_deactivate_all();
			led_set_submenu(LED_STATE_OFF);
			led_set_rxant(LED_STATE_OFF);
			set_knob_function(KNOB_FUNCTION_AUTO);
			
			status.selected_rx_antenna = 0;
			antenna_ctrl_change_rx_ant(status.selected_rx_antenna);
			status.function_status &= ~(1<<FUNC_STATUS_RXANT);
			
			if (band != BAND_UNDEFINED) {
				band_ctrl_send_band_data_to_bus(status.current_band_portion);
				
				//Send RX antenna band data to the bus
				antenna_ctrl_send_rx_ant_band_data_to_bus(band);
				antenna_ctrl_select_default_ant();
        
				//Activate all default sub menu options
				sub_menu_activate_all();
			}
      
			//Update the display
			event_add_message((void*)main_update_display,200,0);
		}
		else {
      //Lets change to band undefined, as long as we are not allowed in on the band we want
      main_set_new_band(BAND_UNDEFINED);
    }
	}
}

/*! \brief Function which will deactiavate all band outptus, BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS */
void band_ctrl_deactivate_all(void) {
	if (current_band_activated_outputs_length > 0) {
		antenna_ctrl_deactivate_outputs(current_band_activated_outputs, current_band_activated_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS);
		current_band_activated_outputs_length = 0;
	}
}

/*! \brief Loads the band limits into the band limits struct */
void band_ctrl_load_band_limits(void) {
	struct_band temp_band;
	
	for (unsigned char i=0;i<9;i++) {
		//Retrieve the band data from the EEPROM
		eeprom_get_band_data(i+1,&temp_band);
				
		band_limits[i].low_portion_low_limit = temp_band.low_portion_low_limit;
		band_limits[i].low_portion_high_limit = temp_band.low_portion_high_limit;
		band_limits[i].high_portion_low_limit = temp_band.high_portion_low_limit;
		band_limits[i].high_portion_high_limit = temp_band.high_portion_high_limit;
	}
}

/*! \brief Retrieve the lower frequency limit of the low band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_low_portion_low(unsigned char band) {
	return(band_limits[band-1].low_portion_low_limit);
}

/*! \brief Retrieve the higher frequency limit of the low band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_low_portion_high(unsigned char band) {
	return(band_limits[band-1].low_portion_high_limit);
}

/*! \brief Retrieve the lower frequency limit of the high band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_high_portion_low(unsigned char band) {
	return(band_limits[band-1].high_portion_low_limit);
}

/*! \brief Retrieve the higher frequency limit of the high band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_high_portion_high(unsigned char band) {
	return(band_limits[band-1].high_portion_high_limit);
}

/*! \brief Retrieve the higher frequency output string, of max length BAND_OUTPUT_STR_SIZE
 *	\return The output string */
unsigned char *band_ctrl_get_high_output_str(void) {
	return(current_band.band_high_output_str);
}

/*! \brief Retrieve the lower frequency output string, of max length BAND_OUTPUT_STR_SIZE
 *	\return The output string */
unsigned char *band_ctrl_get_low_output_str(void) {
	return(current_band.band_low_output_str);
}

/*! \brief Retrieve which band portion we are currently at 
 *  \return BAND_LOW, BAND_HIGH or BAND_UNDEFINED */
unsigned char band_ctrl_get_portion(void) {
	if ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_MANUAL) || ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_AUTO) && (radio_interface_get_interface() == RADIO_INTERFACE_BCD))) {
		if (status.current_band_portion == BAND_HIGH)
			return(BAND_HIGH);
		else if (status.current_band_portion == BAND_LOW)
			return(BAND_LOW);
		else
			return(BAND_UNDEFINED);
	}
	else if (runtime_settings.band_change_mode == BAND_CHANGE_MODE_AUTO) {
		return(radio_get_band_portion());
	}

	return(BAND_UNDEFINED);
}