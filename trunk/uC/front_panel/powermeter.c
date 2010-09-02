/*! \file front_panel/powermeter.c
 *  \brief Power meter
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-05-12
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
#include "display.h"
#include "glcd.h"
#include "main.h"

#define POWERMETER_FLAG_ACTIVE 					0

//! The current status of the power meter
powermeter_struct powermeter_status;

//! Various flags used in the powermeter, defines can be found in powermeter.h
unsigned char powermeter_flags;

//! The counter which keeps track of when we should update the power meter text
unsigned int counter_powermeter_update_text=0;

//! The counter which keeps track of when we should update the power meter bargraph
unsigned int counter_powermeter_update_bargraph=0;

unsigned char text_view_mode = 0;
unsigned char pickup_type = 99;

double fwd_scale_value = 0;
double ref_scale_value = 0;

/*! \brief Initialize the power meter
 *  \param pickup_addr The address of the powermeter unit that sends the information
 *  \param text_update_rate How often we should refresh the text on the display
 *  \param bargraph_update_rate How often we should update the bargraph of the display
 *  \param vswr_limit What is the SWR limit of the device, when this is exceeded we shut down the possibility to PTT 
 *  \param text_view Which kind of data is shown on the text display */
void powermeter_init(unsigned char pickup_addr, unsigned int text_update_rate, unsigned int bargraph_update_rate, unsigned int vswr_limit, unsigned char text_view) {
	powermeter_status.curr_fwd_pwr_value = 0;
	powermeter_status.curr_ref_pwr_value = 0;
	powermeter_status.curr_vswr_value = 0;
		
	powermeter_status.pickup_addr = pickup_addr;
	powermeter_status.text_update_rate = text_update_rate;
	powermeter_status.bargraph_update_rate = bargraph_update_rate;
	powermeter_status.vswr_limit = vswr_limit;
	text_view_mode = text_view;
}

/*! \brief Activate the power meter display 
/*! \param state If this is set to 1 we will activate the powermeter, if set to 0 we will deactivate it */
void powermeter_set_active(unsigned char state) {
	if (state != 0) {
		powermeter_flags |= (1<<POWERMETER_FLAG_ACTIVE);
		
		glcd_clear();
		display_show_powermeter();
		glcd_update_all();
	}
	else {
		powermeter_flags &= ~(1<<POWERMETER_FLAG_ACTIVE);
		
		glcd_clear();
		main_update_display();
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
				ref_scale_value = 6.67;
				break;
			case PICKUP_TYPE_1000W:
				fwd_scale_value = 0.1;
				ref_scale_value = 1;
				break;
			case PICKUP_TYPE_1500W:
				fwd_scale_value = 0.0667;
				ref_scale_value = 0.667;
				break;
			case PICKUP_TYPE_2000W:
				fwd_scale_value = 0.05;
				ref_scale_value = 0.5;
				break;
			case PICKUP_TYPE_3000W:
				fwd_scale_value = 0.033;
				ref_scale_value = 0.33;
				break;
			case PICKUP_TYPE_5000W:
				fwd_scale_value = 0.02;
				ref_scale_value = 0.2;
				break;
			case PICKUP_TYPE_10000W:
				fwd_scale_value = 0.01;
				ref_scale_value = 0.1;
				break;
			case PICKUP_TYPE_15000W:
				fwd_scale_value = 0.0067;
				ref_scale_value = 0.067;
				break;
			default:
				fwd_scale_value = 0;
				ref_scale_value = 0;
				break;
		}
	}
	
	pickup_type = type;
}

/*! \brief This function should be called as much as possible and it does all the updates, such checking for new data, updating display etc */
void powermeter_process_tasks(void) {
	//TODO: Fix so that it shows peak or average power according to the settings	
	if (powermeter_flags & (1<<POWERMETER_FLAG_ACTIVE)) {
		if (counter_powermeter_update_text >= powermeter_status.text_update_rate) {
			display_show_powermeter_text(powermeter_status.curr_fwd_pwr_value,powermeter_status.curr_ref_pwr_value,powermeter_status.curr_vswr_value);
			
			counter_powermeter_update_text = 0;
		}
			
		if (counter_powermeter_update_bargraph >= powermeter_status.bargraph_update_rate) {
			display_show_powermeter_bargraph(powermeter_status.curr_fwd_pwr_value*fwd_scale_value, powermeter_status.curr_ref_pwr_value*ref_scale_value);
			
			counter_powermeter_update_bargraph = 0;
		}
	}
	
	glcd_update_all();
}

/*! \brief This function should be called at 1 ms intervals. It is to keep track of update rates etc for the display */
void powermeter_1ms_tick(void) {
	counter_powermeter_update_text++;
	counter_powermeter_update_bargraph++;
}
