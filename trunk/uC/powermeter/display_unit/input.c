/*! \file input.c \brief Input functions
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
#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>

#include "input.h"

double input_calculate_vswr(void) {
	if (status.curr_fwd_power > 0) {
		double constant = (double)sqrt((double)status.curr_ref_power/(double)status.curr_fwd_power);
	
		return((1.0f+constant)/(1.0f-constant));
	}
	
	return(0);
}

void input_calculate_power(void) {
	//TODO: Fix freq counter, also change calculations to make them easier for uC
	long temp = ((long)(status.curr_fwd_ad_value) * (long)(status.curr_fwd_ad_value)) / current_coupler.fwd_scale_value[0];
	status.curr_fwd_power = (unsigned int)temp;
	temp = ((long)(status.curr_ref_ad_value) * (long)(status.curr_ref_ad_value)) / current_coupler.ref_scale_value[0];
	status.curr_ref_power = (unsigned int)temp;
}

void pulse_sensor_up(void) {
	
}

void pulse_sensor_down(void) {
	
}
