/*! \file front_panel/menu.c 
 *  \brief Menu system
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/menu.c " \endcode
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
#include <avr/interrupt.h>
#include <string.h>

#include "menu.h"
#include "board.h"
#include "glcd.h"
#include "fonts.h"
#include "display.h"
#include "radio_interface.h"
#include "errors.h"
#include "event_handler.h"
#include "led_control.h"

//! Sets the intend from the left
#define MENU_OPTION_LEFT_POS 13

//! Number of options in the menu system
#define MENU_OPTIONS	8

//! Menu options - Errors
const struct_menu_option menu_errors[] = {{"Bus resend"},{"No bus sync"}, {"Bus TX queue full"}, {"Bus RX queue full"}, {"Int. comm resend"}, {"Ant drv timeout"},{"Band drv timeout"},{"High VSWR"},{"Band in use"}};


const struct_menu_option menu_misc[] = {{"Reboot"}};
//! The current selected menu option
unsigned char current_menu_option_selected[MENU_OPTIONS];

//! Flag to indicate which menu level we are on
unsigned char current_menu_level = 0;
//! Flag to indicate the current menu position
unsigned char current_menu_pos   = 0;

/********* SETUP MENU OPTIONS *********/
//! Menu system option - band selection mode
const struct_menu_option menu_option_band_selection_mode[] = {{"Manual"},{"Auto"}};
//! Menu system option - amp ptt output
const struct_menu_option menu_option_amp_ptt_output[] = {{"ON"},{"OFF"}};
//! Menu system option - radio ptt output
const struct_menu_option menu_option_radio_ptt_output[] = {{"ON"},{"OFF"}};
/********* SETUP MENUS *********/
//! Menu system
const struct_menu_text menu_system_text[] = {
{MENU_POS_BAND_MODE, "Band change", (struct_menu_option *)menu_option_band_selection_mode, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_RADIO_PTT, "Radio PTT", (struct_menu_option *)menu_option_radio_ptt_output, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_AMP_PTT, "Amplifier PTT", (struct_menu_option *)menu_option_amp_ptt_output, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_BACKLIGHT_LEVEL, "Backlight", NULL, 0,MENU_OPTION_TYPE_SCROLL_NUMBERS},
{MENU_POS_SHOW_FIRMWARE, "Firmware", NULL, 0,MENU_OPTION_TYPE_NONE},
{MENU_POS_MISC, "Miscellaneous", (struct_menu_option *)menu_misc, 1,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_SHOW_POWERMETER_ADDR, "Powermeter", NULL, 0,MENU_OPTION_TYPE_SCROLL_NUMBERS},
{MENU_POS_SHOW_ERRORS, "Errors", (struct_menu_option *)menu_errors, 0,MENU_OPTION_TYPE_NORMAL},
};

/*! \brief Show the text of a menu on the display 
 *  \param menu_text The menu which we wish to show */
