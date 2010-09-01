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
	#define PICKUP_SCALE_VALUE_160M 27.29
	#define	PICKUP_SCALE_VALUE_80M  27.93
	#define	PICKUP_SCALE_VALUE_40M 	27.83; //40
	#define	PICKUP_SCALE_VALUE_30M 	27.83; //30
	#define	PICKUP_SCALE_VALUE_20M 	27.47; //20
	#define	PICKUP_SCALE_VALUE_17M 	27.47; //17
	#define	PICKUP_SCALE_VALUE_15M 	28.07; //15
	#define	PICKUP_SCALE_VALUE_12M 	28.07; //12
	#define	PICKUP_SCALE_VALUE_10M 	28.9; //10

	#define PICKUP_SCALE_CONSTANT_160M	9.88; //160
	#define PICKUP_SCALE_CONSTANT_80M		8.73; //80
	#define PICKUP_SCALE_CONSTANT_40M		8.16; //40
	#define PICKUP_SCALE_CONSTANT_30M		8.16; //30
	#define PICKUP_SCALE_CONSTANT_20M		8.6; //20
	#define PICKUP_SCALE_CONSTANT_17M		8.6; //17
	#define PICKUP_SCALE_CONSTANT_15M		9.9; //15
	#define PICKUP_SCALE_CONSTANT_12M		9.9; //12
	#define PICKUP_SCALE_CONSTANT_10M		8.9; //10
		
	//How many mV that we have while having no input on the power meter
	#define PICKUP_FWD_0DBM_CONST_VOLTAGE 620
	#define PICKUP_REF_0DBM_CONST_VOLTAGE 490
#endif

//! Struct which contains information of the pickup type
typedef struct {
	unsigned char pickup_type; 
	//! The value which the read RMS voltage should be multiplied with
	double scale_value[9]; //All HF bands
	double scale_constant[9];
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