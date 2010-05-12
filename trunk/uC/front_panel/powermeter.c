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

powermeter_struct powermeter_status;

unsigned char powermeter_flags;

//! The counter which keeps track of when we should update the power meter text
unsigned int counter_powermeter_update_text=0;

//! The counter which keeps track of when we should update the power meter bargraph
unsigned int counter_powermeter_update_bargraph=0;

void powermeter_init(unsigned char pickup_addr, unsigned int text_update_rate, unsigned int bargraph_update_rate, unsigned int vswr_limit) {
	powermeter_status.curr_fwd_pwr_value = 0;
	powermeter_status.curr_ref_pwr_value = 0;
	powermeter_status.curr_vswr_value = 0;
	
	powermeter_status.pickup_addr = pickup_addr;
	powermeter_status.text_update_rate = text_update_rate;
	powermeter_status.bargraph_update_rate = bargraph_update_rate;
	powermeter_status.vswr_limit = vswr_limit;
}

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

void powermeter_update_values(unsigned int fwd_pwr, unsigned int ref_pwr, unsigned int vswr) {
	powermeter_status.curr_fwd_pwr_value = fwd_pwr;
	powermeter_status.curr_ref_pwr_value = ref_pwr;
	powermeter_status.curr_vswr_value = vswr;
}

void powermeter_process_tasks(void) {
	if (powermeter_flags & (1<<POWERMETER_FLAG_ACTIVE)) {
		if (counter_powermeter_update_text >= powermeter_status.text_update_rate) {
			display_show_powermeter_text(powermeter_status.curr_fwd_pwr_value,powermeter_status.curr_ref_pwr_value,powermeter_status.curr_vswr_value);
			
			counter_powermeter_update_text = 0;
		}
		
		if (counter_powermeter_update_bargraph >= powermeter_status.bargraph_update_rate) {
			display_show_powermeter_bargraph(70, 20);
			
			counter_powermeter_update_bargraph = 0;
		}
	}
}

void powermeter_1ms_tick(void) {
	counter_powermeter_update_text++;
	counter_powermeter_update_bargraph++;
}