void menu_show_text(struct_menu_text menu_text) {
	//Clear the display
	glcd_clear();
	
	if (menu_text.pos == MENU_POS_SHOW_ERRORS) {
		//Show the "Menu" text
		glcd_text(0,0,FONT_NINE_DOT,menu_text.header,strlen(menu_text.header));
	
		//Draw a seperation line between the options and the header
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),12);
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),14);
		
		unsigned char count = 0;
		
		for (int i=0;i<NR_OF_ERRORS;i++) {
			if (error_handler_get_state(i)) {
				glcd_text(MENU_OPTION_LEFT_POS,18+count*10,FONT_SEVEN_DOT,menu_errors[i].text,strlen(menu_errors[i].text));
				count++;
			}
		}
	}
	else if (menu_text.pos == MENU_POS_SHOW_FIRMWARE) {
		glcd_text(0,0,FONT_NINE_DOT,menu_text.header,strlen(menu_text.header));
		
		//Draw a seperation line between the options and the header
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),12);
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),14);
		
    char temp[30];
    sprintf(temp,"Version: %s",FIRMWARE_REV);
    glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
	}
	else {
		//Show the "Menu" text
		glcd_text(0,0,FONT_NINE_DOT,menu_text.header,strlen(menu_text.header));
		
		//Draw a seperation line between the options and the header
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),12);
		glcd_line(0,display_calculate_width(menu_text.header,FONT_NINE_DOT,strlen(menu_text.header)),14);
	
		if (menu_text.option_type == MENU_OPTION_TYPE_NORMAL) {
			//Print the different options, mark the current selected with an inverted background
			for (unsigned char i=0;i<menu_text.option_count;i++) {
				glcd_text(MENU_OPTION_LEFT_POS,18+i*10,FONT_SEVEN_DOT,menu_text.options[i].text,strlen(menu_text.options[i].text));
				
				if (current_menu_option_selected[current_menu_pos] == i) {
					glcd_text(0,18+i*10,FONT_SEVEN_DOT,"->",2);
				
					if (current_menu_level == 1) {
						glcd_invert_area(MENU_OPTION_LEFT_POS,display_calculate_width(menu_text.options[i].text,FONT_SEVEN_DOT,strlen(menu_text.options[i].text))+MENU_OPTION_LEFT_POS,18+i*10-1,18+i*10+7);
					}
				}
			}
		}
		else if (menu_text.option_type == MENU_OPTION_TYPE_SCROLL_NUMBERS) {
			if (menu_text.pos == MENU_POS_BACKLIGHT_LEVEL) {
				char temp[11];
				sprintf(temp,"Level %3i",runtime_settings.lcd_backlight_value);
	
				glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
				
				if (current_menu_level == 1)
					glcd_invert_area(MENU_OPTION_LEFT_POS,display_calculate_width(temp,FONT_SEVEN_DOT,strlen(temp))+MENU_OPTION_LEFT_POS,18-1,18+7);
			}
			else if (menu_text.pos == MENU_POS_SHOW_POWERMETER_ADDR) {
				char temp[14];
				if (runtime_settings.powermeter_address == 0x00)
					sprintf(temp,"Address: AUTO");
				else
					sprintf(temp,"Address: %3i",runtime_settings.powermeter_address);
	
				glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
				
				if (current_menu_level == 1)
					glcd_invert_area(MENU_OPTION_LEFT_POS,display_calculate_width(temp,FONT_SEVEN_DOT,strlen(temp))+MENU_OPTION_LEFT_POS,18-1,18+7);				
			}
		}
	}
	
	glcd_update_all();
}

/*! \brief Initialize the menu system */
void menu_init(void) {
	if (runtime_settings.band_change_mode == BAND_CHANGE_MODE_MANUAL)
		current_menu_option_selected[MENU_POS_BAND_MODE] = 0;
	else
		current_menu_option_selected[MENU_POS_BAND_MODE] = 1;

	if (runtime_settings.radio_ptt_output == 1)
		current_menu_option_selected[MENU_POS_RADIO_PTT] = 0;
	else
		current_menu_option_selected[MENU_POS_RADIO_PTT] = 1;

	if (runtime_settings.amplifier_ptt_output == 1)
		current_menu_option_selected[MENU_POS_AMP_PTT] = 0;
	else
		current_menu_option_selected[MENU_POS_AMP_PTT] = 1;
}

//! Function will reset to init values, like menu level etc
void menu_reset(void) {
	current_menu_pos = 0;
	current_menu_level = 0;
}

/*! \brief Shows the menu */
void menu_show(unsigned char pos) {
	current_menu_pos = pos;
	
	menu_show_text(menu_system_text[current_menu_pos]);
}

/*! This function will handle an menu action "event"
 *  \param  menu_action_type Which action did occur? */
