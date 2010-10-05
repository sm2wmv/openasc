/*! \file front_panel/powermeter.h 
 *  \brief Power meter functions
 *  \ingroup front_panel_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-09-04
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

//! Show the peak power on the text display
#define POWERMETER_TEXT_VIEW_PEAK	0
//! Show the average power on the text display
#define POWERMETER_TEXT_VIEW_AVG	1

/*! 150w pickup configuration */
#define PICKUP_TYPE_150W        0
/*! 1000w pickup configuration */
#define PICKUP_TYPE_1000W       1
/*! 1500w pickup configuration */
#define PICKUP_TYPE_1500W       2
/*! 2000w pickup configuration */
#define PICKUP_TYPE_2000W       3
/*! 3000w pickup configuration */
#define PICKUP_TYPE_3000W       4
/*! 5000w pickup configuration */
#define PICKUP_TYPE_5000W       5
/*! 10000w pickup configuration */
#define PICKUP_TYPE_10000W      6
/*! 15000w pickup configuration */
#define PICKUP_TYPE_15000W      7

//! How much we scale the reflected bargraph compared to the forward. 3 for example means that if forward max
//! is 1500w the reflected max will be 1500w/3 = 500w
#define REF_SCALE_PARAM 3

//! Struct which contains information of the power meter status
typedef struct {
	//! Current forward power in watts
	unsigned int curr_fwd_pwr_value;
	//! Current reflected power in watts
	unsigned int curr_ref_pwr_value;
	//! VSWR represented as an integer, 152 means 1.52:1 in SWR
	unsigned int curr_vswr_value;
	//! The update in ms of the text on the display
	unsigned int text_update_rate;
	//! The update rate in ms of the bargraph
	unsigned int bargraph_update_rate;
	//! The VSWR limit of when the radios PTT should be deactivated and the device set into ERROR mode
	unsigned int vswr_limit;
} powermeter_struct;

void powermeter_update_values(unsigned int fwd_pwr, unsigned int ref_pwr, unsigned int vswr, unsigned char type);
void powermeter_init(unsigned int text_update_rate, unsigned int bargraph_update_rate, unsigned int vswr_limit);
void powermeter_process_tasks(void);
void powermeter_1ms_tick(void);

#endif