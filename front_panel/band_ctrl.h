/*! \file front_panel/band_ctrl.h 
 *  \ingroup front_panel_group 
 *  \brief Band control functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/band_ctrl.h" \endcode
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
#include "../global.h"

//! Struct of band data
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
  //! Band lock configuration
  unsigned int band_lock_conf;
} struct_band;

//! Struct of the band limits
typedef struct{
	//! The low limit of the lower portion of the band
	unsigned int low_portion_low_limit;
	//! The high limit of the lower portion of the band
	unsigned int low_portion_high_limit;
	//! The low limit of the higher portion of the band
	unsigned int high_portion_low_limit;
	//! The high limit of the higher portion of the band
	unsigned int high_portion_high_limit;
} struct_band_limits;

//! The band limits, an array with size 9
struct_band_limits band_limits[9];

unsigned char band_ctrl_check_address_in_use(unsigned char addr);

void band_ctrl_load_band_limits(void);

void band_ctrl_load_band(unsigned char band);

unsigned int band_ctrl_get_low_portion_low(unsigned char band);
unsigned int band_ctrl_get_low_portion_high(unsigned char band);
unsigned int band_ctrl_get_high_portion_low(unsigned char band);
unsigned int band_ctrl_get_high_portion_high(unsigned char band);
unsigned char *band_ctrl_get_high_output_str(void);
unsigned char *band_ctrl_get_low_output_str(void);

void band_ctrl_deactivate_all(void);

unsigned int band_ctrl_get_lock_conf(void);

void band_ctrl_change_band_portion(unsigned char band_portion);
void band_ctrl_change_band(unsigned char band);
struct_band* band_ctrl_get_band_ptr(void);

#endif