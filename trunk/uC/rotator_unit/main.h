/*! \file rotator_unit/main.h \brief Main file of the rotator unit
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

#ifndef _MAIN_H_
#define _MAIN_H_


//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	10

//! Run the event first in the event queue
#define FLAG_RUN_EVENT_QUEUE			0
//! The rotator is currently standing still
#define FLAG_NO_ROTATION					1
//! The current preset rotates over north
#define FLAG_ROTATOR_PRESET_OVER_NORTH  2
//! The rotator is being rotated CW
#define FLAG_ROTATION_CW					3
//! The rotator is being rotated CCW
#define FLAG_ROTATION_CCW					4
//! The rotator rotates over south
#define FLAG_ROTATES_OVER_SOUTH   5
//! The preset is active
#define FLAG_ROTATOR_PRESET_ACTIVE  6
//! The current preset rotates over north
#define FLAG_ROTATOR_PRESET_OVER_NORTH  7

//! The heading input is A/D input #1
#define HEADING_INPUT_POT1					1
//! The heading input is A/D input #2
#define HEADING_INPUT_POT2					2
//! The heading input is the pulse sensor input (active high)
#define HEADING_INPUT_PULSE_ACT_HI	3
//! The heading input is the pulse sensor input (active low)
#define HEADING_INPUT_PULSE_ACT_LO	4

//! Rotator mode: HARDWIRED
#define ROTATOR_MODE_HARDWIRED	1
//! Rotator mode: GS-232 (via RS232)
#define ROTATOR_MODE_GS232			2
//! Rotator mode: DCU1 (via RS232)
#define ROTATOR_MODE_DCU1				3

//! Note that one can have several outputs activated at the same time!
//! ------------------------------------------------------------------
//! Rotation output (hardwired mode) - FET1
#define ROTATION_OUTPUT_NONE    0
//! Rotation output (hardwired mode) - FET1
#define ROTATION_OUTPUT_FET1		1
//! Rotation output (hardwired mode) - FET2
#define ROTATION_OUTPUT_FET2		2
//! Rotation output (hardwired mode) - FET3
#define ROTATION_OUTPUT_FET3		3
//! Rotation output (hardwired mode) - FET4
#define ROTATION_OUTPUT_FET4		4
//! Rotation output (hardwired mode) - RELAY1
#define ROTATION_OUTPUT_RELAY1	5
//! Rotation output (hardwired mode) - RELAY2
#define ROTATION_OUTPUT_RELAY2	6
//! Rotation output (hardwired mode) - RELAY3
#define ROTATION_OUTPUT_RELAY3	7
//! Rotation output (hardwired mode) - RELAY4
#define ROTATION_OUTPUT_RELAY4	8

//! The rotator turns over south
#define ROTATOR_PATH_SOUTH 0
//! The rotator turns over north
#define ROTATOR_PATH_NORTH 1

#define EVENT_QUEUE_ROTATE_CW_ID			0
#define EVENT_QUEUE_ROTATE_CCW_ID			1
#define EVENT_QUEUE_ROTATE_STOP_ID		2
#define EVENT_QUEUE_RELEASE_BREAK_ID	3
#define EVENT_QUEUE_ACTIVATE_BREAK_ID	4
#define EVENT_QUEUE_ROTATION_ALLOWED  5

#define AD_VAL_POLL_INTERVAL						100
#define ROTATOR_STATUS_UPDATE_INTERVAL	500

#define FLAG_POLL_AD			0
#define FLAG_SEND_STATUS	1

//! Struct of the settings
typedef struct {
	//! Which one of the inputs is used to sense the beamheading (hardwired mode)
	unsigned char heading_input;
	//! Which kind of mode the rotator card is in
	unsigned char rotator_mode;
	//! Which outputs are used for CW rotation (hardwired mode)
	unsigned int cw_output;
	//! Which outputs are used for CCW rotation (hardwired mode)
	unsigned int ccw_output;
	//! Break release outputs (hardwired mode)
	unsigned int break_output;
	//! Rotation delay, in seconds. This is the time after a rotation has been
	//! excecuted before another rotation can start
	unsigned char rotation_delay;
	//! The end point of the rotator heading
	unsigned int rotation_stop_angle;
	//! The starting point of the rotator heading
	unsigned int rotation_start_angle;
	//! Start rotation input value, min, this should be set at rotation_start_angle
	unsigned int rotation_min;
	//! Start rotation input value, max
	unsigned int rotation_max;
	//! Rotator break delay, the delay between rotation and the break is put in/out (x100 ms)
	unsigned char rotation_break_delay;
  //! Rotator rotates over south or north, 0 = south, 1 = north
  unsigned char rotates_over;
} struct_settings;

//! Struct of the current rotator status
typedef struct {
	//! The current heading of the rotator (curr sample)
	int curr_heading;
	//! The last heading of the rotator (last sample)
	int last_heading;
	//! The current heading of the rotator (curr sample A/D value)
	unsigned int curr_heading_ad_val;
	//! The last heading of the rotator (last sample A/D value)
	unsigned int last_heading_ad_val;
	//! The target heading of the rotator
	int target_heading;
	//! The target heading of the rotator A/D value
	unsigned int target_heading_ad_val;
} struct_rotator_status;

struct_rotator_status rotator_status;
struct_settings rotator_settings;

void rotator_set_no_rotation(void);
void bus_parse_message(void);
void event_add_message(void (*func), unsigned int offset, unsigned char id);
unsigned char read_ext_addr(void);
void event_run(void);

void main_set_preset_active(void);

#endif
