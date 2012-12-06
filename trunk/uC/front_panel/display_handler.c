/*! \file front_panel/display_handler.c 
 *  \ingroup front_panel_group 
 *  \brief Display handler
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-08-14
 *  \code #include "front_panel/display_handler.c" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

#include "display_handler.h"
#include "glcd.h"
#include "fonts.h"
#include "../global.h"
#include "main.h"
#include "ds1307.h"
#include "antenna_ctrl.h"
#include "radio_interface.h"
#include "sub_menu.h"
#include "powermeter.h"
#include "menu.h"
#include "board.h"

display_handler_status_struct display_handler_status;

static char power_temp_str[7];

static char status_field_text[25];

//! Memory area used for printing variables to the display
static char temp_time_ptr[9];

//! The last forward value, used for the power meter bargraph update
unsigned char prev_fwd_val = 0;
//! The last reflected value, used for the power meter bargraph update
unsigned char prev_ref_val = 0;

/*! \brief Get the mode of the screensaver 
 * \return 0 if the screensaver is disabled, 1 otherwise 
 */
unsigned char display_handler_screensaver_mode(void) {
  return(display_handler_status.screensaver_mode);
}

/*! \brief Set the mode of the screensaver to disabled */
void display_handler_disable_screensaver(void) {
  display_handler_status.screensaver_mode = 0;
  
  display_handler_prev_view();
}

/*! \brief Set the mode of the screensaver to enable
 */
void display_handler_enable_screensaver(void) {
  display_handler_status.screensaver_mode = 1;

  display_handler_new_view(DISPLAY_HANDLER_VIEW_SCREENSAVER);
}

/*! \brief Updates the screensaver which consist of a clock */
void display_handler_update_screensaver(void) {
  if (display_handler_status.screensaver_mode == 1) {
    ds1307_read();
  
    sprintf_P((char *)temp_time_ptr,PSTR("%02i:%02i:%02i"),ds1307_get_hours(),ds1307_get_minutes(),ds1307_get_seconds());
  
    glcd_clear();
    glcd_text(8, 23, FONT_EIGHTEEN_DOT,temp_time_ptr,strlen(temp_time_ptr));
    glcd_update_all();
    
    event_add_message((void*)display_handler_update_screensaver, 980, 0);
  }
}

void display_handler_set_backlight(unsigned char value) {
  //Set the pulse width
  OCR1CH = 0x00;
  OCR1CL = (2.55f * value);
}

/**
 * \brief Displays a piece of text right adjusted on the display
 * \param x Where the right adjust should start (pixels)
 * \param y Where the text should be located in y-axis (pixels)
 * \param length The length of the string (strlen)
 * \param font Which font you wish to show the string with */
void display_handler_text_right_adjust(unsigned char x, unsigned char y,char *str, unsigned char length, unsigned char font) {
  glcd_text(x-display_handler_calculate_width(str, font, length), y, font,str,length);
}

/**
 * \brief Retrieve the width of a string in pixels
 *
 * \param str The text string you wish to find out the length of
 * \param font Which font type the string is
 * \param length The length of the string (strlen)
 * \return The actual graphical width of the text string sent in, in pixels
 */
