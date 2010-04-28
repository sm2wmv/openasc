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

struct_sub_menu_array current_sub_menu;

/*! \brief Load a sub menu from the EEPROM
 *  \param band_index Which we band
 *  \param ant_index Which antenna 
 *  \return The kind of sub meny type it is */
unsigned char sub_menu_load(unsigned char band_index, unsigned char ant_index) {
	unsigned char sub_menu_type = antenna_ctrl_get_sub_menu_type(ant_index);
	
	if (sub_menu_type == SUBMENU_VERT_ARRAY) {
		eeprom_get_ant_sub_menu_array_structure(band_index, ant_index, &current_sub_menu);
	}
	
	return(sub_menu_type);
}