/*! \file 4cx1500b/ctrl_box_front/display.c 
 *  \ingroup amp_front_panel_group 
 *  \brief The serial interface to configure the device and control it
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/display.c" \endcode
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
#include "../../global.h"
#include "main.h"
#include "ds1307.h"

//! Flag which indicates if the screensaver is activated or not
unsigned char screensaver_mode = 0;

//! The last forward value, used for the power meter bargraph update
unsigned char last_fwd_val = 0;

//! The last reflected value, used for the power meter bargraph update
unsigned char last_ref_val = 0;

//! Memory area used for printing variables to the display
char *temp_ptr = NULL;

char power_temp_str[7];

//! Index 0 is when PTT is not active, index 1 is when the PTT is active
static unsigned char display_current_view[2];

/*! \brief Display the shutdown in progress screen */
void display_shutdown_view(void) {
	glcd_clear();
	display_text_center_adjust(10,"Shutdown",8, FONT_FIFTEEN_DOT);
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
}

/*! \brief Set the mode of the screensaver to enable
 */
void display_enable_screensaver(void) {
	screensaver_mode = 1;
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

void display_update(void) {
  //If the PTT is active
  if (main_get_ptt_status() != 0) {
    
  }
  else {  //PTT is not active
    
  }
}

/*! \brief This function should be called each ms */
void display_1ms_tick(void) {
  
}