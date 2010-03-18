/*! \file input.h \brief Input functions
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-06-11
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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

#ifndef _INPUT_H_
#define _INPUT_H_

#define COUPLER_NAME_LENGTH	10

typedef struct {
	//! The name of the coupler
	unsigned char coupler_name[COUPLER_NAME_LENGTH];
	 //! The value which the read RMS voltage should be multiplied with
	float fwd_scale_value[10]; //All HF bands + 6m
	//! The value which the read RMS voltage should be multiplied with
	float ref_scale_value[10];	//All HF bands + 6m
	//! The power limit of the coupler, high (in watts)
	unsigned int power_limit;
} struct_coupler_settings;

typedef struct {
	//! The last RED LED on the FWD bargraph is this value
	unsigned int led_fwd_power_high;
	//! The last RED LED on the REF bargraph is this value
	unsigned int led_ref_power_high;
	//! Average factor
	unsigned int average_factor;
	//! Peak level time, this is the time the peak LED is lit (ms)
	unsigned int peak_level_time;
} struct_settings;

typedef struct {
	//! Current A/D value for the forward power
	unsigned int curr_fwd_ad_value;
	//! Current A/D value for the ref power
	unsigned int curr_ref_ad_value;
	//! Current forward power (in Watts)
	unsigned int curr_fwd_power;
	//! Current reflected power (in Watts)
	unsigned int curr_ref_power;
	//! Current VSWR
	float curr_vswr;
} struct_status;

struct_coupler_settings current_coupler;
struct_status status;

float input_calculate_vswr(void);

#endif