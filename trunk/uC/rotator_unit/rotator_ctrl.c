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

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

double rotator_get_multiplier(void) {
	return((double)((double)(rotator_settings.rotation_max) - (double)(rotator_settings.rotation_min)) / (double)(rotator_settings.rotation_degree_max));
}

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

void rotator_rotate_to(int heading) {
	//TODO: Implement this
	
	rotator_rotate_cw();
}

void rotator_release_break(void) {
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_FET1))
		ext_ctrl_fet1_activate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_FET2))
		ext_ctrl_fet2_activate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY1))
		ext_ctrl_relay1_activate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY2))
		ext_ctrl_relay2_activate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY3))
		ext_ctrl_relay3_activate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY4))
		ext_ctrl_relay4_activate();
}

void rotator_activate_break(void) {
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_FET1))
		ext_ctrl_fet1_deactivate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_FET2))
		ext_ctrl_fet2_deactivate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY1))
		ext_ctrl_relay1_deactivate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY2))
		ext_ctrl_relay2_deactivate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY3))
		ext_ctrl_relay3_deactivate();
	if (rotator_settings.break_output & (1<<ROTATION_OUTPUT_RELAY4))
		ext_ctrl_relay4_deactivate();
}

void rotator_rotate_cw(void) {
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_FET1))
			ext_ctrl_fet1_activate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_FET2))
			ext_ctrl_fet2_activate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY1))
			ext_ctrl_relay1_activate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY2))
			ext_ctrl_relay2_activate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY3))
			ext_ctrl_relay3_activate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY4))
			ext_ctrl_relay4_activate();
}

void rotator_rotate_ccw(void) {
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_FET1))
			ext_ctrl_fet1_activate();
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_FET2))
			ext_ctrl_fet2_activate();
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY1))
			ext_ctrl_relay1_activate();
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY2))
			ext_ctrl_relay2_activate();
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY3))
			ext_ctrl_relay3_activate();
		if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY4))
			ext_ctrl_relay4_activate();
}

void rotator_stop_cw(void) {
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_FET1))
			ext_ctrl_fet1_deactivate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_FET2))
			ext_ctrl_fet2_deactivate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY1))
			ext_ctrl_relay1_deactivate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY2))
			ext_ctrl_relay2_deactivate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY3))
			ext_ctrl_relay3_deactivate();
		if (rotator_settings.cw_output & (1<<ROTATION_OUTPUT_RELAY4))
			ext_ctrl_relay4_deactivate();
}

void rotator_stop_ccw(void) {
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_FET1))
		ext_ctrl_fet1_deactivate();
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_FET2))
		ext_ctrl_fet2_deactivate();
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY1))
		ext_ctrl_relay1_deactivate();
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY2))
		ext_ctrl_relay2_deactivate();
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY3))
		ext_ctrl_relay3_deactivate();
	if (rotator_settings.ccw_output & (1<<ROTATION_OUTPUT_RELAY4))
		ext_ctrl_relay4_deactivate();
}