void menu_action(unsigned char menu_action_type) {
	if (menu_action_type == MENU_SCROLL_UP) {
		if (current_menu_level == 0) {
			if (current_menu_pos < (MENU_OPTIONS - 1))
				current_menu_pos++;
			else
				current_menu_pos = 0;
		}
		else if (current_menu_level == 1) {
			if (menu_system_text[current_menu_pos].option_type == MENU_OPTION_TYPE_NORMAL) {
				if (current_menu_option_selected[current_menu_pos] < (menu_system_text[current_menu_pos].option_count - 1))
					current_menu_option_selected[current_menu_pos]++;
			}
			else if (menu_system_text[current_menu_pos].option_type == MENU_OPTION_TYPE_SCROLL_NUMBERS) {
				if (menu_system_text[current_menu_pos].pos == MENU_POS_BACKLIGHT_LEVEL) {
					if (runtime_settings.lcd_backlight_value < 100)
						runtime_settings.lcd_backlight_value++;

					display_set_backlight(runtime_settings.lcd_backlight_value);
				}
				else if (menu_system_text[current_menu_pos].pos == MENU_POS_SHOW_POWERMETER_ADDR) {
					if (runtime_settings.powermeter_address < 0xFF)
						runtime_settings.powermeter_address++;
				}
			}
		}
		
		menu_show(current_menu_pos);
	}
	else if (menu_action_type == MENU_SCROLL_DOWN) {
		if (current_menu_level == 0) {
			if (current_menu_pos > 0)
				current_menu_pos--;
			else
				current_menu_pos = MENU_OPTIONS-1;
		}
		else if (current_menu_level == 1) {
			if (menu_system_text[current_menu_pos].option_type == MENU_OPTION_TYPE_NORMAL) {
				if (current_menu_option_selected[current_menu_pos] > 0)
					current_menu_option_selected[current_menu_pos]--;
			}
			else if (menu_system_text[current_menu_pos].option_type == MENU_OPTION_TYPE_SCROLL_NUMBERS) {
				if (menu_system_text[current_menu_pos].pos == MENU_POS_BACKLIGHT_LEVEL) {
					if (runtime_settings.lcd_backlight_value > 0)
						runtime_settings.lcd_backlight_value--;

					display_set_backlight(runtime_settings.lcd_backlight_value);
				}
				else if (menu_system_text[current_menu_pos].pos == MENU_POS_SHOW_POWERMETER_ADDR) {
					if (runtime_settings.powermeter_address > 0)
						runtime_settings.powermeter_address--;
				}
			}
		}
		
		menu_show(current_menu_pos);
	}
	else if (menu_action_type == MENU_BUTTON_PRESSED) {
		if (current_menu_level == 0) {
			if (current_menu_pos == MENU_POS_SHOW_ERRORS) {
				error_handler_clear_all();
				
				led_set_error(LED_STATE_OFF);
			}	
			else if (menu_system_text[current_menu_pos].option_type != MENU_OPTION_TYPE_NONE)
				current_menu_level = 1;
		}
		else {
			current_menu_level = 0;
			
			if (current_menu_pos == MENU_POS_BAND_MODE) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					runtime_settings.band_change_mode = BAND_CHANGE_MODE_MANUAL;
				else if (current_menu_option_selected[current_menu_pos] == 1) {
					runtime_settings.band_change_mode = BAND_CHANGE_MODE_AUTO;
				}
					
				set_knob_function(KNOB_FUNCTION_AUTO);
			}
			else if (current_menu_pos == MENU_POS_AMP_PTT) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					runtime_settings.amplifier_ptt_output = 1;
				else
					runtime_settings.amplifier_ptt_output = 0;
			}
			else if (current_menu_pos == MENU_POS_RADIO_PTT) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					runtime_settings.radio_ptt_output = 1;
				else
					runtime_settings.radio_ptt_output = 0;
			}
			else if (current_menu_pos == MENU_POS_MISC) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					bootloader_start();
			}
		}
		
		menu_show(current_menu_pos);
	}
}