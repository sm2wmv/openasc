/*! \file front_panel/display_handler.h
 *  \ingroup front_panel_group 
 *  \brief Device handler
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-08-14
 *  \code #include "front_panel/display_handler.h" \endcode
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

#ifndef _DISPLAY_HANDLER_H_
#define _DISPLAY_HANDLER_H_


#include "glcd.h"
#include "main.h"

//! Macro that clears the antenna area of the LCD
#define CLEAR_ANT_AREA()	glcd_clear_area(0,90,0,56)
//! Macro that clears the rotator area of the LCD
#define CLEAR_ROTATOR_AREA()	glcd_clear_area(90,128,0,56)
//! Macro that clears the radio frequency area of the LCD
#define CLEAR_RADIO_FREQ_AREA()	glcd_clear_area(78,128,58,64)
//! Macro that clears the RX antenna area
#define CLEAR_RX_ANTENNA_AREA() glcd_clear_area(0,70,58,64)
//! Macro that clears the rotator area
#define CLEAR_SET_ROTATOR_AREA() glcd_clear_area(0,71,58,64)
//! Macro that clears the sub menu array direction area
#define CLEAR_SET_SUB_MENU_ARRAY_AREA() glcd_clear_area(44,84,35,64)
//! Macro that clears the sub menu stack
#define CLEAR_SET_SUB_MENU_STACK_AREA() glcd_clear_area(14,114,35,64)


#define DISPLAY_SCREENSAVER_DEF_CONTRAST	0

/**************** Text positions ****************/
//! The position of the frequency text X position
#define DISPLAY_RADIO_FREQ_X_POS	128
//! The position of the frequency text Y position
#define DISPLAY_RADIO_FREQ_Y_POS	58

//! The position of the antenna1 rotator text X position
#define DISPLAY_TEXT_ROTATOR_ANT1_X_POS	128
//! The position of the antenna1 rotator text Y position
#define DISPLAY_TEXT_ROTATOR_ANT1_Y_POS	1

//! The position of the antenna2 rotator text X position
#define DISPLAY_TEXT_ROTATOR_ANT2_X_POS	128
//! The position of the antenna2 rotator text Y position
#define DISPLAY_TEXT_ROTATOR_ANT2_Y_POS	16

//! The position of the antenna3 rotator text X position
#define DISPLAY_TEXT_ROTATOR_ANT3_X_POS	128
//! The position of the antenna3 rotator text Y position
#define DISPLAY_TEXT_ROTATOR_ANT3_Y_POS	29

//! The position of the antenna4 rotator text X position
#define DISPLAY_TEXT_ROTATOR_ANT4_X_POS	128
//! The position of the antenna4 rotator text Y position
#define DISPLAY_TEXT_ROTATOR_ANT4_Y_POS	43

//! The position of the antenna1 text X position
#define DISPLAY_TEXT_ANT1_X_POS	0
//! The position of the antenna1 text Y position
#define DISPLAY_TEXT_ANT1_Y_POS	0

//! The position of the antenna2 text X position
#define DISPLAY_TEXT_ANT2_X_POS	0
//! The position of the antenna2 text Y position
#define DISPLAY_TEXT_ANT2_Y_POS	15

//! The position of the antenna3 text X position
#define DISPLAY_TEXT_ANT3_X_POS	0
//! The position of the antenna3 text Y position
#define DISPLAY_TEXT_ANT3_Y_POS	28

//! The position of the antenna4 text X position
#define DISPLAY_TEXT_ANT4_X_POS	0
//! The position of the antenna4 text Y position
#define DISPLAY_TEXT_ANT4_Y_POS	42

//! The position of the RX antenna X pos
#define DISPLAY_TEXT_RX_ANT_X_POS	0
//! The position of the RX antenna Y position
#define DISPLAY_TEXT_RX_ANT_Y_POS	58

//! The position of the SET ROTATE X pos
#define DISPLAY_TEXT_ROTATE_ANT_X_POS	0
//! The position of the SET ROTATE Y position
#define DISPLAY_TEXT_ROTATE_ANT_Y_POS	58

//! The height of the antenna text inverting area
#define DISPLAY_TEXT_ANT_HEIGHT	10

//! The width of the antenna text
#define DISPLAY_TEXT_ANTENNA_WIDTH 8

//! The width addition of the inverted area when an antenna is in use
#define DISPLAY_TEXT_ANTENNA_IN_USE_ADDITION_WIDTH	16

/************************************************/

#define DISPLAY_HANDLER_VIEW_OPENASC_LOGO     0
#define DISPLAY_HANDLER_VIEW_ANTENNAS         1
#define DISPLAY_HANDLER_VIEW_POWERMETER       2
#define DISPLAY_HANDLER_VIEW_SUBMENU          3
#define DISPLAY_HANDLER_VIEW_SET_ROTATOR_DIR  4
#define DISPLAY_HANDLER_VIEW_MENU             5
#define DISPLAY_HANDLER_VIEW_SHUTDOWN         6
#define DISPLAY_HANDLER_VIEW_CONFIG_SPLASH    7
#define DISPLAY_HANDLER_VIEW_SCREENSAVER      8

typedef struct {
  unsigned int new_display;
  unsigned int active_display;
  unsigned int prev_display;
  unsigned char force_repaint;
  
  unsigned int counter_powermeter_update_text;
  unsigned int counter_powermeter_update_bargraph;
  
  unsigned int powermeter_bargraph_update_rate;
  unsigned int powermeter_text_update_rate;
  
  double powermeter_fwd_scale_val;
  double powermeter_ref_scale_val;
  
  unsigned char screensaver_mode;
} display_handler_status_struct;

void display_handler_text_center_adjust(unsigned char y,char *str, unsigned char length, unsigned char font);
unsigned char display_handler_calculate_width(char *str, unsigned char font, unsigned char length);
void display_handler_text_right_adjust(unsigned char x, unsigned char y,char *str, unsigned char length, unsigned char font);

void display_handler_show_bargraph_fwd(unsigned char percent);
void display_handler_show_bargraph_ref(unsigned char percent);

void display_handler_show_powermeter_bargraph(unsigned int fwd_power, unsigned int ref_power);
void display_handler_show_powermeter_text(unsigned int fwd_power, unsigned int ref_power, unsigned int vswr);
void display_handler_show_powermeter(void);

void display_handler_new_view_powermeter(unsigned int bargraph_update_rate, unsigned int text_update_rate, double fwd_scale_val, double ref_scale_val);
void display_handler_new_view(unsigned int display_view);
void display_handler_prev_view(void);

void display_handler_tick(void);

void display_handler_setup_view(void);
void display_handler_shutdown_view(void);

void display_handler_antennas(unsigned char band);
void display_handler_show_rx_ant(unsigned char ant_index);
void display_handler_update_antennas(unsigned char band, unsigned char antenna);
void display_handler_rotator_directions(unsigned char band);
void display_handler_show_sub_menu(unsigned char ant_index, unsigned char sub_menu_type);
void display_handler_repaint(void);
void display_handler_show_set_heading(unsigned int rotator_heading);

unsigned int display_handler_get_view(void);

void display_handler_disable_screensaver(void);
unsigned char display_handler_screensaver_mode(void);
void display_handler_enable_screensaver(void);
void display_handler_update_screensaver(void);

void display_handler_set_backlight(unsigned char value);

#endif