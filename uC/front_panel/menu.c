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
#include <avr/pgmspace.h>

#include "menu.h"
#include "board.h"
#include "glcd.h"
#include "fonts.h"
#include "radio_interface.h"
#include "errors.h"
#include "event_handler.h"
#include "led_control.h"
#include "display_handler.h"
#include "main.h"

//! Sets the intend from the left
#define MENU_OPTION_LEFT_POS 13

//! Number of options in the menu system
#define MENU_OPTIONS	10

prog_char error_0[] PROGMEM = "Bus resend";
prog_char error_1[] PROGMEM = "No bus sync";
prog_char error_2[] PROGMEM = "Bus TX queue full";
prog_char error_3[] PROGMEM = "Bus RX queue full";
prog_char error_4[] PROGMEM = "Int. comm resend";
prog_char error_5[] PROGMEM = "Ant drv timeout";
prog_char error_6[] PROGMEM = "Band drv timeout";
prog_char error_7[] PROGMEM = "High VSWR";
prog_char error_8[] PROGMEM = "Band in use";
prog_char error_9[] PROGMEM = "Int comm. TX full";
prog_char error_10[] PROGMEM = "Int comm. RX full";
prog_char error_11[] PROGMEM = "Event queue full";
prog_char error_12[] PROGMEM = "High ref power";
prog_char error_13[] PROGMEM = "PA Error";
prog_char error_14[] PROGMEM = "Rotator Error";

/*//! Menu options - Errors
const struct_menu_option menu_errors[NR_OF_ERRORS] = {
  {"Bus resend"},{"No bus sync"}, {"Bus TX queue full"}, {"Bus RX queue full"}, {"Int. comm resend"}, 
  {"Ant drv timeout"},{"Band drv timeout"},{"High VSWR"},{"Band in use"}, {"Int comm. TX full"},
  {"Int comm. RX full"},{"Event queue full"},{"High ref power"},{"PA Error"},{"Rotator Error"}
};*/

PROGMEM const char *menu_errors[] =  {error_0,error_1,error_2,error_3,error_4,error_5,error_6,error_7,error_8,error_9,error_10,error_11,error_12,error_13,error_14};


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
//! Menu system option - Amplifier ON/OFF
const struct_menu_option menu_option_amp_status[] = {{"ON"},{"OFF"}};

/********* SETUP MENUS *********/
prog_char header_1[] PROGMEM = "Band change";
prog_char header_2[] PROGMEM = "Radio PTT";
prog_char header_3[] PROGMEM = "Amplifier PTT";
prog_char header_4[] PROGMEM = "Backlight";
prog_char header_5[] PROGMEM = "Information";
prog_char header_6[] PROGMEM = "Miscellaneous";
prog_char header_7[] PROGMEM = "Powermeter";
prog_char header_8[] PROGMEM = "Antenna status";
prog_char header_9[] PROGMEM = "Power amplifier";
prog_char header_10[] PROGMEM = "Errors";

//! Menu system
const struct_menu_text menu_system_text[] = {
{MENU_POS_BAND_MODE, header_1, (struct_menu_option *)menu_option_band_selection_mode, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_RADIO_PTT, header_2, (struct_menu_option *)menu_option_radio_ptt_output, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_AMP_PTT, header_3, (struct_menu_option *)menu_option_amp_ptt_output, 2,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_BACKLIGHT_LEVEL, header_4, NULL, 0,MENU_OPTION_TYPE_SCROLL_NUMBERS},
{MENU_POS_SHOW_INFO, header_5, NULL, 0,MENU_OPTION_TYPE_NONE},
{MENU_POS_MISC, header_6, (struct_menu_option *)menu_misc, 1,MENU_OPTION_TYPE_NORMAL},
{MENU_POS_SHOW_POWERMETER_ADDR, header_7, NULL, 0,MENU_OPTION_TYPE_SCROLL_NUMBERS},
{MENU_POS_ANT_STATUS, header_8, NULL, 0, MENU_OPTION_TYPE_SCROLL_NUMBERS},
{MENU_POS_AMP_STATUS, header_9, (struct_menu_option *)menu_option_amp_status, 0, MENU_OPTION_TYPE_NORMAL},
{MENU_POS_SHOW_ERRORS, header_10, NULL, 0,MENU_OPTION_TYPE_NORMAL}
};