unsigned char display_handler_calculate_width(char *str, unsigned char font, unsigned char length) {
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
void display_handler_text_center_adjust(unsigned char y,char *str, unsigned char length, unsigned char font) {
  glcd_text(64-display_handler_calculate_width(str, font, length)/2, y, font,str,length);
}

/*! \brief Will display the forward bargraph
 *  \param percent How much we wish to fill it */
void display_handler_show_bargraph_fwd(unsigned char percent) {
  unsigned char val = (unsigned char)(((float)percent*122.0f)/100.0);
  
  if (prev_fwd_val > val) {
    for (unsigned char i=val;i<prev_fwd_val;i++) {
      glcd_set_byte(3+i,2,0x00);

      if ((i%5) == 0) {
        glcd_set_byte(3+i,2,0x00);
      }
    }
    
    glcd_update_area(3+val,prev_fwd_val+3,16,24);
  }
  else {
    for (unsigned char i=prev_fwd_val;i<val;i++) {
      glcd_set_byte(3+i,2,0xFF);

      if ((i%5) == 0) {
        glcd_set_byte(3+i,2,0x00);
      }
    }
  
    glcd_update_area(3+prev_fwd_val,3+val,16,24);
  }
  
  prev_fwd_val = val;
}

/*! \brief Will display the reflected bargraph
 *  \param percent How much we wish to fill it */
void display_handler_show_bargraph_ref(unsigned char percent) {
  unsigned char val = (unsigned char)(((float)percent*77.0f)/100.0);
  
  if (prev_ref_val > val) {
    for (unsigned char i=val;i<prev_ref_val;i++) {
      glcd_set_byte(3+i,6,0x00);

      if ((i%5) == 0) {
        glcd_set_byte(3+i,6,0x00);
      }
    }
    
    glcd_update_area(3+val,prev_ref_val+3,48,56);
  }
  else {
    for (unsigned char i=prev_ref_val;i<val;i++) {
      glcd_set_byte(3+i,6,0xFF);

      if ((i%5) == 0) {
        glcd_set_byte(3+i,6,0x00);
      }
    }
  
    glcd_update_area(3+prev_ref_val,3+val,48,56);
  }
  
  prev_ref_val = val;
}

/*! \brief Show the power meter bargraphs 
 *  \param fwd_power The forward power in percent
 *  \param ref_power The reflected power in percent */
void display_handler_show_powermeter_bargraph(unsigned int fwd_power, unsigned int ref_power) {
  display_handler_show_bargraph_fwd(fwd_power*display_handler_status.powermeter_fwd_scale_val);
  display_handler_show_bargraph_ref(ref_power*display_handler_status.powermeter_ref_scale_val);
  
  glcd_update_all();
}

/*! \brief This function will print out the power meter text which shows FWD, REF power and VSWR
 *  \param fwd_power Forward power in watts
 *  \param ref_power Reflected power in watts
 *  \param vswr The current VSWR, for example 151 means 1.51:1 */
void display_handler_show_powermeter_text(unsigned int fwd_power, unsigned int ref_power, unsigned int vswr) {
  sprintf_P(power_temp_str,PSTR("%5iW"),fwd_power);
  display_handler_text_right_adjust(125,3,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
  sprintf_P(power_temp_str,PSTR("%3iW"),ref_power);
  display_handler_text_right_adjust(80,34,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
  
  if (vswr != 0) {
    sprintf_P(power_temp_str,PSTR("%i.%02i:1"),(vswr/100),(vswr%100));
    display_handler_text_right_adjust(125,45,power_temp_str,strlen(power_temp_str),FONT_NINE_DOT);
  }
  else //If we have VSWR as 0 we just clear that area instead of writing out 0.0:1
    display_handler_text_right_adjust(125,45,"      ",6,FONT_NINE_DOT);
  
  glcd_update_all();  
}

/*! \brief This function will show the power meter display */
void display_handler_show_powermeter(void) {
  prev_fwd_val = 0;
  prev_ref_val = 0;
  
  glcd_rectangle(0,0,28,128);
  glcd_rectangle(0,32,28,128);

  glcd_text(3,3,FONT_NINE_DOT,"FWD:",4);
  glcd_text(3,34,FONT_NINE_DOT,"REF:",4);
  display_handler_text_right_adjust(125,34,"VSWR",4,FONT_NINE_DOT);
}

/*! \brief Display the setup  in progress screen */
void display_handler_setup_view(void) {
  glcd_clear();
  display_handler_text_center_adjust(10,"Setup",5, FONT_FIFTEEN_DOT);
  display_handler_text_center_adjust(30,"in progress",11, FONT_FIFTEEN_DOT);
  glcd_update_all();
}

/*! \brief Display the shutdown in progress screen */
void display_handler_shutdown_view(void) {
  glcd_clear();
  display_handler_text_center_adjust(10,"Shutdown",8, FONT_FIFTEEN_DOT);
  display_handler_text_center_adjust(30,"in progress",11, FONT_FIFTEEN_DOT);
  glcd_update_all();
}

/**
 * \brief Display a set of antennas on the display
 * \param band The band you wish to show the antennas from */
void display_handler_antennas(unsigned char band) {
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

  if (runtime_settings.antenna_disabled[band-1] != 0) {
    unsigned char x2 = 0;
    
    if ((runtime_settings.antenna_disabled[band-1] & (1<<0)) != 0) {
     
      if (antenna_ctrl_get_antenna_text_length(0) != 0) {
        x2 = antenna_ctrl_get_antenna_text_length(0)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
    
        if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_IN_USE_FLAG))
          x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;

        glcd_line(0,x2,DISPLAY_TEXT_ANT1_Y_POS+5);
      }
    }
    
    if ((runtime_settings.antenna_disabled[band-1] & (1<<1)) != 0) {
      if (antenna_ctrl_get_antenna_text_length(1) != 0) {
        x2 = antenna_ctrl_get_antenna_text_length(1)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
    
        if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_IN_USE_FLAG))
          x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
        
        glcd_line(0,x2,DISPLAY_TEXT_ANT2_Y_POS+5);
      }
    }

    if ((runtime_settings.antenna_disabled[band-1] & (1<<2)) != 0) {
      if (antenna_ctrl_get_antenna_text_length(2) != 0) {
        x2 = antenna_ctrl_get_antenna_text_length(2)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
    
        if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_IN_USE_FLAG))
          x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
        
        glcd_line(0,x2,DISPLAY_TEXT_ANT3_Y_POS+5);
      }
    }

    if ((runtime_settings.antenna_disabled[band-1] & (1<<3)) != 0) {
      if (antenna_ctrl_get_antenna_text_length(3) != 0) {
        x2 = antenna_ctrl_get_antenna_text_length(3)*DISPLAY_TEXT_ANTENNA_WIDTH-1;
    
        if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_IN_USE_FLAG))
          x2 += DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH;
        
        glcd_line(0,x2,DISPLAY_TEXT_ANT4_Y_POS+5);
      }
    }
  }
}

