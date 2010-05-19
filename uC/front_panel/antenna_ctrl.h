/*! \file front_panel/antenna_ctrl.h
 *  \ingroup front_panel_group
 *  \brief Antenna control functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/antenna_ctrl.h" \endcode
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

#ifndef _ANTENNA_CTRL_H_
#define _ANTENNA_CTRL_H_
#include "main.h"

//! The rotator is currently standing still
#define FLAG_NO_ROTATION					1
//! The rotator is allowed to be rotated
#define FLAG_ROTATION_ALLOWED			2
//! The rotator is being rotated CW
#define FLAG_ROTATION_CW					3
//! The rotator is being rotated CCW
#define FLAG_ROTATION_CCW					4

//! Struct which contains information of the rx antennas
typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! The length of the antenna names
	unsigned char name_length[10];
	//! RX antenna name
  char name[10][RX_ANTENNA_NAME_LENGTH];
	//! RX antenna output str length
	unsigned char output_length[10];
	//! RX antenna output str
  char output_str[10][RX_ANTENNA_OUTPUT_STR_LENGTH];
	//! The length of the band output data
	unsigned char band_output_length[4];
	//! Band output str
	char band_output_str[4][RX_ANTENNA_BAND_OUTPUT_STR_LENGTH];
} struct_rx_antennas;

//! Structure of an antenna
typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! The type of sub menu it is
	unsigned char sub_menu_type[4];
	//! The length of the text for the antennas
	unsigned char antenna_text_length[4];	
	//! The text for the antennas
	char antenna_text[4][ANTENNA_TEXT_SIZE];
	//! Antenna flags
	unsigned char antenna_flag[4];
	//! The direction of the antennas
	int antenna_direction[4];
	/*! This is used to show what antenna combinations that are allowed
	 *  Bit0  = ANT 1
	 *  Bit1  = ANT 2
	 *  Bit2  = ANT 3
	 *  Bit3  = ANT 4
	 *  Bit4  = ANT 1 + ANT 2
	 *  Bit5  = ANT 1 + ANT 3
	 *  Bit6  = ANT 1 + ANT 4
	 *  Bit7  = ANT 2 + ANT 3
	 *  Bit8  = ANT 2 + ANT 4
	 *  Bit9  = ANT 3 + ANT 4
	 *  Bit10 = ANT 1 + ANT 2 + ANT 3
	 *  Bit11 = ANT 1 + ANT 2 + ANT 4
	 *  Bit12 = ANT 1 + ANT 3 + ANT 4
	 *  Bit13 = ANT 2 + ANT 3 + ANT 4
	 *  Bit14 = ANT 1 + ANT 2 + ANT 3 + ANT 4
   */
	unsigned int antenna_comb_allowed;
		//! The length of the antenna output strings
	unsigned char antenna_output_length[15];
	/*! The antenna output strings which contains what outputs that should be 
	 *  activated when the antenna combination is chosen */
	unsigned char antenna_comb_output_str[15][ANTENNA_OUTPUT_COMB_SIZE];
	//!The address to the rotator which controls the antenna
	unsigned char rotator_addr[4];
	//!The SUB address to the rotator which controls the antenna
	unsigned char rotator_sub_addr[4];
	//!The number of degrees the rotator can turn, this might be for example 450 degrees for YAESU
	unsigned int rotator_max_rotation[4];
	//!The minimum heading of the rotator, this can also be negative numbers if starting point is not at 0 degrees
	unsigned int rotator_min_heading[4];
	//!The delay from a rotation has occured to it can start to rotate again (in seconds)
	unsigned char rotator_delay[4];
	//! The rotator flags
	unsigned char rotator_flags[4];
	//! Does the rotator have 360 degree view? Should it show 0-360 degree or start_point + rotation, maybe 90 - 500 deg
	unsigned char rotator_view_360_deg;
	//! The default antenna index (0-3)
	unsigned char default_antenna;
} struct_antenna;

void antenna_ctrl_deactivate_all_rx_band(void);

void antenna_ctrl_send_ant_data_to_bus(void);
void antenna_ctrl_send_rx_ant_data_to_bus(unsigned char antenna_index);
void antenna_ctrl_send_rx_ant_band_data_to_bus(char index);

unsigned char antenna_ctrl_comb_allowed(unsigned char antenna_comb);
unsigned char antenna_ctrl_get_comb_value(unsigned char antenna_comb);

void antenna_ctrl_set_antenna_text(char *str, unsigned char index);
char* antenna_ctrl_get_antenna_text(unsigned char index);

unsigned char antenna_ctrl_get_antenna_text_length(unsigned char index);

void antenna_ctrl_set_output_comb(unsigned char *data, unsigned char index, unsigned char length);
unsigned char* antenna_ctrl_get_output_comb(unsigned char index);
unsigned char antenna_ctrl_get_output_comb_length(unsigned char index);

void antenna_ctrl_set_direction(unsigned int dir, unsigned char index);
unsigned int antenna_ctrl_get_direction(unsigned char index);

void antenna_ctrl_set_flags(unsigned char flags, unsigned char index);
unsigned char antenna_ctrl_get_flags(unsigned char index);

void antenna_ctrl_set_comb_allowed(unsigned int comb);
unsigned int antenna_ctrl_get_comb_allowed(void);

void antenna_ctrl_change_rx_ant(unsigned char ant_index);

void antenna_ctrl_set_antenna_data(struct_antenna *data);
void antenna_ctrl_set_rx_antenna_data(struct_rx_antennas *data);
unsigned char antenna_ctrl_get_rx_antenna_count(void);

char* antenna_ctrl_get_rx_antenna_name(unsigned char ant_index);
char* antenna_ctrl_get_rx_antenna_output_str(unsigned char ant_index);

void antenna_ctrl_ant_read_eeprom(unsigned char band_index);
void antenna_ctrl_rx_ant_read_eeprom(void);

void antenna_ctrl_deactivate_outputs(unsigned char *addresses, unsigned char length, unsigned char cmd);

unsigned char antenna_ctrl_get_sub_menu_type(unsigned char ant_index);

void antenna_ctrl_deactivate_all(void);

void antenna_ctrl_rotate(unsigned char ant_index, unsigned int heading);
unsigned char antenna_ctrl_antenna_selected(void);
unsigned char antenna_ctrl_get_rotatable(void);
unsigned int antenna_ctrl_get_start_heading(unsigned char ant_index);
unsigned int antenna_ctrl_get_max_rotation(unsigned char ant_index);

unsigned char antenna_ctrl_get_rotator_addr(unsigned char ant_index);

void antenna_ctrl_set_rotator_flags(unsigned char ant_index,unsigned char flags);
unsigned char antenna_ctrl_get_rotator_flags(unsigned char ant_index);

void antenna_ctrl_select_default_ant(void);

#endif