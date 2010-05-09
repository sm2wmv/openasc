/*! \file front_panel/sub_menu.c \brief Antenna sub menu functions
 *  \ingroup front_panel_group
 *  \brief Antenna sub menu functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-04-28
 *  \code #include "front_panel/sub_menu.c" \endcode
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
#include <string.h>

#include "sub_menu.h"
#include "antenna_ctrl.h"
#include "main.h"
#include "eeprom.h"
#include "band_ctrl.h"

#include "../global.h"
#include "../internal_comm.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//TODO: Fix the implementation if more than one antenna has got a sub menu, should be done in event_handler.c
//TODO: Maybe fix so that we are able to select RX antennas while in the sub menu of an ARRAY?

struct_sub_menu_array current_sub_menu_array[4];

unsigned char curr_option_selected[4] = {0,0,0,0};

//! Array which we store the current devices which we have activated antenna outputs on
unsigned char current_activated_sub_outputs[4][SUB_MENU_ARRAY_STR_SIZE];

//! How many devices we have activated antenna outputs on
unsigned char current_activated_sub_outputs_length[4] = {0,0,0,0};

/*! \brief Load a set of sub menu from the EEPROM for a specific band
 *  \param band_index Which we band */
void sub_menu_load(unsigned char band_index) {
	unsigned char sub_menu_type[4];
		
	for (unsigned char i=0;i<4;i++) {
		curr_option_selected[i] = 0;
		
		sub_menu_type[i] = antenna_ctrl_get_sub_menu_type(i);
		
		if (sub_menu_type[i] == SUBMENU_VERT_ARRAY)
			eeprom_get_ant_sub_menu_array_structure(band_index, i, &current_sub_menu_array[i]);
	}
}

/*! \brief Get the text for the sub menu 
 *  \param ant_index The antenna index we wish to get the antenna text for
 *  \param pos Which sub menu position to show
 *  \return Returns the text of the sub menu antenna index */
unsigned char *sub_menu_get_text(unsigned char ant_index, unsigned char pos) {
	if (antenna_ctrl_get_sub_menu_type(ant_index) == SUBMENU_VERT_ARRAY) {
		return(current_sub_menu_array[ant_index].direction_name[pos]);
	}
	
	return("   ");
}

unsigned char sub_menu_get_current_pos(unsigned char ant_index) {
	return(curr_option_selected[ant_index]);
}

void sub_menu_set_current_pos(unsigned char ant_index, unsigned char new_pos) {
	curr_option_selected[ant_index] = new_pos;
}

unsigned char sub_menu_get_count(void) {
	unsigned char temp = 0;
	
	for (unsigned char i=0;i<4;i++)
		if (antenna_ctrl_get_sub_menu_type(i) != SUBMENU_NONE)
			temp++;
	
	return(temp);
}

unsigned char sub_menu_get_type(unsigned char ant_index) {
	return(antenna_ctrl_get_sub_menu_type(ant_index));
}

void sub_menu_pos_down(unsigned char ant_index) {
	if (sub_menu_get_type(ant_index) == SUBMENU_VERT_ARRAY) {
		if (sub_menu_get_current_pos(ant_index) > 0)
			sub_menu_set_current_pos(ant_index, sub_menu_get_current_pos(ant_index)-1);
		else
			sub_menu_set_current_pos(ant_index,current_sub_menu_array[ant_index].direction_count-1);
	}
}

void sub_menu_pos_up(unsigned char ant_index) {
	if (sub_menu_get_type(ant_index) == SUBMENU_VERT_ARRAY) {
		if (sub_menu_get_current_pos(ant_index) < current_sub_menu_array[ant_index].direction_count - 1)
			sub_menu_set_current_pos(ant_index, sub_menu_get_current_pos(ant_index)+1);
		else
			sub_menu_set_current_pos(ant_index,0);
	}
}


/*! \brief Send the output string for the sub menu position to the bus 
 *  \param ant_index The index of the antenna you wish to send the string of 
 *  \param pos The sub menu position we wish to send the output str of */
