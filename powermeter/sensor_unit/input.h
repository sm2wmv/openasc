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

#include "main.h"

/*! Calibration parameters */
#ifdef PICKUP_UNIT_ID_1
	//MAX rated at 3500W (recommended < 3kW) ~47dB attenuation from the RF input
	//to the input board input (47 includes the first attenuation board)
	#define PICKUP_SCALE_FWD_VALUE_160M 12.95
	#define	PICKUP_SCALE_FWD_VALUE_80M  12.97
	#define	PICKUP_SCALE_FWD_VALUE_40M  12.94
	#define	PICKUP_SCALE_FWD_VALUE_20M 	12.91
	#define	PICKUP_SCALE_FWD_VALUE_15M 	13.22
	#define	PICKUP_SCALE_FWD_VALUE_10M 	12.94

	#define PICKUP_SCALE_FWD_CONSTANT_160M	3.06
	#define PICKUP_SCALE_FWD_CONSTANT_80M		2.93
	#define PICKUP_SCALE_FWD_CONSTANT_40M		3.04
	#define PICKUP_SCALE_FWD_CONSTANT_20M		2.81
	#define PICKUP_SCALE_FWD_CONSTANT_15M		1.44
	#define PICKUP_SCALE_FWD_CONSTANT_10M		2.17

	#define PICKUP_SCALE_REF_VALUE_160M 14.46
	#define	PICKUP_SCALE_REF_VALUE_80M  14.46
	#define	PICKUP_SCALE_REF_VALUE_40M  13.93
	#define	PICKUP_SCALE_REF_VALUE_20M 	13.92
	#define	PICKUP_SCALE_REF_VALUE_15M 	13.2
	#define	PICKUP_SCALE_REF_VALUE_10M 	13.61

	#define PICKUP_SCALE_REF_CONSTANT_160M	7.22
	#define PICKUP_SCALE_REF_CONSTANT_80M		5.07
	#define PICKUP_SCALE_REF_CONSTANT_40M		4
	#define PICKUP_SCALE_REF_CONSTANT_20M		2.84
	#define PICKUP_SCALE_REF_CONSTANT_15M		6.45
	#define PICKUP_SCALE_REF_CONSTANT_10M		9.12
		
	//How many mV that we have while having no input on the power meter
	#define PICKUP_FWD_0DBM_CONST_VOLTAGE 357.5
	#define PICKUP_REF_0DBM_CONST_VOLTAGE 272.5
	
	#define PICKUP_FWD_SCALE_FACTOR	1.0
	#define PICKUP_REF_SCALE_FACTOR	1.0
#endif

#ifdef PICKUP_UNIT_ID_2
	//10kW pickup, #2. Scaling handles up to 14kW
	#define PICKUP_SCALE_FWD_VALUE_160M 13.17
	#define	PICKUP_SCALE_FWD_VALUE_80M  12.97
	#define	PICKUP_SCALE_FWD_VALUE_40M  13.18
	#define	PICKUP_SCALE_FWD_VALUE_20M 	13.17
	#define	PICKUP_SCALE_FWD_VALUE_15M 	13.11
	#define	PICKUP_SCALE_FWD_VALUE_10M 	13.24

	#define PICKUP_SCALE_FWD_CONSTANT_160M	-3.65
	#define PICKUP_SCALE_FWD_CONSTANT_80M	  -3.17
	#define PICKUP_SCALE_FWD_CONSTANT_40M		-3.96
	#define PICKUP_SCALE_FWD_CONSTANT_20M		-4.24
	#define PICKUP_SCALE_FWD_CONSTANT_15M		-4.45
	#define PICKUP_SCALE_FWD_CONSTANT_10M		-5.57

	#define PICKUP_SCALE_REF_VALUE_160M 12.91
	#define	PICKUP_SCALE_REF_VALUE_80M  12.95
	#define	PICKUP_SCALE_REF_VALUE_40M  12.92
	#define	PICKUP_SCALE_REF_VALUE_20M 	12.91
	#define	PICKUP_SCALE_REF_VALUE_15M 	12.87
	#define	PICKUP_SCALE_REF_VALUE_10M 	12.96

	#define PICKUP_SCALE_REF_CONSTANT_160M	5.23
	#define PICKUP_SCALE_REF_CONSTANT_80M		5.05
	#define PICKUP_SCALE_REF_CONSTANT_40M		4.92
	#define PICKUP_SCALE_REF_CONSTANT_20M		4.82
	#define PICKUP_SCALE_REF_CONSTANT_15M		4.37
	#define PICKUP_SCALE_REF_CONSTANT_10M		3.27
		
	//How many mV that we have while having no input on the power meter
	#define PICKUP_FWD_0DBM_CONST_VOLTAGE 355
	#define PICKUP_REF_0DBM_CONST_VOLTAGE 285
	
	#define PICKUP_FWD_SCALE_FACTOR	1.14
	#define PICKUP_REF_SCALE_FACTOR	1.14
#endif

//! Struct which contains information of the pickup type
typedef struct {
	unsigned char pickup_type; 
	//! The value which the read RMS voltage should be multiplied with
	double fwd_scale_value[6]; //All HF bands
	double fwd_scale_constant[6];
	double ref_scale_value[6]; //All HF bands
	double ref_scale_constant[6];	
} struct_coupler_settings;

typedef struct {
	//! Current A/D value for the forward power
	unsigned int curr_fwd_ad_value;
	//! Current A/D value for the ref power
	unsigned int curr_ref_ad_value;
	//! Current forward power (in Watts)
	double curr_fwd_power;
	//! Current reflected power (in Watts)
	double curr_ref_power;
	//! Current VSWR
	double curr_vswr;
	//! Current band
	unsigned char curr_band;
	//! Current freq
	unsigned int curr_freq;
} struct_status;

struct_coupler_settings current_coupler;
struct_status status;

double input_calculate_vswr(void);
void input_calculate_power(void);

#endif