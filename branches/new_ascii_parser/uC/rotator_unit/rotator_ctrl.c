/*! \file rotator_unit/rotator_ctrl.c \brief Rotator command execution
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-04-22
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

// BUS_BASE_ADDR can be found and be set in the makefile, this can be used
// if the current range (16 bits) is not enough.

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "init.h"
#include "board.h"
#include "ext_control.h"
#include "a2d.h"
#include "../delay.h"
#include "rotator_ctrl.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_commands.h"

/*typedef struct {
	//! The current heading of the rotator (curr sample)
	unsigned int curr_heading;
	//! The last heading of the rotator (last sample)
	unsigned int last_heading;
	//! The current heading of the rotator (curr sample A/D value)
	unsigned int curr_heading_ad_val;
	//! The last heading of the rotator (last sample A/D value)
	unsigned int last_heading_ad_val;
	//! The target heading of the rotator
	unsigned int target_heading;
	//! The target heading of the rotator A/D value
	unsigned int target_heading_ad_val;
	//! The current speed of the rotator (calculated by using the heading input)
	unsigned char rotator_speed;
} struct_rotator_status;*/


void rotator_release_break(void) {
  switch(rotator_settings.break_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_activate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_activate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_activate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_activate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_activate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_activate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_activate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_activate();
      break;
    default:
      break;      
  }
}

void rotator_activate_break(void) {
  switch(rotator_settings.break_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_deactivate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_deactivate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_deactivate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_deactivate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_deactivate();
      break;
    default:
      break;      
  }
}

void rotator_rotate_cw(void) {
  switch(rotator_settings.cw_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_activate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_activate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_activate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_activate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_activate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_activate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_activate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_activate();
      break;
    default:
      break;
  }
}

void rotator_rotate_ccw(void) {
  switch(rotator_settings.ccw_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_activate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_activate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_activate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_activate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_activate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_activate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_activate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_activate();
      break;
    default:
      break;      
  }
}

void rotator_stop(void) {
  switch(rotator_settings.cw_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_deactivate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_deactivate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_deactivate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_deactivate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_deactivate();
      break;
    default:
      break;      
  }
  
  switch(rotator_settings.ccw_output) {
    case ROTATION_OUTPUT_FET1:
      ext_ctrl_fet1_deactivate();
      break;
    case ROTATION_OUTPUT_FET2:
      ext_ctrl_fet2_deactivate();
      break;
    case ROTATION_OUTPUT_FET3:
      ext_ctrl_fet3_deactivate();
      break;
    case ROTATION_OUTPUT_FET4:
      ext_ctrl_fet4_deactivate();
      break; 
    case ROTATION_OUTPUT_RELAY1:
      ext_ctrl_relay1_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY2:
      ext_ctrl_relay2_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY3:
      ext_ctrl_relay3_deactivate();
      break;
    case ROTATION_OUTPUT_RELAY4:
      ext_ctrl_relay4_deactivate();
      break;
    default:
      break;      
  }
}

unsigned int rotator_read_heading(void) {
  if (rotator_settings.heading_input == HEADING_INPUT_POT1) {
    return(a2dConvert10bit(ADC_CH_ADC0));
  }
  else if (rotator_settings.heading_input == HEADING_INPUT_POT2) {
    return(a2dConvert10bit(ADC_CH_ADC1));
  }
  
  return(0);
}