/*! \brief Show the current selected RX ant 
 *  \param ant_index The antenna index of which antenna that is selected and should be shown  */
void display_handler_show_rx_ant(unsigned char ant_index) {
  CLEAR_RX_ANTENNA_AREA();
  
  if ((antenna_ctrl_get_rx_antenna_count() >= ant_index) && (ant_index != 0)) {
    char temp[strlen(antenna_ctrl_get_rx_antenna_name(ant_index-1))+5];
    
    sprintf_P((char *)temp, PSTR("RX: %s"),antenna_ctrl_get_rx_antenna_name(ant_index-1));
    
    glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
  }
}

/**
 * \brief Displays an antenna but inverted
 *
 * \param ant_index Which antenna you wish to invert */
void display_handler_invert_antenna(unsigned char ant_index) {
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
 * \brief Display the current rotator directions
 * If the rotator option has been enabled for a certain antenna
 * it will be shown it's current direction on the LCD
 * \param band The band you wish to show the rotators direction */
void display_handler_rotator_directions(unsigned char band) {
  CLEAR_ROTATOR_AREA();
  char temp_dir[9];
  
  if (antenna_ctrl_get_flags(0) & (1<<ANTENNA_ROTATOR_FLAG)) {
    sprintf_P((char *)temp_dir,PSTR("%3i deg"),antenna_ctrl_get_direction(0));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT1_X_POS,DISPLAY_TEXT_ROTATOR_ANT1_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }
  
  if (antenna_ctrl_get_flags(1) & (1<<ANTENNA_ROTATOR_FLAG)) {
    sprintf_P((char *)temp_dir,PSTR("%3i deg"),antenna_ctrl_get_direction(1),antenna_ctrl_get_rotates_char(1));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT2_X_POS,DISPLAY_TEXT_ROTATOR_ANT2_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }

  if (antenna_ctrl_get_flags(2) & (1<<ANTENNA_ROTATOR_FLAG)) {
    sprintf_P((char *)temp_dir,PSTR("%3i deg"),antenna_ctrl_get_direction(2));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT3_X_POS,DISPLAY_TEXT_ROTATOR_ANT3_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }

  if (antenna_ctrl_get_flags(3) & (1<<ANTENNA_ROTATOR_FLAG)) {
    sprintf_P((char *)temp_dir,PSTR("%3i deg"),antenna_ctrl_get_direction(3));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT4_X_POS,DISPLAY_TEXT_ROTATOR_ANT4_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }
  
  if (antenna_ctrl_get_sub_menu_type(0) == SUBMENU_VERT_ARRAY) {
    //sub_menu_get_text(0, sub_menu_get_current_pos(0))
    sprintf_P((char *)temp_dir,PSTR("Dir %s"),sub_menu_get_text(0, sub_menu_get_current_pos(0)));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT1_X_POS,DISPLAY_TEXT_ROTATOR_ANT1_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }
  
  if (antenna_ctrl_get_sub_menu_type(1) == SUBMENU_VERT_ARRAY) {
    sprintf_P((char *)temp_dir,PSTR("Dir %s"),sub_menu_get_text(1, sub_menu_get_current_pos(1)));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT2_X_POS,DISPLAY_TEXT_ROTATOR_ANT2_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }
  
  if (antenna_ctrl_get_sub_menu_type(2) == SUBMENU_VERT_ARRAY) {
    sprintf_P((char *)temp_dir,PSTR("Dir %s"),sub_menu_get_text(2, sub_menu_get_current_pos(2)));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT3_X_POS,DISPLAY_TEXT_ROTATOR_ANT3_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  }
  
  if (antenna_ctrl_get_sub_menu_type(3) == SUBMENU_VERT_ARRAY) {
    sprintf_P((char *)temp_dir,PSTR("Dir %s"),sub_menu_get_text(3, sub_menu_get_current_pos(3)));
    display_handler_text_right_adjust(DISPLAY_TEXT_ROTATOR_ANT4_X_POS,DISPLAY_TEXT_ROTATOR_ANT4_Y_POS,temp_dir,strlen(temp_dir),FONT_SEVEN_DOT);
  } 
}

/*! \brief Updates the display 
 * \param band Which band to show the antenna information from
 * \param antenna The antenna combination that is currently selected
 */
void display_handler_update_antennas(unsigned char band, unsigned char antenna) {
  display_handler_antennas(band);

  display_handler_rotator_directions(band);
    
  glcd_line(0,128,55);
    
  if (antenna & (1<<0))
    display_handler_invert_antenna(ANTENNA_1);

  if (antenna & (1<<1))
    display_handler_invert_antenna(ANTENNA_2);

  if (antenna & (1<<2))
    display_handler_invert_antenna(ANTENNA_3);

  if (antenna & (1<<3))
    display_handler_invert_antenna(ANTENNA_4);

  display_handler_status_field_text(strlen(status_field_text),status_field_text);
  
  glcd_update_all();
}

/*! \brief Will show the sub menu of a certain antenna 
 *  \param ant_index The antenna index (0-3)
 *  \param sub_menu_type Which type of sub menu it is */
void display_handler_show_sub_menu(unsigned char ant_index, unsigned char sub_menu_type) {
  char temp[12];
  
  if (sub_menu_type == SUBMENU_VERT_ARRAY) {
    CLEAR_SET_SUB_MENU_ARRAY_AREA();

    sprintf((char *)temp, "%s",antenna_ctrl_get_antenna_text(ant_index));
    display_handler_text_center_adjust(3, temp, strlen(temp), FONT_NINE_DOT);
    
    strcpy_P((char *)temp,PSTR("Direction"));
    display_handler_text_center_adjust(19, temp, strlen(temp), FONT_NINE_DOT);
    
    sprintf((char *)temp, "%s",sub_menu_get_text(ant_index, sub_menu_get_current_pos(ant_index)));
    display_handler_text_center_adjust(38, temp, strlen(temp), FONT_FIFTEEN_DOT);
    
    if (status.selected_rx_antenna != 0) {
      CLEAR_RX_ANTENNA_AREA();

      if ((antenna_ctrl_get_rx_antenna_count() >= status.selected_rx_antenna) && (status.selected_rx_antenna != 0)) {
        char temp[strlen(antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1))+5];
    
        sprintf_P((char *)temp, PSTR("RX: %s"),antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1));
    
        glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
      }
    }
    
    glcd_update_all();
  }
  else if (sub_menu_type == SUBMENU_STACK) {
    CLEAR_SET_SUB_MENU_STACK_AREA();
    
    sprintf((char *)temp, "%s",antenna_ctrl_get_antenna_text(ant_index));
    display_handler_text_center_adjust(3, temp, strlen(temp), FONT_NINE_DOT);
    
    strcpy_P((char *)temp, PSTR("combination"));
    display_handler_text_center_adjust(19, temp, strlen(temp), FONT_NINE_DOT);
    
    sprintf((char *)temp, "%s",sub_menu_get_text(ant_index, sub_menu_get_current_pos(ant_index)));
    display_handler_text_center_adjust(38, temp, strlen(temp), FONT_FIFTEEN_DOT);
    
    if (status.selected_rx_antenna != 0) {
      CLEAR_RX_ANTENNA_AREA();

      if ((antenna_ctrl_get_rx_antenna_count() >= status.selected_rx_antenna) && (status.selected_rx_antenna != 0)) {
        char temp[strlen(antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1))+5];
    
        sprintf_P((char *)temp, PSTR("RX: %s"),antenna_ctrl_get_rx_antenna_name(status.selected_rx_antenna-1));
    
        glcd_text(DISPLAY_TEXT_RX_ANT_X_POS, DISPLAY_TEXT_RX_ANT_Y_POS, FONT_SIX_DOT, temp, strlen(temp));
      }
    }
    
    glcd_update_all();
  }
}

