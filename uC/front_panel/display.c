/*! \file front_panel/display.c 
 *  \ingroup front_panel_group 
 *  \brief The serial interface to configure the device and control it
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/display.c" \endcode
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
#include <string.h>
#include <avr/io.h>

#include "display.h"
#include "glcd.h"
#include "fonts.h"
#include "../global.h"
#include "main.h"
#include "ds1307.h"
#include "antenna_ctrl.h"
#include "radio_interface.h"
#include "sub_menu.h"

//! Flag which indicates if the screensaver is activated or not
unsigned char screensaver_mode = 0;

//! The last forward value, used for the power meter bargraph update
unsigned char last_fwd_val = 0;

//! The last reflected value, used for the power meter bargraph update
unsigned char last_ref_val = 0;

//! Memory area used for printing variables to the display
char *temp_ptr = NULL;

char power_temp_str[7];

/*! \brief Display the shutdown in progress screen */
void display_shutdown_view(void) {
	glcd_clear();
	display_text_center_adjust(10,"Shutdown",8, FONT_FIFTEEN_DOT);
	display_text_center_adjust(30,"in progress",11, FONT_FIFTEEN_DOT);
	glcd_update_all();
}

/*! \brief Display the setup  in progress screen */
void display_setup_view(void) {
	glcd_clear();
	display_text_center_adjust(10,"Setup",5, FONT_FIFTEEN_DOT);
	display_text_center_adjust(30,"in progress",11, FONT_FIFTEEN_DOT);
	glcd_update_all();
}

/*! \brief Get the mode of the screensaver 
 * \return 0 if the screensaver is disabled, 1 otherwise 
 */
unsigned char display_screensaver_mode(void) {
	return(screensaver_mode);
}

/*! \brief Set the mode of the screensaver to disabled
 */
void display_disable_screensaver(void) {
	screensaver_mode = 0;
  
  status.current_display = status.prev_display;
  status.prev_display = CURRENT_DISPLAY_SCREENSAVER;
}

/*! \brief Set the mode of the screensaver to enable
 */
void display_enable_screensaver(void) {
	screensaver_mode = 1;
  
  status.prev_display = status.current_display;
  status.current_display = CURRENT_DISPLAY_SCREENSAVER;
}

/*! \brief Updates the screensaver which consist of a clock */
void display_update_screensaver(void) {
	if (screensaver_mode == 1) {
		//If the temp_ptr hasn't been initialized then we need to allocate the memory for it
		if (temp_ptr == NULL)
			temp_ptr = (char *)malloc(9);
	
		ds1307_read();
	
		sprintf((char *)temp_ptr,"%02i:%02i:%02i",ds1307_get_hours(),ds1307_get_minutes(),ds1307_get_seconds());
	
		glcd_clear();
		glcd_text(8, 23, FONT_EIGHTEEN_DOT,temp_ptr,strlen(temp_ptr));
		glcd_update_all();
		
		event_add_message((void*)display_update_screensaver, 980, 0);
	}
}

/** \brief Set the backlight level of the LCD
  * \param value What we wish to set the backlight level to to, 0-100%
	*/
void display_set_backlight(unsigned char value) {
	//Set the pulse width
	OCR1CH = 0x00;
	OCR1CL = (2.55f * value);
}

/**
 * \brief Retrieve the width of a string in pixels
 *
 * \param str The text string you wish to find out the length of
 * \param font Which font type the string is
 * \param length The length of the string (strlen)
 * \return The actual graphical width of the text string sent in, in pixels
 */
unsigned char display_calculate_width(char *str, unsigned char font, unsigned char length) {
	unsigned char width = 0;
	
	for (unsigned char pos=0;pos<length;pos++)
	{
		if (fonts[font].fixed_width != 0)
			width += fonts[font].fixed_width;
		else
			width += fonts[font].width_table[(char)*(str+pos)-fonts[font].glyph_beg];
	}
	
	return (width);
}


/**
 * \brief Displays a piece of text center adjusted on the display
 * \param y Where the text should be located in y-axis (pixels)
 * \param str The string we wish to center adjust to the display
 * \param length The length of the string (strlen)
 * \param font Which font you wish to show the string with */
void display_text_center_adjust(unsigned char y,char *str, unsigned char length, unsigned char font) {
	glcd_text(64-display_calculate_width(str, font, length)/2, y, font,str,length);
}

