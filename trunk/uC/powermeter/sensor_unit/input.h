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
	#define PICKUP_SCALE_FWD_VALUE_160M 26.64
	#define	PICKUP_SCALE_FWD_VALUE_80M  26.76
	#define	PICKUP_SCALE_FWD_VALUE_40M  27.18
	#define	PICKUP_SCALE_FWD_VALUE_20M 	27.8
	#define	PICKUP_SCALE_FWD_VALUE_15M 	27.86
	#define	PICKUP_SCALE_FWD_VALUE_10M 	27.78

	#define PICKUP_SCALE_FWD_CONSTANT_160M	3.51
	#define PICKUP_SCALE_FWD_CONSTANT_80M		3.07
	#define PICKUP_SCALE_FWD_CONSTANT_40M		2.12
	#define PICKUP_SCALE_FWD_CONSTANT_20M		0.79
	#define PICKUP_SCALE_FWD_CONSTANT_15M		0.14
	#define PICKUP_SCALE_FWD_CONSTANT_10M		-0.41

	#define PICKUP_SCALE_REF_VALUE_160M 29.23
	#define	PICKUP_SCALE_REF_VALUE_80M  27.83
	#define	PICKUP_SCALE_REF_VALUE_40M  27.03
	#define	PICKUP_SCALE_REF_VALUE_20M 	26.88
	#define	PICKUP_SCALE_REF_VALUE_15M 	27.03
	#define	PICKUP_SCALE_REF_VALUE_10M 	27.56

	#define PICKUP_SCALE_REF_CONSTANT_160M	16.06
	#define PICKUP_SCALE_REF_CONSTANT_80M		16.78
	#define PICKUP_SCALE_REF_CONSTANT_40M		15.69
	#define PICKUP_SCALE_REF_CONSTANT_20M		13.46
	#define PICKUP_SCALE_REF_CONSTANT_15M		13.2
	#define PICKUP_SCALE_REF_CONSTANT_10M		13.66
		
	//How many mV that we have while having no input on the power meter
	#define PICKUP_FWD_0DBM_CONST_VOLTAGE 655
	#define PICKUP_REF_0DBM_CONST_VOLTAGE 510
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