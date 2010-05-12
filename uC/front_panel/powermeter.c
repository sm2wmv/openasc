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

powermeter_struct powermeter_status;

void powermeter_init(void) {
	powermeter_status.curr_fwd_pwr_value = 0;
	powermeter_status.curr_ref_pwr_value = 0;
	powermeter_status.curr_vswr_value = 0;
	
	powermeter_status.prev_fwd_pwr_value = 0;
	powermeter_status.prev_ref_pwr_value = 0;
	powermeter_status.prev_vswr_value = 0;	
}

void powermeter_process_tasks(void) {
	if ((radio_get_ptt_status() & (1<<RADIO_FLAG_RADIO_PTT)) != 0) {
		if ((main_flags & (1<<FLAGS_POWERMETER_ACTIVE)) == 0) {
			glcd_clear();
			display_show_powermeter();
			display_show_powermeter_text(8705,160,131);
			display_show_powermeter_bargraph(70, 20);
			glcd_update_all();
				
			main_flags |= (1<<FLAGS_POWERMETER_ACTIVE);
		}
			
		if ((main_flags & (1<<FLAG_POWERMETER_UPDATE_TEXT)) != 0) {
			display_show_powermeter_text(8705,160,131);
				
			main_flags &= ~(1<<FLAG_POWERMETER_UPDATE_TEXT);
			counter_powermeter_update_text = 0;
		}
			
		if ((main_flags & (1<<FLAG_POWERMETER_UPDATE_BARGRAPH)) != 0) {
			display_show_powermeter_bargraph(70, 20);
				
			main_flags &= ~(1<<FLAG_POWERMETER_UPDATE_BARGRAPH);
			counter_powermeter_update_bargraph = 0;
		}
	}
	else if ((main_flags & (1<<FLAGS_POWERMETER_ACTIVE)) != 0) {
		glcd_clear();
		main_flags &= ~(1<<FLAGS_POWERMETER_ACTIVE);
		main_flags |= (1<<FLAG_UPDATE_DISPLAY);
	}
}