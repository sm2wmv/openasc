/*! \file main.h \brief Main file of the front panel
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-08
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

#ifndef _MAIN_H_
#define _MAIN_H_

#define FIRMWARE_REV "0.1b\0"

#define ENABLE_TIMER0_INT() 	TIMSK0 |= (1<<OCIE0A);
#define DISABLE_TIMER0_INT() 	TIMSK0 &= ~(1<<OCIE0A);

#define BUS_STATUS_ALLOWED_TO_SEND_BIT	0
#define BUS_STATUS_PREAMBLE_FOUND_BIT		1

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	20

//! This flag is to indicate that the antenna exist
#define ANTENNA_EXIST_FLAG		0
//! Flag if there is a rotator that can be controlled
#define ANTENNA_ROTATOR_FLAG	1
//! Flag if the antenna is a multiband antenna
#define ANTENNA_IS_MULTIBAND	2
//! Flag that shows if an antenna is occupied, used for multiband antennas primary
#define ANTENNA_IN_USE_FLAG		3

//! Screensaver timeout
#define DISPLAY_SCREENSAVER_TIMEOUT	5000

//! The limit (in ms) of the radio communication timeout. If this limit is reached the 
//! radio rx buffers will be cleared
#define RADIO_RX_DATA_TIMEOUT	100

//! Indicate that radio is enabled or disabled
#define PTT_RADIO_BIT	0
//! Indicate that amp is enabled or disabled
#define PTT_AMP_BIT		1
//! Indicate that inhibit is enabled or disabled
#define INHIBIT_ENABLED_BIT	2

//! Band changes are done manually
#define BAND_CHANGE_MODE_MANUAL		0
//! Band changes are done automatically
#define BAND_CHANGE_MODE_AUTO			1

/****************************************************************/

//!POLL BUTTONS, is set when a poll on the front panel buttons should occur
#define FLAG_POLL_BUTTONS					0
//!EXT DEVICES flag is set when a poll for external devices should occur
#define FLAG_POLL_EXT_DEVICES 		1
//!Run the event first in the event queue
#define FLAG_RUN_EVENT_QUEUE			2
//!FLAG to indicate that the display should be updated
#define FLAG_UPDATE_DISPLAY				3
#define FLAG_PULSE_SENSOR_UP			4
#define FLAG_PULSE_SENSOR_DOWN		5
//! This flag indicates the state of the last BAND blink event, used to blink the LED when a new band change is in process
#define FLAG_LAST_BAND_BLINK			6
//! This flag is used to blink the antennas which can be rotated
#define FLAG_LAST_ANTENNA_BLINK		7
//! This flag is used when we wish an RX antenna change actually should happen
#define FLAG_CHANGE_RX_ANT				8


//! The poll interval of the front panel buttons (unit = ms)
#define INTERVAL_POLL_BUTTONS 50

//! The poll interval of the rotary encoder (unit = ms)
#define INTERVAL_POLL_ROTARY_ENCODER	5

//! The poll interval of the external devices as shown in board.h (unit = ms)
#define INTERVAL_POLL_EXT_DEVICES	1

//! Knob function is to select RX antenna
#define KNOB_FUNCTION_NONE				0
//! Knob function is to select RX antenna
#define KNOB_FUNCTION_RX_ANT			1
//! Knob function is to select band
#define KNOB_FUNCTION_SELECT_BAND	2
//! Knob function is to set the heading of a rotator
#define KNOB_FUNCTION_SET_HEADING	3
//! Auto select, pick the one which is most likely to be used
#define KNOB_FUNCTION_AUTO				4

//! RX antenna name length
#define RX_ANTENNA_NAME_LENGTH				15
//! RX antenna output str length
#define RX_ANTENNA_OUTPUT_STR_LENGTH	10
//! RX antenna band output str length
#define RX_ANTENNA_BAND_OUTPUT_STR_LENGTH	10

//! The max size of the antenna output str length
#define ANTENNA_TEXT_SIZE					10
//! The max size of the output combination length
#define ANTENNA_OUTPUT_COMB_SIZE	10
//! The max size of the band output str
#define BAND_OUTPUT_STR_SIZE			10
//! The max size of the sub menu array output str size
#define SUB_MENU_ARRAY_STR_SIZE		10