/**
 * \brief Displays a piece of text right adjusted on the display
 * \param x Where the right adjust should start (pixels)
 * \param y Where the text should be located in y-axis (pixels)
 * \param length The length of the string (strlen)
 * \param font Which font you wish to show the string with */
void display_text_right_adjust(unsigned char x, unsigned char y,char *str, unsigned char length, unsigned char font) {
	glcd_text(x-display_calculate_width(str, font, length), y, font,str,length);
}

/**
 * \brief Display a set of antennas on the display
 * \param band The band you wish to show the antennas from */
void display_antennas(unsigned char band) {
	CLEAR_ANT_AREA(); //Clears the antenna field of the display
	
	//Check if the antenna is in use by another device, in that case
	//we indicate that on the display. Otherwise we just print as usual
	if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(0))+3];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(0));
		glcd_text(DISPLAY_TEXT_ANT1_X_POS, DISPLAY_TEXT_ANT1_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(0));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT1_X_POS, DISPLAY_TEXT_ANT1_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(0),antenna_ctrl_get_antenna_text_length(0));

	if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(1))+3];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(1));
		glcd_text(DISPLAY_TEXT_ANT2_X_POS, DISPLAY_TEXT_ANT2_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(1));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT2_X_POS, DISPLAY_TEXT_ANT2_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(1),antenna_ctrl_get_antenna_text_length(1));

	if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(2))+3];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(2));
		glcd_text(DISPLAY_TEXT_ANT3_X_POS, DISPLAY_TEXT_ANT3_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(2));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT3_X_POS, DISPLAY_TEXT_ANT3_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(2),antenna_ctrl_get_antenna_text_length(2));

	if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(3))+3];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(3));
		glcd_text(DISPLAY_TEXT_ANT4_X_POS, DISPLAY_TEXT_ANT4_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(3));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT4_X_POS, DISPLAY_TEXT_ANT4_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(3),antenna_ctrl_get_antenna_text_length(3));
}

/**
 * \brief Display the current rotator directions
 * If the rotator option has been enabled for a certain antenna
 * it will be shown it's current direction on the LCD
 * \param band The band you wish to show the rotators direction */
void display_rotator_directions(unsigned char band) {
	CLEAR_ROTATOR_AREA();
	char temp_dir[9];
	
	if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(0));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT1_X_POS,DISPLAY_TEXT_ROTATOR_ANT1_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}
	
	if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(1));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT2_X_POS,DISPLAY_TEXT_ROTATOR_ANT2_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}

	if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(2));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT3_X_POS,DISPLAY_TEXT_ROTATOR_ANT3_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}

	if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(3));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT4_X_POS,DISPLAY_TEXT_ROTATOR_ANT4_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}
	
	if (antenna_ctrl_get_sub_menu_type(0) == SUBMENU_VERT_ARRAY) {
		//sub_menu_get_text(0, sub_menu_get_current_pos(0))
		sprintf((char *)temp_dir,"Dir %s",sub_menu_get_text(0, sub_menu_get_current_pos(0)));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT1_X_POS,DISPLAY_TEXT_ROTATOR_ANT1_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}
	
	if (antenna_ctrl_get_sub_menu_type(1) == SUBMENU_VERT_ARRAY) {
		sprintf((char *)temp_dir,"Dir %s",sub_menu_get_text(1, sub_menu_get_current_pos(1)));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT2_X_POS,DISPLAY_TEXT_ROTATOR_ANT2_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}
	
	if (antenna_ctrl_get_sub_menu_type(2) == SUBMENU_VERT_ARRAY) {
		sprintf((char *)temp_dir,"Dir %s",sub_menu_get_text(2, sub_menu_get_current_pos(2)));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT3_X_POS,DISPLAY_TEXT_ROTATOR_ANT3_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}
	
	if (antenna_ctrl_get_sub_menu_type(3) == SUBMENU_VERT_ARRAY) {
		sprintf((char *)temp_dir,"Dir %s",sub_menu_get_text(3, sub_menu_get_current_pos(3)));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT4_X_POS,DISPLAY_TEXT_ROTATOR_ANT4_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
	}	
}

/**
 * \brief Displays an antenna but inverted
 *
 * \param ant_index Which antenna you wish to invert */
