/*! \file band_ctrl.h \brief Band control functions
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-06-21
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

#ifndef _BAND_CTRL_H_
#define _BAND_CTRL_H_

#include "main.h"

typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! The low limit of the lower portion of the band
	unsigned int low_portion_low_limit;
	//! The high limit of the lower portion of the band
	unsigned int low_portion_high_limit;
	//! The low limit of the higher portion of the band
	unsigned int high_portion_low_limit;
	//! The high limit of the higher portion of the band
	unsigned int high_portion_high_limit;
	//! The length of the high output str
	unsigned char band_high_output_str_length;
	//! The length of the low output str
	unsigned char band_low_output_str_length;
	//! These outputs are activated when you enter the high area of a band
	unsigned char band_high_output_str[BAND_OUTPUT_STR_SIZE];
	//! These outputs are activated when you enter the low area of a band
	unsigned char band_low_output_str[BAND_OUTPUT_STR_SIZE];
} struct_band;

unsigned int band_ctrl_get_low_portion_low(void);
unsigned int band_ctrl_get_low_portion_high(void);
unsigned int band_ctrl_get_high_portion_low(void);
unsigned int band_ctrl_get_high_portion_high(void);
unsigned char *band_ctrl_get_high_output_str(void);
unsigned char *band_ctrl_get_low_output_str(void);

void band_ctrl_deactivate_all(void);

void band_ctrl_change_band(unsigned char band);

#endif