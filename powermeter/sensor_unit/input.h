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

typedef struct {
	unsigned char pickup_type; 
	//! The value which the read RMS voltage should be multiplied with
	unsigned int fwd_scale_value[10]; //All HF bands + 6m
	//! The value which the read RMS voltage should be multiplied with
	unsigned int ref_scale_value[10];	//All HF bands + 6m
} struct_coupler_settings;

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
	double curr_vswr;
} struct_status;

struct_coupler_settings current_coupler;
struct_status status;

double input_calculate_vswr(void);
void input_calculate_power(void);

#endif