void display_invert_antenna(unsigned char ant_index) {
	unsigned char x2=0,y1=0,y2=0;
	
	//This function works only with characters with width = 8
	if (ant_index == ANTENNA_1) {
		y1 = DISPLAY_TEXT_ANT1_Y_POS;
		y2 = DISPLAY_TEXT_ANT1_Y_POS+DISPLAY_TEXT_ANT_HEIGHT;
		x2 = antenna_ctrl_get_antenna_text_length(0)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
		
		if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_IN_USE_FLAG))
			x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
	} 
	else if (ant_index == ANTENNA_2) {
		y1 = DISPLAY_TEXT_ANT2_Y_POS;
		y2 = DISPLAY_TEXT_ANT2_Y_POS+DISPLAY_TEXT_ANT_HEIGHT;
		x2 = antenna_ctrl_get_antenna_text_length(1)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
		
		if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_IN_USE_FLAG))
			x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
	}
	else if (ant_index == ANTENNA_3) {
		y1 = DISPLAY_TEXT_ANT3_Y_POS;
		y2 = DISPLAY_TEXT_ANT3_Y_POS+DISPLAY_TEXT_ANT_HEIGHT;
		x2 = antenna_ctrl_get_antenna_text_length(2)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
		
		if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_IN_USE_FLAG))
			x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
	}
	else if (ant_index == ANTENNA_4) {
		y1 = DISPLAY_TEXT_ANT4_Y_POS;
		y2 = DISPLAY_TEXT_ANT4_Y_POS+DISPLAY_TEXT_ANT_HEIGHT;
		x2 = antenna_ctrl_get_antenna_text_length(3)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
		
		if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_IN_USE_FLAG))
			x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
	}
	else return;

	glcd_invert_area(0,x2,y1,y2);
}

/**
 * \brief Displays the radios frequency
 * Will display the radios frequency in the bottom right corner of the display
 * \param freq the frequency you want to display */
void display_radio_freq(unsigned char length, char *freq) {
	CLEAR_RADIO_FREQ_AREA();
	display_text_right_adjust(DISPLAY_RADIO_FREQ_X_POS,DISPLAY_RADIO_FREQ_Y_POS,freq,length,FONT_FIVE_DOT);
}

/**
 * \brief Displays a specified view
 * This is to display lines, icons etc depending on what kind of
 * of "view" you wish to display.
 * \param mode Which view mode you wish to display */
void display_view(unsigned char mode) {
	if (mode == VIEW_ANTENNAS) {
		glcd_line(0,128,55);
	}
}

/*! \brief Show the current selected RX ant 
 *  \param ant_index The antenna index of which antenna that is selected and should be shown  */
void display_show_rx_ant(unsigned char ant_index) {
	CLEAR_RX_ANTENNA_AREA();
	
  if ((status.current_display_level == DISPLAY_LEVEL_BAND) | (status.current_display_level == DISPLAY_LEVEL_SUBMENU)) {	

		if ((antenna_ctrl_get_rx_antenna_count() >= ant_index) && (ant_index != 0)) {
			char temp[strlen(antenna_ctrl_get_rx_antenna_name(ant_index-1))+5];
			
			sprintf((char *)temp, "RX: %s",antenna_ctrl_get_rx_antenna_name(ant_index-1));
			
			glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
		}
		
		if (status.current_display_level == DISPLAY_LEVEL_BAND) {
			display_view(VIEW_ANTENNAS);
		}
		
		glcd_update_all();
	}
}

/*! \brief Show SET rotator heading
 *  \param rotator_heading The current set rotator heading  
 *  \param view_360_deg The status of the view_360_deg option */
void display_show_set_heading(unsigned int rotator_heading, unsigned char view_360_deg) {
	if (status.current_display_level == DISPLAY_LEVEL_BAND) {	
		glcd_clear();
		char temp[8];
		char size = 0;
		
		if (view_360_deg == 1) {
			if (rotator_heading >= 360)
				rotator_heading -= 360;
		}
		
		if (rotator_heading < 10)
			size = 1;
		else if (rotator_heading < 100)
			size = 2;
		else
			size = 3;
				
		sprintf((char *)temp, "Heading");
		display_text_center_adjust(0, temp, strlen(temp), FONT_FIFTEEN_DOT);
		
		sprintf((char *)temp, "%i",rotator_heading);
		display_text_center_adjust(20, temp, size, FONT_MT1);
		
		glcd_update_all();
	}
}

/*! \brief Updates the display 
 * \param band Which band to show the antenna information from
 * \param antenna The antenna combination that is currently selected
 */
