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
	
	double temp = 0;
	if ((status.curr_fwd_power > 0.1) && (status.curr_ref_power > 0.1)) {
		double constant = (double)sqrt((double)status.curr_ref_power/(double)status.curr_fwd_power);
	
		temp = (1.0f+constant)/(1.0f-constant);
	
		return(temp);
	}
	
	
	return(0);
}

void input_calculate_power(void) {
	double fwd_volt;
	double ref_volt;
	double fwd_dbm_val;
	double ref_dbm_val;

	fwd_volt = ((2.56f/1.024)*status.curr_fwd_ad_value);
	ref_volt = ((2.56f/1.024)*status.curr_ref_ad_value);

	fwd_dbm_val = (fwd_volt-620)/current_coupler.scale_value[4] - current_coupler.scale_constant[4];
	ref_dbm_val = (ref_volt-490)/current_coupler.scale_value[4] - current_coupler.scale_constant[4];
			
	status.curr_fwd_power = pow(10,fwd_dbm_val/10)/1000;
	status.curr_ref_power = pow(10,ref_dbm_val/10)/1000;
}
