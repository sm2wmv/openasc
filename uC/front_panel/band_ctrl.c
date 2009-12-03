/*! \file band_ctrl.c \brief Band control functions
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-01-22
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
#include "../global.h"


/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//! Contains the current band information
struct_band current_band;

unsigned char current_band_activated_outputs[BAND_OUTPUT_STR_SIZE];
unsigned char current_band_activated_outputs_length = 0;

/*! Send the output string for the current band to the bus */
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

					bus_add_tx_message(bus_get_address(), addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, temp+start_pos);
	
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

					bus_add_tx_message(bus_get_address(), addr, (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_BAND_OUTPUT, count-start_pos, temp+start_pos);
	
					start_pos += count;
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

void band_ctrl_load_band(unsigned char band) {
	//Retrieve the band data from the EEPROM
	eeprom_get_band_data(band,&current_band);
	
	printf("LOW_LOW %i\n",current_band.low_portion_low_limit);
	printf("LOW_HIGH %i\n",current_band.low_portion_high_limit);
	printf("HIGH_LOW %i\n",current_band.high_portion_low_limit);
	printf("HIGH_HIGH %i\n",current_band.high_portion_high_limit);
	
	printf("high_len: %i\n",current_band.band_high_output_str_length);
	printf("low_len: %i\n",current_band.band_low_output_str_length);
}

void band_ctrl_change_band(unsigned char band) {
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
		band_ctrl_load_band(band-1);
		antenna_ctrl_ant_read_eeprom(band);
		status.current_display = CURRENT_DISPLAY_ANTENNA_INFO;
	}
	else
		status.current_display = CURRENT_DISPLAY_LOGO;
	
	antenna_ctrl_deactivate_all_rx_band();
	antenna_ctrl_deactivate_all();
	band_ctrl_deactivate_all();
	
	if (band != BAND_UNDEFINED) {
		//TODO: Change to correct band portion
		band_ctrl_send_band_data_to_bus(BAND_HIGH);
		
		//Set RX antenna band data to the bus
		antenna_ctrl_send_rx_ant_band_data_to_bus(band);
	}
	
	//Update the display
	main_update_display();	
}

void band_ctrl_deactivate_all(void) {
	antenna_ctrl_deactivate_outputs(current_band_activated_outputs, current_band_activated_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_BAND_OUTPUTS);
	current_band_activated_outputs_length = 0;
}

/*! \brief Retrieve the lower frequency limit of the low band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_low_portion_low(void) {
	return(current_band.low_portion_low_limit);
}

/*! \brief Retrieve the higher frequency limit of the low band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_low_portion_high(void) {
	return(current_band.low_portion_high_limit);
}

/*! \brief Retrieve the lower frequency limit of the high band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_high_portion_low(void) {
	return(current_band.high_portion_low_limit);
}

/*! \brief Retrieve the higher frequency limit of the high band limit 
 *	\return The frequency in kHz */
unsigned int band_ctrl_get_high_portion_high(void) {
	return(current_band.high_portion_high_limit);
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

unsigned char band_ctrl_get_portion(void) {
	//TODO: Add support for others as well not only radio
	return(radio_get_band_portion());
}