//! The delimiter that seperates the outputs from which address they should be sent to
#define OUTPUT_ADDR_DELIMITER			0xFF

//! Define for function status
#define FUNC_STATUS_RXANT							0
//! Define for function status, that rotation is active
#define FUNC_STATUS_ROTATE						1
//! Define for function status, to select which antenna that should be rotated
#define FUNC_STATUS_SELECT_ANT_ROTATE	2

//! DISPLAY OPTIONS
#define DISPLAY_LEVEL_LOGO		0
#define DISPLAY_LEVEL_BAND		1
#define DISPLAY_LEVEL_SUBMENU	2

#define CURRENT_DISPLAY_LOGO					0
#define CURRENT_DISPLAY_ANTENNA_INFO	1
#define CURRENT_DISPLAY_MENU_SYSTEM		2

//! The time from when a pulse sensor change occured to the actual change does happen
#define PULSE_SENSOR_RX_ANT_CHANGE_LIMIT	250

/****************************************************************/

enum enum_inhibit_state {
	INHIBIT_OK_TO_SEND,
	INHIBIT_NOT_OK_TO_SEND
};

typedef struct {
	//! The size of this structure
	unsigned int struct_size;
	//! This device address on the communication bus
	unsigned char network_address;
	//! The number of devices on the bus
	unsigned char network_device_count;
	//! Device is the master unit
	unsigned char network_device_is_master;
	//! The external keypad assignments
	unsigned char ext_key_assignments[17];
} struct_setting;

//! This struct only contains information that is temporary
typedef struct {
	unsigned int buttons_current_state;
	unsigned int buttons_last_state;
	unsigned char ext_devices_current_state;
	unsigned char ext_devices_last_state;
	
	//! Bit 0-3 = TX, Bit 4-7 = RX
	unsigned char selected_ant;	
	//! The currently selected band
	unsigned char selected_band;
	//! The variable for changing to a new band
	unsigned char new_band;
	//! The variable for the new beamheading
	unsigned int new_beamheading;
	
	//! The status of different functions, like rx ant etc, see defines above
	unsigned char function_status;
	
	/*! 0 = openASC logo, 1 = curr band level, 2 = sub menu */
	unsigned char current_display_level;
	/*! 0 = openASC logo, 1 = antenna info, 2 = menu system */
	unsigned char current_display;
	
	/*! The currently selected RX antenna, -1 if none selected */
	char selected_rx_antenna;
	/*! Knob function */
	unsigned char knob_function;
	/*! Which antenna to rotate */
	unsigned char antenna_to_rotate;
	/*! Rotator resolution chosen */
	unsigned char rotator_step_resolution;
} struct_status;

//! Settings like status but which should be saved into the EEPROM
typedef struct {
	//!The value of the LCD backlight, 0-100%	
	unsigned char lcd_backlight_value;
	//! Amp PTT output status, 1 = ON, 0 = OFF
	unsigned char amplifier_ptt_output;
	//! Radio PTT output status, 1 = ON, 0 = OFF
	unsigned char radio_ptt_output;
	//! Rotator small resolution step
	unsigned char rotator_small_step;
	//! Rotator big resolution step
	unsigned char rotator_big_step;
	//! Show if the device is inhibited or not
	unsigned char inhibit_state;
	//! Band change mode
	unsigned char band_change_mode;
} struct_runtime_settings;

/* Different antenna views */
#define VIEW_ANTENNAS 0

//! Contains different statuses of buttons etc
struct_status status;

//! Contains settings which will be saved and restored each time the box is turned on/off
struct_runtime_settings runtime_settings;

void event_add_message(void (*func), unsigned int offset, unsigned char id);
unsigned char ext_key_get_assignment(unsigned char index);
void ext_key_set_assignment(unsigned char index, unsigned char func);
		
void main_update_display(void);

void check_knob_function(void);
void set_tx_ant_leds(void);
void set_knob_function(unsigned char function);
void shutdown_device(void);

enum enum_inhibit_state main_get_inhibit_state(void);
void main_set_inhibit_state(enum enum_inhibit_state state);

#endif