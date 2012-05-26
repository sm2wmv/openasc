/*! \file     general_io/sk3w_power_amp_ctrl/controller.c
 *  \ingroup  general_io_group
 *  \brief    Handling of controllers (boxes)
 *  \author   Tobias Blomberg, SN0SVX
 *  \date     2012-05-23
 * 
 * Contains functions for handling controllers and actions performed by
 * controllers.
 */
//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
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

#include <string.h>
#include <avr/io.h>

#include <global.h>

#include "controller.h"
#include "pa.h"

//! The maximun number of controller that we can handle
#define MAX_CONTROLLERS 6

//! Selected band for each controller
static Controller ctrlrs[MAX_CONTROLLERS];


void controller_init(void) {
  for (int i=0; i<MAX_CONTROLLERS; ++i) {
    ctrlrs[i].addr = 0;
    ctrlrs[i].band = BAND_UNDEFINED;
  }
}

int8_t controller_set_band(uint8_t ctrlr, uint8_t fm_addr, uint8_t band) {
	if ((ctrlr >= MAX_CONTROLLERS) || (band > BAND_MAX)) {
		return -1;
	}
	if (band != BAND_UNDEFINED) {
    for (int i=0; i<MAX_CONTROLLERS; ++i) {
      if (ctrlrs[i].band == band) {
        return -1;
      }
    }
  }
	if (ctrlrs[ctrlr].band != BAND_UNDEFINED) {
		pa_clear_controller(ctrlrs[ctrlr].band);
	}
  ctrlrs[ctrlr].band = band;
  if (band != BAND_UNDEFINED) {
    pa_set_controller(band, ctrlr);
    ctrlrs[ctrlr].addr = fm_addr;
  }
  else {
    ctrlrs[ctrlr].addr = 0;
  }
  return 0;
}

uint8_t controller_band(uint8_t ctrlr) {
  if (ctrlr >= MAX_CONTROLLERS) {
    return BAND_UNDEFINED;
  }
  return ctrlrs[ctrlr].band;
}

uint8_t controller_band_list(const Controller **ctrlr_list) {
  *ctrlr_list = ctrlrs;
  return sizeof(ctrlrs)/sizeof(*ctrlrs);
}

int8_t controller_toggle_mains(uint8_t ctrlr) {
  if (ctrlr >= MAX_CONTROLLERS) {
    return -1;
  }
  return pa_toggle_mains(ctrlrs[ctrlr].band);
}

void controller_set_tx_active(uint8_t ctrlr, int8_t is_active) {
  if (ctrlr >= MAX_CONTROLLERS) {
    return;
  }
  pa_set_tx_active(ctrlrs[ctrlr].band, is_active);
}