void sub_menu_send_data_to_bus(unsigned char ant_index, unsigned char pos) {
	unsigned char activate_cmd, deactivate_cmd, deactivate_all_cmd;
	
	printf("SEND\n");
	
	unsigned char length = 0;
	
	switch (ant_index) {
		case 0:
			activate_cmd = BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT1_OUTPUT;
			deactivate_cmd = BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT1_OUTPUT;
			deactivate_all_cmd = BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT1_OUTPUTS;
			
			length = current_sub_menu_array[0].output_str_dir_length[pos];
			break;
		case 1:
			activate_cmd = BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT2_OUTPUT;
			deactivate_cmd = BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT2_OUTPUT;
			deactivate_all_cmd = BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT2_OUTPUTS;
			
			length = current_sub_menu_array[1].output_str_dir_length[pos];
			break;
		case 2:
			activate_cmd = BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT3_OUTPUT;
			deactivate_cmd = BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT3_OUTPUT;
			deactivate_all_cmd = BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT3_OUTPUTS;
			
			length = current_sub_menu_array[2].output_str_dir_length[pos];
			break;
		case 3:
			activate_cmd = BUS_CMD_DRIVER_ACTIVATE_SUBMENU_ANT4_OUTPUT;
			deactivate_cmd = BUS_CMD_DRIVER_DEACTIVATE_SUBMENU_ANT4_OUTPUT;
			deactivate_all_cmd = BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT4_OUTPUTS;
			
			length = current_sub_menu_array[3].output_str_dir_length[pos];
			break;
		default:
			break;
	}
	
	if (length > 0) {
		unsigned char temp[length];
	
		unsigned char start_pos = 0;
		unsigned char count = 0;
		unsigned char addr_count=0;

		unsigned char i=0;
			
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[ant_index], current_activated_sub_outputs_length[ant_index], deactivate_all_cmd);
		current_activated_sub_outputs_length[ant_index] = 0;
		
		while(i<length) {
			if (current_sub_menu_array[ant_index].output_str_dir[pos][i] == OUTPUT_ADDR_DELIMITER) {
				//Will add which address the message was sent to
				current_activated_sub_outputs[ant_index][addr_count++] = current_sub_menu_array[ant_index].output_str_dir[pos][i+1];
				
				if (current_sub_menu_array[ant_index].output_str_dir[pos][i+1] != 0x00)
					bus_add_tx_message(bus_get_address(), current_sub_menu_array[ant_index].output_str_dir[pos][i+1], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), activate_cmd, count-start_pos, temp+start_pos);
				else
					internal_comm_add_tx_message(activate_cmd,count-start_pos, (char *)(temp+start_pos));

				start_pos += count;
				i++;
			} 
			else {
				temp[count] = current_sub_menu_array[ant_index].output_str_dir[pos][i];
				count++;
			}
			
			i++;
		}
		
		current_activated_sub_outputs_length[ant_index] = addr_count;
	}
	else if (current_activated_sub_outputs_length[ant_index] > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[ant_index], current_activated_sub_outputs_length[ant_index], deactivate_all_cmd);
		current_activated_sub_outputs_length[ant_index] = 0;
	}
}

void sub_menu_deactivate_all(void) {
	if (current_activated_sub_outputs_length[0] > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[0], current_activated_sub_outputs_length[0], BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT1_OUTPUTS);
		current_activated_sub_outputs_length[0] = 0;
	}
	
	if (current_activated_sub_outputs_length[1] > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[1], current_activated_sub_outputs_length[1], BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT2_OUTPUTS);
		current_activated_sub_outputs_length[1] = 0;
	}
	
	if (current_activated_sub_outputs_length[2] > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[2], current_activated_sub_outputs_length[2], BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT3_OUTPUTS);
		current_activated_sub_outputs_length[2] = 0;
	}
	
	if (current_activated_sub_outputs_length[3] > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_sub_outputs[3], current_activated_sub_outputs_length[3], BUS_CMD_DRIVER_DEACTIVATE_ALL_SUBMENU_ANT4_OUTPUTS);
		current_activated_sub_outputs_length[3] = 0;
	}
}

void sub_menu_activate_all(void) {
	//Activate all the sub menu options avaible for default position #0
	if (antenna_ctrl_get_sub_menu_type(0) != SUBMENU_NONE)
		sub_menu_send_data_to_bus(0,0);
	
	if (antenna_ctrl_get_sub_menu_type(1) != SUBMENU_NONE)	
		sub_menu_send_data_to_bus(1,0);
	
	if (antenna_ctrl_get_sub_menu_type(2) != SUBMENU_NONE)	
		sub_menu_send_data_to_bus(2,0);
	
	if (antenna_ctrl_get_sub_menu_type(3) != SUBMENU_NONE)
		sub_menu_send_data_to_bus(3,0);
}