/*! \brief Show SET rotator heading
 *  \param rotator_heading The current set rotator heading  */
void display_handler_show_set_heading(unsigned int rotator_heading) {
  glcd_clear();
  char temp[8];
  char size = 0;
  
  if (rotator_heading < 10)
    size = 1;
  else if (rotator_heading < 100)
    size = 2;
  else
    size = 3;
      
  strcpy_P((char *)temp, PSTR("Heading"));
  display_handler_text_center_adjust(0, temp, strlen(temp), FONT_FIFTEEN_DOT);
  
  sprintf((char *)temp, "%i",rotator_heading);
  display_handler_text_center_adjust(20, temp, size, FONT_MT1);
  
  glcd_update_all();
}

/**
 * \brief Displays the status field
 * Will display the text on the status field
 * \param length The length of the text we wish to shown
 * \param text The text which we wish to display */
void display_handler_status_field_text(unsigned char length, char *text) {
  CLEAR_RADIO_STATUS_AREA();
  display_handler_text_right_adjust(DISPLAY_RADIO_FREQ_X_POS,DISPLAY_RADIO_FREQ_Y_POS,text,length,FONT_SIX_DOT);
}

/*! \brief Update the radio frequency area of the display */
/*void display_update_radio_freq(void) {
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
*/
/*! \brief Gets called at DISPLAY_HANDLER_TICK_INTERVAL ms intervals, which is defined in main.h */
void display_handler_tick(void) {
  //Check if there is a new display to be viewed or we force a repaint of the current view
  if ((display_handler_status.new_display != display_handler_status.active_display) || (display_handler_status.force_repaint == 1)) {
    #ifdef DEBUG_COMPUTER_USART_ENABLED
      printf("FORCE_STATUS: %i\r\n",display_handler_status.force_repaint);
    #endif
    
    display_handler_status.force_repaint = 0;
    
    if ((display_handler_status.new_display == DISPLAY_HANDLER_VIEW_POWERMETER) && (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU))
      return;
    
    if ((display_handler_status.new_display == DISPLAY_HANDLER_VIEW_POWERMETER) && (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR))
      return;
      
    if (display_handler_status.active_display != display_handler_status.new_display)
      display_handler_status.prev_display = display_handler_status.active_display;

    display_handler_status.active_display = display_handler_status.new_display;

    if (status.function_status & (1<<FUNC_STATUS_SHUTDOWN_IN_PROGRESS)) {
      display_handler_shutdown_view();
    }
    else {
      if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_POWERMETER) {
        glcd_clear();
        display_handler_show_powermeter();
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_OPENASC_LOGO) {
        glcd_print_picture();
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_CONFIG_SPLASH) {
        display_handler_setup_view(); 
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_ANTENNAS) {
        if (status.selected_band != BAND_UNDEFINED) {
          if (status.function_status & (1<<FUNC_STATUS_RXANT))
            display_handler_show_rx_ant(status.selected_rx_antenna);
          else
            display_handler_show_rx_ant(0);
            
            display_handler_update_antennas(status.selected_band, status.selected_ant);
        }
        else {
          display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
          display_handler_status.force_repaint = 1;
        }
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) {
        glcd_clear();
        display_handler_show_sub_menu(status.sub_menu_antenna_index,sub_menu_get_type(status.sub_menu_antenna_index));
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR) {
        display_handler_show_set_heading(status.new_beamheading);
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) {
        menu_show(menu_curr_pos());
      }
      else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SHUTDOWN) {
        display_handler_shutdown_view();
      }
    }
  }
}

