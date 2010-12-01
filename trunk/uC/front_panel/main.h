/*! \file front_panel/main.h 
 *  \brief Main file of the front panel
 *  \defgroup front_panel_group Front panel
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-09-04
 *  \code #include "front_panel/main.h" \endcode
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

//! The current firmware revision nr
#define FIRMWARE_REV "0.3b\0"

//! Macro to enable timer 0 interrupt
#define ENABLE_TIMER0_INT() 	TIMSK0 |= (1<<OCIE0A);
//! Macro to disable timer 0 interrupt
#define DISABLE_TIMER0_INT() 	TIMSK0 &= ~(1<<OCIE0A);

//! Flag to indicate that the bus is allowed to transmit
#define BUS_STATUS_ALLOWED_TO_SEND_BIT	0
//! Flag to indicate that a preamble has been found in the bus
#define BUS_STATUS_PREAMBLE_FOUND_BIT		1

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	25

//! This flag is to indicate that the antenna exist
#define ANTENNA_EXIST_FLAG		0
//! Flag if there is a rotator that can be controlled
#define ANTENNA_ROTATOR_FLAG	1
//! Flag if the antenna is a multiband antenna - Not implemented
#define ANTENNA_IS_MULTIBAND	2
//! Flag that shows if an antenna is occupied, used for multiband antennas primary - Not implemented
#define ANTENNA_IN_USE_FLAG		3

//! Screensaver timeout (in seconds)
#define DISPLAY_SCREENSAVER_TIMEOUT 300

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
//! Poll the pulse sensor
#define FLAG_POLL_PULSE_SENSOR		4
//! This flag indicates the state of the last BAND blink event, used to blink the LED when a new band change is in process
#define FLAG_LAST_BAND_BLINK			5
//! This flag is used to blink the antennas which can be rotated
#define FLAG_LAST_ANTENNA_BLINK		6
//! This flag is used to trigger an RX antenna change, after a certain amount of time which is set with the flag below
#define FLAG_CHANGE_RX_ANT				7
//! Works together with the above flag, but this is set when the actual antenna change should occur
#define FLAG_PROCESS_RX_ANT_CHANGE	8
//! Blink the band led
#define FLAG_BLINK_BAND_LED				9
//! Indicate that we should poll the radio
#define FLAG_POLL_RADIO						10
//! This flag is set to indicate that we have changed the sub menu
#define FLAG_CHANGE_SUBMENU					11
//! This flag is to indicate that a sub menu change should occur, ie sent out on the bus
#define FLAG_PROCESS_SUBMENU_CHANGE	12

//! The poll interval of the front panel buttons (unit = ms)
#define INTERVAL_POLL_BUTTONS 50

//! The poll interval of the rotary encoder (unit = ms)
#define INTERVAL_POLL_ROTARY_ENCODER	5

//! The poll interval of the external devices as shown in board.h (unit = ms)
#define INTERVAL_POLL_EXT_DEVICES		1

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
//! Knob function set submenu option
#define KNOB_FUNCTION_SET_SUBMENU	5

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
#define BAND_OUTPUT_STR_SIZE			15
//! The max size of the sub menu array output str size
#define SUB_MENU_ARRAY_STR_SIZE		10
//! The size of the name of a 4-SQ
#define SUB_MENU_ARRAY_NAME_SIZE  4
//! The max size of the sub menu array output str size
#define SUB_MENU_STACK_STR_SIZE 10
//! The size of the name of a stack
#define SUB_MENU_STACK_NAME_SIZE 10

//! The delimiter that seperates the outputs from which address they should be sent to
#define OUTPUT_ADDR_DELIMITER			0xFF

//! Define for function status
#define FUNC_STATUS_RXANT							0
//! Define for function status, that rotation is active
#define FUNC_STATUS_ROTATE						1
//! Define for function status, to select which antenna that should be rotated
#define FUNC_STATUS_SELECT_ANT_ROTATE	2
//! Define for function status, to select sub menu
#define FUNC_STATUS_SUBMENU						3

//! Display level openASC logo
#define DISPLAY_LEVEL_LOGO		0
//! Display level current band
#define DISPLAY_LEVEL_BAND		1
//! Display level sub menu
#define DISPLAY_LEVEL_SUBMENU	2

//! Current display is the openASC logo
#define CURRENT_DISPLAY_LOGO					0
//! Current display is the antenna information
#define CURRENT_DISPLAY_ANTENNA_INFO	1
//! Current display is the menu system
#define CURRENT_DISPLAY_MENU_SYSTEM		2
//! Current display is the shutdown in progress view
#define CURRENT_DISPLAY_SHUTDOWN_VIEW	3
//! Current display power meter view
#define CURRENT_DISPLAY_POWERMETER_VIEW 4
//! Current display is some selection of some sort, for example rotator direction
#define CURRENT_DISPLAY_SETTING	5

//! The time from when a pulse sensor change occured to the actual change does happen, in ms
#define PULSE_SENSOR_RX_ANT_CHANGE_LIMIT	250

//! The time from when a pulse sensor change occured to the actual change does happen, in ms
#define PULSE_SENSOR_SUBMENU_CHANGE_LIMIT	250

//! Sub menu type NONE
#define SUBMENU_NONE				0
//! Sub menu type 4-SQ
#define SUBMENU_VERT_ARRAY	1
//! Sub menu type stack
#define SUBMENU_STACK				2

/****************************************************************/