void display_update(unsigned char band, unsigned char antenna) {
	if (status.current_display_level == DISPLAY_LEVEL_BAND) {
		display_antennas(band);
		display_rotator_directions(band);
		
		display_update_radio_freq();
		
		display_view(VIEW_ANTENNAS);
		
		if (antenna & (1<<0))
			display_invert_antenna(ANTENNA_1);

		if (antenna & (1<<1))
			display_invert_antenna(ANTENNA_2);

		if (antenna & (1<<2))
			display_invert_antenna(ANTENNA_3);

		if (antenna & (1<<3))
			display_invert_antenna(ANTENNA_4);
		
		glcd_update_all();
	}
	else if (status.current_display_level == DISPLAY_LEVEL_SUBMENU) {
		display_show_sub_menu(antenna-1,sub_menu_get_type(antenna-1));
	}
}

/*! \brief Update the radio frequency area of the display */
void display_update_radio_freq(void) {
	if (status.current_display == CURRENT_DISPLAY_ANTENNA_INFO) {
		if ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_AUTO) && (radio_interface_get_interface() != RADIO_INTERFACE_BCD)) {
			if ((radio_interface_get_interface() == RADIO_INTERFACE_CAT_POLL) | (radio_interface_get_interface() == RADIO_INTERFACE_CAT_MON)) {
				char temp_str[15];
				char temp_bandport = '?';
				
				unsigned char temp_port = radio_get_band_portion();
				
				if (temp_port == BAND_HIGH)
					temp_bandport = 'H';
				else if (temp_port == BAND_LOW)
					temp_bandport = 'L';
					
				sprintf((char *)temp_str, "%0i kHz (%c)",radio_get_current_freq(), temp_bandport);
							
				display_radio_freq(strlen(temp_str),(char *)temp_str);
				
				display_view(VIEW_ANTENNAS);
			}
			else
				CLEAR_RADIO_FREQ_AREA();
		}
		else if ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_MANUAL) || ((runtime_settings.band_change_mode == BAND_CHANGE_MODE_AUTO) && (radio_interface_get_interface() == RADIO_INTERFACE_BCD))) {
			char temp_str[4];
			unsigned char temp_nr_to_view = 0;
			
			if (status.current_band_portion == BAND_HIGH) {
				temp_str[0] = 'H';
				temp_str[1] = 'I';
				temp_str[2] = 'G';
				temp_str[3] = 'H';
				
				temp_nr_to_view = 4;
			}
			else if (status.current_band_portion == BAND_LOW) {
				temp_str[0] = 'L';
				temp_str[1] = 'O';
				temp_str[2] = 'W';
				
				temp_nr_to_view = 3;
			}
			
			display_radio_freq(temp_nr_to_view,(char *)temp_str);
			display_view(VIEW_ANTENNAS);
		}
		
		glcd_update_all();
	}
}

/*! \brief Will show the sub menu of a certain antenna 
 *  \param ant_index The antenna index (0-3)
 *  \param sub_menu_type Which type of sub menu it is */
void display_show_sub_menu(unsigned char ant_index, unsigned char sub_menu_type) {
	if (status.current_display_level == DISPLAY_LEVEL_SUBMENU) {
		char temp[12];
		
		if (sub_menu_type == SUBMENU_VERT_ARRAY) {
			CLEAR_SET_SUB_MENU_ARRAY_AREA();

			sprintf((char *)temp, "%s",antenna_ctrl_get_antenna_text(ant_index));
			display_text_center_adjust(3, temp, strlen(temp), FONT_NINE_DOT);
			
			sprintf((char *)temp, "Direction");
			display_text_center_adjust(19, temp, strlen(temp), FONT_NINE_DOT);
			
			sprintf((char *)temp, "%s",sub_menu_get_text(ant_index, sub_menu_get_current_pos(ant_index)));
			display_text_center_adjust(38, temp, strlen(temp), FONT_FIFTEEN_DOT);
			
			if (status.selected_rx_antenna != 0) {
				CLEAR_RX_ANTENNA_AREA();

				if ((antenna_ctrl_get_rx_antenna_count() >= status.selected_rx_antenna) && (status.selected_rx_antenna != 0)) {
					char temp[strlen(antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1))+5];
			
					sprintf((char *)temp, "RX: %s",antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1));
			
					glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
				}
			}
			
			glcd_update_all();
		}
		else if (sub_menu_type == SUBMENU_STACK) {
			CLEAR_SET_SUB_MENU_STACK_AREA();
			
			sprintf((char *)temp, "%s",antenna_ctrl_get_antenna_text(ant_index));
			display_text_center_adjust(3, temp, strlen(temp), FONT_NINE_DOT);
			
			sprintf((char *)temp, "combination");
			display_text_center_adjust(19, temp, strlen(temp), FONT_NINE_DOT);
			
			sprintf((char *)temp, "%s",sub_menu_get_text(ant_index, sub_menu_get_current_pos(ant_index)));
			display_text_center_adjust(38, temp, strlen(temp), FONT_FIFTEEN_DOT);
			
			if (status.selected_rx_antenna != 0) {
				CLEAR_RX_ANTENNA_AREA();

				if ((antenna_ctrl_get_rx_antenna_count() >= status.selected_rx_antenna) && (status.selected_rx_antenna != 0)) {
					char temp[strlen(antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1))+5];
			
					sprintf((char *)temp, "RX: %s",antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1));
			
					glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
				}
			}
			
			glcd_update_all();
		}
	}
}

