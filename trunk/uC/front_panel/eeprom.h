/*! \file front_panel/eeprom.h 
 *  \brief EEPROM functions
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/eeprom.h" \endcode
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

#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "board.h"
#include "sequencer.h"
#include "main.h"
#include "antenna_ctrl.h"
#include "radio_interface.h"
#include "band_ctrl.h"

#define EEPROM_STARTUP_BYTE_ADDR 0x01

//! The EEPROM table
typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! The start address of the antenna structure in the EEPROM memory
	unsigned int antenna[9];
	//! The start address of the band structure in the EEPROM memory
	unsigned int band[9];
	//! The start address of the RX antenna structure in the EEPROM memory
	unsigned int rx_antennas;
	
	//! The start address of the setting structure in the EEPROM memory
	unsigned int settings;
	//! The start address of the radio settings structure in the EEPROM memory
	unsigned int radio_settings;
	//! The start address of the sequencer
	unsigned int struct_ptt;
	
	//! The sub menus of antenna 1
	unsigned int antenna1_sub_menu[9];
	//! The sub menus of antenna 2
	unsigned int antenna2_sub_menu[9];
	//! The sub menus of antenna 3
	unsigned int antenna3_sub_menu[9];
	//! The sub menus of antenna 4
	unsigned int antenna4_sub_menu[9];
	
	//! The sub menus of the rx antennas
	unsigned int rx_antenna_sub_menu[10];

	//! Runtime settings, such as backlight level etc
	unsigned int runtime_settings;
} struct_eeprom_table;

unsigned char eeprom_read_startup_byte(void);
void eeprom_write_startup_byte(unsigned char val);

//void eeprom_load_band(struct_band *ptr,unsigned char band);
void eeprom_read_table(void);
void eeprom_create_table(void);

void eeprom_save_runtime_settings(struct_runtime_settings *content);
void eeprom_get_runtime_settings(struct_runtime_settings *data);

void eeprom_get_antenna_data(struct_antenna *data, unsigned char band);
void eeprom_get_band_data(unsigned char band, struct_band *data);

void eeprom_get_ptt_data(struct_ptt *data);

void eeprom_save_ant_structure(unsigned char band_index, struct_antenna *content);

void eeprom_get_radio_settings_structure(struct_radio_settings *data);

void eeprom_get_rx_antenna_data(struct_rx_antennas *data);
void eeprom_save_rx_ant_structure(struct_rx_antennas *data);

void eeprom_save_radio_settings_structure(struct_radio_settings *data);
void eeprom_save_band_data(unsigned char band, struct_band *data);

void eeprom_get_settings_structure(struct_setting *data);
void eeprom_save_settings_structure(struct_setting *data);
void eeprom_save_ptt_data(struct_ptt *data);

void eeprom_save_ant_sub_menu_array_structure(unsigned char band_index, unsigned char ant_index, struct_sub_menu_array *data);

#endif