/*! \file front_panel/sub_menu.h
 *  \ingroup front_panel_group
 *  \brief Antenna sub menu functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-04-28
 *  \code #include "front_panel/sub_menu.h" \endcode
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

#ifndef _SUB_MENU_H_
#define _SUB_MENU_H_

#include "main.h"

//! Struct of a sub menu with the type array
typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! Number of directions
	unsigned char direction_count;
	//! The directions
	unsigned char direction_name[8][SUB_MENU_ARRAY_NAME_SIZE];
	//! The length of the output str
	unsigned char output_str_dir_length[8];
	//! The output strings of the different directions
	unsigned char output_str_dir[8][SUB_MENU_ARRAY_STR_SIZE];
} struct_sub_menu_array;

void sub_menu_load(unsigned char band_index);
unsigned char *sub_menu_get_text(unsigned char ant_index, unsigned char pos);

unsigned char sub_menu_get_count(void);
unsigned char sub_menu_get_current_pos(unsigned char ant_index);
void sub_menu_set_current_pos(unsigned char ant_index, unsigned char new_pos);
unsigned char sub_menu_get_type(unsigned char ant_index);

void sub_menu_pos_down(unsigned char ant_index);
void sub_menu_pos_up(unsigned char ant_index);

#endif