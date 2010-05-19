/*! \file front_panel/powermeter.h 
 *  \brief Power meter functions
 *  \ingroup front_panel_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-05-12
 *  \code #include "front_panel/powermeter.h" \endcode
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

#ifndef _POWERMETER_H_
#define _POWERMETER_H_

//! Struct which contains information of the power meter status
typedef struct {
	//! Current forward power in watts
	unsigned int curr_fwd_pwr_value;
	//! Current reflected power in watts
	unsigned int curr_ref_pwr_value;
	//! VSWR represented as an integer, 152 means 1.52:1 in SWR
	unsigned int curr_vswr_value;
	//! Address of the power meter pickup
	unsigned char pickup_addr;
	//! The update in ms of the text on the display
	unsigned int text_update_rate;
	//! The update rate in ms of the bargraph
	unsigned int bargraph_update_rate;
	//! The VSWR limit of when the radios PTT should be deactivated and the device set into ERROR mode
	unsigned int vswr_limit;
} powermeter_struct;

void powermeter_update_values(unsigned int fwd_pwr, unsigned int ref_pwr, unsigned int vswr);
void powermeter_init(unsigned char pickup_addr, unsigned int text_update_rate, unsigned int bargraph_update_rate, unsigned int vswr_limit);
void powermeter_process_tasks(void);
void powermeter_1ms_tick(void);

#endif