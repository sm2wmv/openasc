/*! \file menu.h \brief Menu system
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-03-01
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

#ifndef _MENU_H_
#define _MENU_H_

#include <avr/pgmspace.h>

typedef struct {
	char *text;
} struct_menu_option;

typedef struct {
	//! Position nr in the menu system
	unsigned char pos;
	//! Header text
	char *header;
	//! Pointer to the options
	struct_menu_option *options;
	//! Number of options
	unsigned char option_count;
	/*! Which kind of option
			0 = regular option
			1 = numbers
			2 = nothing
	*/
	unsigned char option_type;
} struct_menu_text;

#define MENU_OPTION_TYPE_NORMAL					0
#define MENU_OPTION_TYPE_SCROLL_NUMBERS	1
#define MENU_OPTION_TYPE_NONE						99

//! Show band change mode, auto or manual
#define MENU_POS_BAND_MODE				0
//! Show the radio output ptt ON/OFF
#define MENU_POS_RADIO_PTT				1
//! Show the amplifier output ptt ON/OFF
#define MENU_POS_AMP_PTT					2
//! Change the backlight level of the LCD
#define MENU_POS_BACKLIGHT_LEVEL	3
//! Show network actvitity
#define MENU_POS_SHOW_ACTIVITY		4
//! Show the error menu
#define MENU_POS_SHOW_ERRORS			5

#define MENU_SCROLL_UP			0
#define MENU_SCROLL_DOWN		1
#define MENU_BUTTON_PRESSED	2

void menu_show(void);
void menu_action(unsigned char menu_action_type);
void menu_init(void);
void menu_reset(void);

#endif