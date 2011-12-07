/*! \file 4cx1500b/ctrl_box_front/display.h
 *  \ingroup amp_front_panel_group 
 *  \brief The serial interface to configure the device and control it
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/display.h" \endcode
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

#ifndef _DISPLAY_H_
#define _DISPLAY_H_


#include "glcd.h"
#include "main.h"

#define DISPLAY_SCREENSAVER_DEF_CONTRAST	0

//! The number of different display views we have
#define DISPLAY_VIEW_COUNT  5

//! View which shows the current error(s)
#define DISPLAY_VIEW_ERRORS             0
//! View which shows the Forward power, VSWR and reflected power
#define DISPLAY_VIEW_FWD_REF_VSWR       1
//! View which shows the Forward power, VSWR and grid current
#define DISPLAY_VIEW_FWD_GRIDC_VSWR     2
//! View which shows the HV voltage, Screen voltage and control voltage
#define DISPLAY_VIEW_HV_SV_CV           3
//! View which shows the current tune and load C position (as A/D value, ie max 1024)
#define DISPLAY_VIEW_TUNE_LOAD_POS      4

//! The update rate for texts
#define DISPLAY_UPDATE_RATE_TEXT      500
//! The update rate for bargraph displays
#define DISPLAY_UPDATE_RATE_BARGRAPH  100

void display_disable_screensaver(void);
void display_enable_screensaver(void);
void display_shutdown_view(void);
void display_update_screensaver(void);
unsigned char display_screensaver_mode(void);
void display_set_backlight(unsigned char value);
unsigned char display_calculate_width(char *str, unsigned char font, unsigned char length);
void display_text_center_adjust(unsigned char y,char *str, unsigned char length, unsigned char font);

void display_show_powermeter_bargraph(unsigned int fwd_power, unsigned int ref_power);
void display_show_powermeter_text(unsigned int fwd_power, unsigned int ref_power, unsigned int vswr);
void display_show_powermeter(void);

void display_update(void);

void display_1ms_tick(void);

#endif