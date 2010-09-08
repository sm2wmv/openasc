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

	//TODO: Try to optimize the calculations a bit
	fwd_volt = ((2.56f/1.024)*status.curr_fwd_ad_value);
	ref_volt = ((2.56f/1.024)*status.curr_ref_ad_value);
	
	fwd_dbm_val = (fwd_volt-PICKUP_FWD_0DBM_CONST_VOLTAGE)/current_coupler.fwd_scale_value[status.curr_band] - current_coupler.fwd_scale_constant[status.curr_band];
	ref_dbm_val = (ref_volt-PICKUP_REF_0DBM_CONST_VOLTAGE)/current_coupler.ref_scale_value[status.curr_band] - current_coupler.ref_scale_constant[status.curr_band];
			
	status.curr_fwd_power = pow(10,fwd_dbm_val/10)/1000;
	status.curr_ref_power = pow(10,ref_dbm_val/10)/1000;
	
/*	#ifdef CAL_MODE
		printf("FWD VOLT: %.1fmV\n",fwd_volt);
		printf("REF VOLT: %.1fmV\n",ref_volt);
		
		printf("\n\n");
		printf("FWD dBm: %.1fdBm\n",fwd_dbm_val);
		printf("REF dBm: %.1fdBm\n",ref_dbm_val);
		
		printf("FWD dBm(2): %.1fdBm\n",fwd_dbm_val-50);
		printf("REF dBm(2): %.1fdBm\n",ref_dbm_val-50);		
	
		
		printf("FREQ: %ukHz\n",status.curr_freq);
		printf("BAND: %i\n",status.curr_band);
		printf("FWD PWR: %.1f\n",status.curr_fwd_power);
		printf("REF PWR: %.1f\n",status.curr_fwd_power);
		printf("VSWR: %.2f\n",status.curr_vswr);
	#endif*/
}