/*! \brief Will display the forward bargraph
 *  \param percent How much we wish to fill it */
void display_show_bargraph_fwd(unsigned char percent) {
	unsigned char val = (unsigned char)(((float)percent*122.0f)/100.0);
	
	if (last_fwd_val > val) {
		for (unsigned char i=val;i<last_fwd_val;i++) {
			glcd_set_byte(3+i,2,0x00);

			if ((i%5) == 0) {
				glcd_set_byte(3+i,2,0x00);
			}
		}
		
		glcd_update_area(3+val,last_fwd_val+3,16,24);
	}
	else {
		for (unsigned char i=last_fwd_val;i<val;i++) {
			glcd_set_byte(3+i,2,0xFF);

			if ((i%5) == 0) {
				glcd_set_byte(3+i,2,0x00);
			}
		}
	
		glcd_update_area(3+last_fwd_val,3+val,16,24);
	}
	
	last_fwd_val = val;
}

/*! \brief Will display the reflected bargraph
 *  \param percent How much we wish to fill it */
void display_show_bargraph_ref(unsigned char percent) {
	unsigned char val = (unsigned char)(((float)percent*77.0f)/100.0);
	
	if (last_ref_val > val) {
		for (unsigned char i=val;i<last_ref_val;i++) {
			glcd_set_byte(3+i,6,0x00);

			if ((i%5) == 0) {
				glcd_set_byte(3+i,6,0x00);
			}
		}
		
		glcd_update_area(3+val,last_ref_val+3,48,56);
	}
	else {
		for (unsigned char i=last_ref_val;i<val;i++) {
			glcd_set_byte(3+i,6,0xFF);

			if ((i%5) == 0) {
				glcd_set_byte(3+i,6,0x00);
			}
		}
	
		glcd_update_area(3+last_ref_val,3+val,48,56);
	}
	
	last_ref_val = val;
}

/*! \brief Show the power meter bargraphs 
 *  \param fwd_power The forward power in percent
 *  \param ref_power The reflected power in percent */
void display_show_powermeter_bargraph(unsigned int fwd_power, unsigned int ref_power) {
	display_show_bargraph_fwd(fwd_power);
	display_show_bargraph_ref(ref_power);
}

/*! \brief This function will print out the power meter text which shows FWD, REF power and VSWR
 *  \param fwd_power Forward power in watts
 *  \param ref_power Reflected power in watts
 *  \param vswr The current VSWR, for example 151 means 1.51:1 */
void display_show_powermeter_text(unsigned int fwd_power, unsigned int ref_power, unsigned int vswr) {
	sprintf(power_temp_str,"%5iW",fwd_power);
	display_text_right_adjust(125,3,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
	sprintf(power_temp_str,"%3iW",ref_power);
	display_text_right_adjust(80,34,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
	
	if (vswr != 0) {
		sprintf(power_temp_str,"%i.%01i:1",(vswr/100),(vswr%100));
		display_text_right_adjust(125,45,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
	}
	else //If we have VSWR as 0 we just clear that area instead of writing out 0.0:1
		display_text_right_adjust(125,45,"     ",5,FONT_NINE_DOT);
}

/*! \brief This function will show the power meter display */
void display_show_powermeter(void) {
	last_fwd_val = 0;
	last_ref_val = 0;
	
	glcd_rectangle(0,0,28,128);
	glcd_rectangle(0,32,28,128);

	glcd_text(3,3,FONT_NINE_DOT,"FWD:",4);
	glcd_text(3,34,FONT_NINE_DOT,"REF:",4);
	display_text_right_adjust(125,34,"VSWR",4,FONT_NINE_DOT);
}