//! Different inhibit states
enum enum_inhibit_state {
	//! Inhibit state, OK to start a transmission
	INHIBIT_OK_TO_SEND,
  //! Inhibit state, NOT OK to start a transmission
	INHIBIT_NOT_OK_TO_SEND,
  //! Inhibit state, NOT OK to start a transmission, Radio is in TX
 	INHIBIT_NOT_OK_TO_SEND_RADIO_TX
};

//! Settings struct
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
	//! The powermeter address
	unsigned char powermeter_address[9];	//We can have one power meter address for each band
	//! The powermeter VSWR alarm limit (250 means 2.5:1)
	unsigned int powermeter_vswr_limit;
	//! The powermeter update rate on the display text (0 means it's disabled, both text and bargraph)
	unsigned int powermeter_update_rate_text;
	//! The powermeter update rate on the display bargraph
	unsigned int powermeter_update_rate_bargraph;
	//! The powermeter text view option
	unsigned char powermeter_text_view;
	//! Which PTT input of various boards this openASC box is configured to use, this is sent out in PING messages
	//! and is saved in the various boxes so that they are aware of which TX ACTIVE input they should listen to
	//! 0 = None, 1-7 inputs
	unsigned char ptt_interlock_input;
	//! AUX1 button function, same kind of function indexes as the external key assignments
	unsigned char aux1_button_func;
	//! AUX2 button function, same kind of function indexes as the external key assignments
	unsigned char aux2_button_func;
} struct_setting;

//! This struct only contains information that is temporary
typedef struct {
	//! The current state of the buttons
	unsigned int buttons_current_state;
	//! The last state of the buttons
	unsigned int buttons_last_state;
	//! The current state of the ext devices
	unsigned char ext_devices_current_state;
	//! The last state of the ext devices
	unsigned char ext_devices_last_state;
	
	//! Bit 0-3 = TX, Bit 4-7 = RX
	unsigned char selected_ant;	
	//! The currently selected band
	unsigned char selected_band;
	//! The variable for changing to a new band
	unsigned char new_band;
	//! CURRENT Band portion selected
	unsigned char current_band_portion;
	//! NEW Band portion selected
	unsigned char new_band_portion;
	
	//! The variable for the new beamheading
	unsigned int new_beamheading;
	
	//! The status of different functions, like rx ant etc, see defines above
	unsigned char function_status;
	
	/*! 0 = openASC logo, 1 = curr band level, 2 = sub menu */
	unsigned char current_display_level;
	/*! 0 = openASC logo, 1 = antenna info, 2 = menu system, 3 = shutdown view */
	unsigned char current_display;
	/*! Previous display */
	unsigned char prev_display;
	
	/*! The currently selected RX antenna, -1 if none selected */
	unsigned char selected_rx_antenna;
	/*! Knob function */
	unsigned char knob_function;
	/*! The previous knob function */
	unsigned char knob_prev_function;
	/*! Which antenna to rotate */
	unsigned char antenna_to_rotate;
	/*! Rotator resolution chosen */
	unsigned char rotator_step_resolution;
	
	/*! The last RX antenna used */
	unsigned char last_rx_antenna;
	
	/*! The sub menu antenna index we are changing */
	unsigned char sub_menu_antenna_index;
} struct_status;

//! Settings like status but which should be saved into the EEPROM
typedef struct {
	//!The value of the LCD backlight, 0-100%	
	unsigned char lcd_backlight_value;
	//! Amp PTT output status, 1 = ON, 0 = OFF
	unsigned char amplifier_ptt_output;
	//! Radio PTT output status, 1 = ON, 0 = OFF
	unsigned char radio_ptt_output;
	//! Show if the device is inhibited or not
	unsigned char inhibit_state;
	//! Band change mode
	unsigned char band_change_mode;
	//! Powermeter address
	unsigned char powermeter_address;
} struct_runtime_settings;

/* Different views */
//! Flag used if we wish to view antennas
#define VIEW_ANTENNAS 0

//! Contains different statuses of buttons etc
struct_status status;

//! Contains settings which will be saved and restored each time the box is turned on/off
struct_runtime_settings runtime_settings;

void main_update_ptt_status(void);

void main_save_settings(void);

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

void send_ping(void);

void main_set_device_online(unsigned char state);
unsigned char main_get_aux_button(unsigned char buttonIndex);
unsigned char main_band_change_ok(void);
void clear_screensaver_timer(void);

unsigned char main_get_powermeter_address(unsigned char band);

#endif
