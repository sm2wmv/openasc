/*! \file front_panel/powermeter.c
 *  \brief Power meter
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-09-04
 *  \code #include "front_panel/powermeter.c" \endcode
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
#include <string.h>

#include "powermeter.h"
#include "radio_interface.h"
#include "glcd.h"
#include "main.h"
#include "errors.h"
#include "display_handler.h"

#define POWERMETER_FLAG_ACTIVE 					0

//! The current status of the power meter
powermeter_struct powermeter_status;

//! Various flags used in the powermeter, defines can be found in powermeter.h
unsigned char powermeter_flags;

//! The counter which keeps track of when we should update the power meter text
unsigned int counter_powermeter_update_text=0;

//! The counter which keeps track of when we should update the power meter bargraph
unsigned int counter_powermeter_update_bargraph=0;

unsigned char pickup_type = 99;

double fwd_scale_value = 0;
double ref_scale_value = 0;

/*! \brief Initialize the power meter
 *  \param text_update_rate How often we should refresh the text on the display
 *  \param bargraph_update_rate How often we should update the bargraph of the display */
void powermeter_init(unsigned int text_update_rate, unsigned int bargraph_update_rate) {
	powermeter_status.curr_fwd_pwr_value = 0;
	powermeter_status.curr_ref_pwr_value = 0;
	powermeter_status.curr_vswr_value = 0;
		
	powermeter_status.text_update_rate = text_update_rate;
	powermeter_status.bargraph_update_rate = bargraph_update_rate;
}

/*! \brief Activate the power meter display 
/*! \param state If this is set to 1 we will activate the powermeter, if set to 0 we will deactivate it */
void powermeter_set_active(unsigned char state) {
	if (state != 0) {
    //if ((status.function_status & (1<<FUNC_STATUS_MENU_ACTIVE)) == 0) {
      powermeter_flags |= (1<<POWERMETER_FLAG_ACTIVE);
      display_handler_new_view_powermeter(powermeter_status.bargraph_update_rate, powermeter_status.text_update_rate, fwd_scale_value, ref_scale_value);
    //}
	}
	else {
    //if ((status.function_status & (1<<FUNC_STATUS_MENU_ACTIVE)) == 0) {
      powermeter_flags &= ~(1<<POWERMETER_FLAG_ACTIVE);
      display_handler_prev_view();
//    }
	}
}

/*! \brief Update the values of the power meter
 *  \param fwd_pwr The current forward power in watts
 *  \param ref_pwr The current reflected power in watts
 *  \param vswr The current VSWR value, for example 151 means 1.51:1 in VSWR 
 *  \param type Which kind of pickup type it is */
void powermeter_update_values(unsigned int fwd_pwr, unsigned int ref_pwr, unsigned int vswr, unsigned char type) {
	powermeter_status.curr_fwd_pwr_value = fwd_pwr;
	powermeter_status.curr_ref_pwr_value = ref_pwr;
	powermeter_status.curr_vswr_value = vswr;

	if (type != pickup_type) {
		switch(type) {
			case PICKUP_TYPE_150W:
				fwd_scale_value = 0.667;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_1000W:
				fwd_scale_value = 0.1;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_1500W:
				fwd_scale_value = 0.0667;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_2000W:
				fwd_scale_value = 0.05;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_3000W:
				fwd_scale_value = 0.033;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_5000W:
				fwd_scale_value = 0.02;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_10000W:
				fwd_scale_value = 0.01;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			case PICKUP_TYPE_15000W:
				fwd_scale_value = 0.0067;
				ref_scale_value = fwd_scale_value*REF_SCALE_PARAM;
				break;
			default:
				fwd_scale_value = 0;
				ref_scale_value = 0;
				break;
		}
	}
	
	pickup_type = type;
}

/*! \brief This function should be called at 1 ms intervals. It is to keep track of update rates etc for the display */
void powermeter_1ms_tick(void) {
	counter_powermeter_update_text++;
	counter_powermeter_update_bargraph++;
}

/*! \brief Get the update rate of the text
 *  \return The update rate in ms */
unsigned int powermeter_get_text_update_rate(void) {
  return(powermeter_status.text_update_rate);
}

/*! \brief Get the update rate of the bargraph
 *  \return The update rate in ms */
unsigned int powermeter_get_bargraph_update_rate(void) {
  return(powermeter_status.bargraph_update_rate);
}

/*! \brief Get the current forward power value
 *  \return The forward power in watts */
unsigned int powermeter_get_fwd_power(void) {
  return(powermeter_status.curr_fwd_pwr_value);
}

/*! \brief Get the current reflected power value
 *  \return The reflected power in watts */
unsigned int powermeter_get_ref_power(void) {
  return(powermeter_status.curr_ref_pwr_value);
}

/*! \brief Get the current VSWR, 253 means 2.53:1
 *  \return The VSWR as an int */
unsigned int powermeter_get_vswr(void) {
  return(powermeter_status.curr_vswr_value); 
}

double powermeter_get_fwd_scale_val(void) {
  return(fwd_scale_value);
}

double powermeter_get_ref_scale_val(void) {
  return(ref_scale_value);
}

unsigned char powermeter_is_active(void) {
  if (powermeter_flags & (1<<POWERMETER_FLAG_ACTIVE))
    return(1);

  return(0);
}