/*! \brief Set a new display screen */
void display_handler_new_view(unsigned int display_view) {
  display_handler_status.new_display = display_view;
}

void display_handler_repaint(void) {
  display_handler_status.force_repaint = 1;
}

void display_handler_new_view_powermeter(unsigned int bargraph_update_rate, unsigned int text_update_rate, double fwd_scale_val, double ref_scale_val) {
  display_handler_status.new_display = DISPLAY_HANDLER_VIEW_POWERMETER;
  
  display_handler_status.powermeter_bargraph_update_rate = bargraph_update_rate;
  display_handler_status.powermeter_text_update_rate = text_update_rate;
  
  display_handler_status.powermeter_fwd_scale_val = fwd_scale_val;
  display_handler_status.powermeter_ref_scale_val = ref_scale_val;
}

void display_handler_show_last_view(void) {
  display_handler_status.new_display = display_handler_status.prev_display;
  display_handler_status.prev_display = display_handler_status.active_display;
}

void display_handler_prev_view(void) {
  glcd_clear();

  //This might be very ugly way of doing it, but I have not figured out another way
  
  #ifdef DEBUG_COMPUTER_USART_ENABLED
    printf("Active disp: %i\n\r",display_handler_status.active_display);
    printf("Prev disp: %i\n\r",display_handler_status.prev_display);
    printf("New disp: %i\n\r",display_handler_status.new_display);
  #endif
    
  if ((display_handler_status.new_display == DISPLAY_HANDLER_VIEW_POWERMETER) && (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU)) {
    display_handler_status.new_display = display_handler_status.active_display;
  }
  else if ((display_handler_status.new_display == DISPLAY_HANDLER_VIEW_POWERMETER) && (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR)) {
    display_handler_status.new_display = display_handler_status.active_display;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_POWERMETER)) {
    display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SUBMENU)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_MENU)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_ANTENNAS)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }  
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SUBMENU)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR)) {
    if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;   
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_MENU)) {
    if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR) {
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SCREENSAVER) {
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_OPENASC_LOGO)) {
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_MENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SCREENSAVER)) {
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SCREENSAVER)) {  
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;   
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_ANTENNAS) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR)) {  
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;   
  }
  else if ((display_handler_status.active_display == DISPLAY_HANDLER_VIEW_SUBMENU) && (display_handler_status.prev_display == DISPLAY_HANDLER_VIEW_ANTENNAS)) {  
    if (PORTH & (1<<LED_ERROR_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (PORTC & (1<<LED_ROTATE_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR;
    else if (PORTD & (1<<LED_SUBMENU_BIT))
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_SUBMENU;
    else if (PORTK & (1<<7)) //Menu LED bit 
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_MENU;
    else if (status.selected_band == BAND_UNDEFINED)
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_OPENASC_LOGO;
    else
      display_handler_status.new_display = DISPLAY_HANDLER_VIEW_ANTENNAS;
  }
  else {
    #ifdef DEBUG_COMPUTER_USART_ENABLED
      printf("PREV_DISP_ELSE\r\n");
    #endif
      
    display_handler_status.new_display = display_handler_status.prev_display;
  }
  
  #ifdef DEBUG_COMPUTER_USART_ENABLED
    printf("EXIT NEW DISP: %i\r\n",display_handler_status.new_display);
  #endif

  display_handler_status.force_repaint = 1;
}

unsigned int display_handler_get_view(void) {
  return(display_handler_status.active_display);
}

void display_handler_set_status_field_text_P(unsigned char length, const char *data) {
  if (length <= 24) {
    strncpy_P(status_field_text,data,length);
    
    status_field_text[length] = 0;
  }
}

void display_handler_set_status_field_text(unsigned char length, char *data) {
  if (length <= 24) {
    strncpy(status_field_text,data,length);

    status_field_text[length] = 0;
  }
}

void display_handler_powermeter_bargraph_tick(void) {
  if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_POWERMETER) {
    display_handler_show_powermeter_bargraph(powermeter_get_fwd_power(), powermeter_get_ref_power());
  }
}

void display_handler_powermeter_text_tick(void) {
  if (display_handler_status.active_display == DISPLAY_HANDLER_VIEW_POWERMETER) {
    display_handler_show_powermeter_text(powermeter_get_fwd_power(),powermeter_get_ref_power(),powermeter_get_vswr());  
  }
}