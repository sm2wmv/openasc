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
	#define PICKUP_SCALE_FWD_VALUE_160M 26.42
	#define	PICKUP_SCALE_FWD_VALUE_80M  27.36
	#define	PICKUP_SCALE_FWD_VALUE_40M  27.26
	#define	PICKUP_SCALE_FWD_VALUE_20M 	26.74
	#define	PICKUP_SCALE_FWD_VALUE_15M 	27.01
	#define	PICKUP_SCALE_FWD_VALUE_10M 	27.36

	#define PICKUP_SCALE_FWD_CONSTANT_160M	5.1
	#define PICKUP_SCALE_FWD_CONSTANT_80M		3.13
	#define PICKUP_SCALE_FWD_CONSTANT_40M		3.13
	#define PICKUP_SCALE_FWD_CONSTANT_20M		3.74
	#define PICKUP_SCALE_FWD_CONSTANT_15M		2.55
	#define PICKUP_SCALE_FWD_CONSTANT_10M		1.06

	#define PICKUP_SCALE_REF_VALUE_160M 26.65
	#define	PICKUP_SCALE_REF_VALUE_80M  26.4
	#define	PICKUP_SCALE_REF_VALUE_40M  25.93
	#define	PICKUP_SCALE_REF_VALUE_20M 	25.45
	#define	PICKUP_SCALE_REF_VALUE_15M 	28.88
	#define	PICKUP_SCALE_REF_VALUE_10M 	26.37

	#define PICKUP_SCALE_REF_CONSTANT_160M	17.06
	#define PICKUP_SCALE_REF_CONSTANT_80M		15.14
	#define PICKUP_SCALE_REF_CONSTANT_40M		14.01
	#define PICKUP_SCALE_REF_CONSTANT_20M		16.91
	#define PICKUP_SCALE_REF_CONSTANT_15M		-7.65
	#define PICKUP_SCALE_REF_CONSTANT_10M		15.74
		
	//How many mV that we have while having no input on the power meter
	#define PICKUP_FWD_0DBM_CONST_VOLTAGE 645
	#define PICKUP_REF_0DBM_CONST_VOLTAGE 475
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
} struct_status;

struct_coupler_settings current_coupler;
struct_status status;

double input_calculate_vswr(void);
void input_calculate_power(void);

#endif