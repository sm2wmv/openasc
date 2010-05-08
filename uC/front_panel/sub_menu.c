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

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//TODO: Fix the transmission of submenu output str out on the bus
//      For this we also need to figure out a way so that we can have multiple selected sub menu options on different
//      antennas at the same time. Maybe insert some antenna index which is saved in the driver units? Or just ignore 
//      using DEACTIVATE ALL commands?

//TODO: Fix the implementation if more than one antenna has got a sub menu, should be done in event_handler.c

struct_sub_menu_array current_sub_menu_array[4];

unsigned char curr_option_selected[4] = {0,0,0,0};

/*! \brief Load a set of sub menu from the EEPROM for a specific band
 *  \param band_index Which we band */
void sub_menu_load(unsigned char band_index) {
	unsigned char sub_menu_type[4];
	
	for (unsigned char i=0;i<4;i++) {
		sub_menu_type[i] = antenna_ctrl_get_sub_menu_type(i);
		eeprom_get_ant_sub_menu_array_structure(band_index, i, &current_sub_menu_array[i]);
	}
}

/*! \brief Get the text for the sub menu 
 *  \param ant_index The antenna index we wish to get the antenna text for
 *  \param pos Which sub menu position to show
 *  \return Returns the text of the sub menu antenna index */
unsigned char *sub_menu_get_text(unsigned char ant_index, unsigned char pos) {
	if (antenna_ctrl_get_sub_menu_type(ant_index) == SUBMENU_VERT_ARRAY) {
		printf("%s\n",current_sub_menu_array[ant_index].direction_name[pos]);
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


/*! \brief Send the output string for the rx antenna to the bus 
 *  \param index The index of the antenna you wish to send the string of */
void sub_menu_send_data_to_bus(unsigned char ant_index) {

}