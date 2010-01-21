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

unsigned char screensaver_mode = 0;

char *temp_ptr = NULL;

/*! \brief Display the shutdown in progress screen */
void display_shutdown_view(void) {
	glcd_clear();
	display_text_center_adjust(10,"Shutdown",8, FONT_FIFTEEN_DOT);
	display_text_center_adjust(30,"in progress",11, FONT_FIFTEEN_DOT);
	glcd_update_all();
}

/*! \brief Display the setup  in progressscreen */
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

/*! \brief Updates the screensaver which consist of a clock */
void display_update_screensaver(void) {
	//If the temp_ptr hasn't been initialized then we need to allocate the memory for it
	if (temp_ptr == NULL)
		temp_ptr = (char *)malloc(8);

	if (screensaver_mode == 0)
		screensaver_mode = 1;

	ds1307_read();

	sprintf((char *)temp_ptr,"%02i:%02i:%02i",ds1307_get_hours(),ds1307_get_minutes(),ds1307_get_seconds());

	glcd_clear();
	glcd_text(8, 23, FONT_EIGHTEEN_DOT,temp_ptr,strlen(temp_ptr));
	glcd_update_all();

	if (screensaver_mode == 1)
		event_add_message((void*)display_update_screensaver, 250, 0);
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
		char temp[strlen(antenna_ctrl_get_antenna_text(0))+2];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(0));
		glcd_text(DISPLAY_TEXT_ANT1_X_POS, DISPLAY_TEXT_ANT1_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(0));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT1_X_POS, DISPLAY_TEXT_ANT1_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(0),antenna_ctrl_get_antenna_text_length(0));

	if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(1))+2];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(1));
		glcd_text(DISPLAY_TEXT_ANT2_X_POS, DISPLAY_TEXT_ANT2_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(1));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT2_X_POS, DISPLAY_TEXT_ANT2_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(1),antenna_ctrl_get_antenna_text_length(1));

	if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(2))+2];
		sprintf(temp,"(%s)",antenna_ctrl_get_antenna_text(2));
		glcd_text(DISPLAY_TEXT_ANT3_X_POS, DISPLAY_TEXT_ANT3_Y_POS, FONT_NINE_DOT,temp,antenna_ctrl_get_antenna_text_length(2));
	}
	else	
		glcd_text(DISPLAY_TEXT_ANT3_X_POS, DISPLAY_TEXT_ANT3_Y_POS, FONT_NINE_DOT,antenna_ctrl_get_antenna_text(2),antenna_ctrl_get_antenna_text_length(2));

	if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_IN_USE_FLAG)) {
		char temp[strlen(antenna_ctrl_get_antenna_text(3))+2];
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
	char temp_dir[7];
	
	if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(0));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT1_X_POS,DISPLAY_TEXT_ROTATOR_ANT1_Y_POS,temp_dir,strlen(temp_dir),FONT_FIVE_DOT);
	}
	
	if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(1));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT2_X_POS,DISPLAY_TEXT_ROTATOR_ANT2_Y_POS,temp_dir,strlen(temp_dir),FONT_FIVE_DOT);
	}

	if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(2));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT3_X_POS,DISPLAY_TEXT_ROTATOR_ANT3_Y_POS,temp_dir,strlen(temp_dir),FONT_FIVE_DOT);
	}

	if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_ROTATOR_FLAG)) {
		sprintf((char *)temp_dir,"%3i deg",antenna_ctrl_get_direction(3));
		display_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT4_X_POS,DISPLAY_TEXT_ROTATOR_ANT4_Y_POS,temp_dir,strlen(temp_dir),FONT_FIVE_DOT);
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
void display_radio_freq(char *freq) {
	CLEAR_RADIO_FREQ_AREA();
	display_text_right_adjust(DISPLAY_RADIO_FREQ_X_POS,DISPLAY_RADIO_FREQ_Y_POS,freq,strlen(freq),FONT_FIVE_DOT);
}

/**
 * \brief Displays a specified view
 * This is to display lines, icons etc depending on what kind of
 * of "view" you wish to display.
 * \param mode Which view mode you wish to display */
void display_view(unsigned char mode) {
	if (mode == VIEW_ANTENNAS) {
		glcd_line(0,128,56);
	}
}

/*! \brief Show the current selected RX ant 
 *  \param ant_index The antenna index of which antenna that is selected and should be shown 
 */
void display_show_rx_ant(unsigned char ant_index) {
	if (status.current_display_level == DISPLAY_LEVEL_BAND) {	
		CLEAR_RX_ANTENNA_AREA();

		if ((antenna_ctrl_get_rx_antenna_count() >= ant_index) && (ant_index != 0)) {
			char temp[strlen(antenna_ctrl_get_rx_antenna_name(ant_index-1))+4];
			
			sprintf((char *)temp, "RX: %s",antenna_ctrl_get_rx_antenna_name(ant_index-1));
			
			glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_FIVE_DOT, temp, strlen(temp));
		}
		else {
			display_view(VIEW_ANTENNAS);
		}
		
		glcd_update_all();
	}
}

/*! \brief Show SET rotator heading
 *  \param rotator_heading The current set rotator heading  */
void display_show_set_heading(unsigned int rotator_heading, unsigned char view_360_deg) {
	if (status.current_display_level == DISPLAY_LEVEL_BAND) {	
		glcd_clear();
		char temp[7];
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
}

void display_update_radio_freq(void) {
	if (status.current_display == CURRENT_DISPLAY_ANTENNA_INFO) {
		if ((radio_interface_get_interface() == RADIO_INTERFACE_CAT_POLL) | (radio_interface_get_interface() == RADIO_INTERFACE_CAT_MON)) {	
			char temp_str[10];
			sprintf((char *)temp_str, "%0i kHz",radio_get_current_freq());
						
			display_radio_freq((char *)temp_str);
			
			display_view(VIEW_ANTENNAS);
		}
		else
			CLEAR_RADIO_FREQ_AREA();
		
		glcd_update_all();
	}
}