/*! \brief Show the text of a menu on the display 
 *  \param menu_text The menu which we wish to show */
void menu_show_text(struct_menu_text menu_text) {
  char temp_str[20];
  
  //Clear the display
	glcd_clear();
	
	if (menu_text.pos == MENU_POS_SHOW_ERRORS) {
		strcpy_P(temp_str,menu_text.header);
		glcd_text(0,0,FONT_NINE_DOT,temp_str,strlen(temp_str));
	
		//Draw a seperation line between the options and the header
		glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),12);
		glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),14);
		
		unsigned char count = 0;
		
		for (int i=0;i<NR_OF_ERRORS;i++) {
			if (error_handler_get_state(i)) {
        strcpy_P(temp_str, (char*)pgm_read_word(&(menu_errors[i])));
				glcd_text(MENU_OPTION_LEFT_POS,18+count*10,FONT_SEVEN_DOT,temp_str,strlen(temp_str));
				count++;
			}
		}
	}
	else if (menu_text.pos == MENU_POS_SHOW_INFO) {
    strcpy_P(temp_str,menu_text.header);
    glcd_text(0,0,FONT_NINE_DOT,temp_str,strlen(temp_str));
  
    //Draw a seperation line between the options and the header
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),12);
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),14);
		
    char temp[30];
    sprintf_P(temp,PSTR("Version: %s"),VERSION);
    glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
    
    sprintf_P(temp,PSTR("Address: %i"),bus_get_address());
    glcd_text(MENU_OPTION_LEFT_POS,18+10,FONT_SEVEN_DOT,temp,strlen(temp));
	}
	else if (menu_text.pos == MENU_POS_AMP_STATUS) {
    strcpy_P(temp_str,menu_text.header);
    glcd_text(0,0,FONT_NINE_DOT,temp_str,strlen(temp_str));
  
    //Draw a seperation line between the options and the header
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),12);
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),14);
    
    char temp[30];
    
    /* MAINS */
    if (status.amp_flags & (1<<AMP_STATUS_MAINS))
      strcpy_P(temp,PSTR("Mains: ON"));
    else
      strcpy_P(temp,PSTR("Mains: OFF"));

    glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
    

    /*! Operate/standby */
    if (status.amp_flags & (1<<AMP_STATUS_OPR_STBY))
      strcpy_P(temp,PSTR("Mode: Operate"));
    else
      strcpy_P(temp,PSTR("Mode: Standby"));

    glcd_text(MENU_OPTION_LEFT_POS,18+10,FONT_SEVEN_DOT,temp,strlen(temp));
    
    
    /*! Amp status */
    if (status.amp_op_status == AMP_OP_STATUS_READY)
      strcpy_P(temp,PSTR("Status: Ready"));
    else if (status.amp_op_status == AMP_OP_STATUS_ERROR)
      strcpy_P(temp,PSTR("Status: ERROR"));
    else if (status.amp_op_status == AMP_OP_STATUS_TUNING)
      strcpy_P(temp,PSTR("Status: Tuning"));
    else if (status.amp_op_status == AMP_OP_STATUS_OFF)
      strcpy_P(temp,PSTR("Status: Amp off"));
    else if (status.amp_op_status == AMP_OP_STATUS_WARMUP)
      strcpy_P(temp,PSTR("Status: Warmup"));
    else if (status.amp_op_status == AMP_OP_STATUS_COOLDOWN)
      strcpy_P(temp,PSTR("Status: Cooldown"));
    else
      strcpy_P(temp,PSTR("Status: Standby"));
    
    glcd_text(MENU_OPTION_LEFT_POS,18+10+10,FONT_SEVEN_DOT,temp,strlen(temp));
    
    /*! Amp band info */
    if (status.amp_band == BAND_UNDEFINED)
      strcpy_P(temp,PSTR("Band: None"));
    else if (status.amp_band == BAND_160M)
      strcpy_P(temp,PSTR("Band: 160m"));
    else if (status.amp_band == BAND_80M)
      strcpy_P(temp,PSTR("Band: 80m"));
    else if (status.amp_band == BAND_40M)
      strcpy_P(temp,PSTR("Band: 40m"));
    else if (status.amp_band == BAND_30M)
      strcpy_P(temp,PSTR("Band: 30m"));
    else if (status.amp_band == BAND_20M)
      strcpy_P(temp,PSTR("Band: 20m"));
    else if (status.amp_band == BAND_17M)
      strcpy_P(temp,PSTR("Band: 17m"));
    else if (status.amp_band == BAND_15M)
      strcpy_P(temp,PSTR("Band: 15m"));
    else if (status.amp_band == BAND_12M)
      strcpy_P(temp,PSTR("Band: 12m"));
    else if (status.amp_band == BAND_10M)
      strcpy_P(temp,PSTR("Band: 10m"));

    sprintf(temp+strlen(temp)," S%i",status.amp_segment);
    
    glcd_text(MENU_OPTION_LEFT_POS,18+10+10+10,FONT_SEVEN_DOT,temp,strlen(temp));
  }
	else {
    strcpy_P(temp_str,menu_text.header);
    glcd_text(0,0,FONT_NINE_DOT,temp_str,strlen(temp_str));
  
    //Draw a seperation line between the options and the header
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),12);
    glcd_line(0,display_handler_calculate_width(temp_str,FONT_NINE_DOT,strlen(temp_str)),14);
	
		if (menu_text.option_type == MENU_OPTION_TYPE_NORMAL) {
			//Print the different options, mark the current selected with an inverted background
			for (unsigned char i=0;i<menu_text.option_count;i++) {
				glcd_text(MENU_OPTION_LEFT_POS,18+i*10,FONT_SEVEN_DOT,menu_text.options[i].text,strlen(menu_text.options[i].text));
				
				if (current_menu_option_selected[current_menu_pos] == i) {
					glcd_text(0,18+i*10,FONT_SEVEN_DOT,"->",2);
				
					if (current_menu_level == 1) {
						glcd_invert_area(MENU_OPTION_LEFT_POS,display_handler_calculate_width(menu_text.options[i].text,FONT_SEVEN_DOT,strlen(menu_text.options[i].text))+MENU_OPTION_LEFT_POS,18+i*10-1,18+i*10+7);
					}
				}
			}
		}
		else if (menu_text.option_type == MENU_OPTION_TYPE_SCROLL_NUMBERS) {
			if (menu_text.pos == MENU_POS_BACKLIGHT_LEVEL) {
				char temp[11];
				sprintf_P(temp,PSTR("Level %3i"),runtime_settings.lcd_backlight_value);
	
				glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
				
				if (current_menu_level == 1)
					glcd_invert_area(MENU_OPTION_LEFT_POS,display_handler_calculate_width(temp,FONT_SEVEN_DOT,strlen(temp))+MENU_OPTION_LEFT_POS,18-1,18+7);
			}
			else if (menu_text.pos == MENU_POS_SHOW_POWERMETER_ADDR) {
				char temp[14];
				if (runtime_settings.powermeter_address == 0x00)
					sprintf_P(temp,PSTR("Address: AUTO"));
				else
					sprintf_P(temp,PSTR("Address: %3i"),runtime_settings.powermeter_address);
	
				glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
				
				if (current_menu_level == 1)
					glcd_invert_area(MENU_OPTION_LEFT_POS,display_handler_calculate_width(temp,FONT_SEVEN_DOT,strlen(temp))+MENU_OPTION_LEFT_POS,18-1,18+7);				
			}
			else if (menu_text.pos == MENU_POS_ANT_STATUS) {
        char temp[20];
        
        sprintf_P(temp,PSTR("Comb value: %3i"),runtime_settings.antenna_disabled[status.selected_band-1]);
  
        glcd_text(MENU_OPTION_LEFT_POS,18,FONT_SEVEN_DOT,temp,strlen(temp));
        
        if (current_menu_level == 1)
          glcd_invert_area(MENU_OPTION_LEFT_POS,display_handler_calculate_width(temp,FONT_SEVEN_DOT,strlen(temp))+MENU_OPTION_LEFT_POS,18-1,18+7);
        
        if (runtime_settings.antenna_disabled[status.selected_band-1] & (1<<0))
          sprintf_P(temp,PSTR("A1: Disabled"));
        else
          sprintf_P(temp,PSTR("A1: Enabled"));
        
        glcd_text(MENU_OPTION_LEFT_POS,18+12,FONT_SEVEN_DOT,temp,strlen(temp));

        if (runtime_settings.antenna_disabled[status.selected_band-1] & (1<<1))
          sprintf_P(temp,PSTR("A2: Disabled"));
        else
          sprintf_P(temp,PSTR("A2: Enabled"));
       
        glcd_text(MENU_OPTION_LEFT_POS,18+12+9,FONT_SEVEN_DOT,temp,strlen(temp));

        if (runtime_settings.antenna_disabled[status.selected_band-1] & (1<<2))
          sprintf_P(temp,PSTR("A3: Disabled"));
        else
          sprintf_P(temp,PSTR("A3: Enabled"));
        
        glcd_text(MENU_OPTION_LEFT_POS,18+12+18,FONT_SEVEN_DOT,temp,strlen(temp));

        if (runtime_settings.antenna_disabled[status.selected_band-1] & (1<<3))
          sprintf_P(temp,PSTR("A4: Disabled"));
        else
          sprintf_P(temp,PSTR("A4: Enabled"));
        
        glcd_text(MENU_OPTION_LEFT_POS,18+12+27,FONT_SEVEN_DOT,temp,strlen(temp));
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

					display_handler_set_backlight(runtime_settings.lcd_backlight_value);
				}
				else if (menu_system_text[current_menu_pos].pos == MENU_POS_SHOW_POWERMETER_ADDR) {
					if (runtime_settings.powermeter_address < 0xFF)
						runtime_settings.powermeter_address++;
				}
				else if (menu_system_text[current_menu_pos].pos == MENU_POS_ANT_STATUS) {
          if (status.selected_band != 0)
            if (runtime_settings.antenna_disabled[status.selected_band-1] < 15)
              runtime_settings.antenna_disabled[status.selected_band-1]++;
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

					display_handler_set_backlight(runtime_settings.lcd_backlight_value);
				}
				else if (menu_system_text[current_menu_pos].pos == MENU_POS_SHOW_POWERMETER_ADDR) {
					if (runtime_settings.powermeter_address > 0)
						runtime_settings.powermeter_address--;
				}
        else if (menu_system_text[current_menu_pos].pos == MENU_POS_ANT_STATUS) {
          if (status.selected_band != 0)
            if (runtime_settings.antenna_disabled[status.selected_band-1] > 0)
              runtime_settings.antenna_disabled[status.selected_band-1]--;
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
        
        main_update_status_field();
			}
			else if (current_menu_pos == MENU_POS_RADIO_PTT) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					runtime_settings.radio_ptt_output = 1;
				else
					runtime_settings.radio_ptt_output = 0;

        main_update_status_field();
			}
			else if (current_menu_pos == MENU_POS_MISC) {
				if (current_menu_option_selected[current_menu_pos] == 0)
					forceHardReset();
			}
		}
		
		menu_show(current_menu_pos);
	}
}

unsigned char menu_curr_pos(void) {
  return(current_menu_pos);
}

void menu_set_pos(unsigned char pos) {
  current_menu_pos = pos;
}