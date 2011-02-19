/*! \file 4cx1500b/ctrl_box_front/main.h 
 *  \brief Main file of the front panel
 *  \defgroup amp_front_panel_group Front panel
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-09-04
 *  \code #include "4cx1500b/ctrl_box_front/main.h" \endcode
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

//! Screensaver timeout (in seconds)
#define DISPLAY_SCREENSAVER_TIMEOUT 300

#define DEVICE_ID_AMP_CTRL_BOARD   1
#define DEVICE_ID_AMP_CTRL_BOX     2
#define DEVICE_ID_AMP_POWERMETER   3

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

//! The poll interval of the front panel buttons (unit = ms)
#define INTERVAL_POLL_BUTTONS 50

//! The poll interval of the rotary encoder (unit = ms)
#define INTERVAL_POLL_ROTARY_ENCODER	5

//! The poll interval of the external devices as shown in board.h (unit = ms)
#define INTERVAL_POLL_EXT_DEVICES		1

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
	//! The powermeter address
	unsigned char powermeter_address[9];	//We can have one power meter address for each band
	//! The powermeter update rate on the display text (0 means it's disabled, both text and bargraph)
	unsigned int powermeter_update_rate_text;
	//! The powermeter update rate on the display bargraph
	unsigned int powermeter_update_rate_bargraph;
	//! The powermeter text view option
	unsigned char powermeter_text_view;
} struct_setting;

typedef struct {
  unsigned char lcd_backlight_value;
} struct_runtime_settings;

typedef struct {
  unsigned char ptt_state;
} struct_status;

struct_runtime_settings runtime_settings;

struct_setting settings;

struct_status status;

void main_set_device_online(unsigned char val);
void event_add_message(void (*func), unsigned int offset, unsigned char id);
void main_update_display(void);
void shutdown_device(void);
void send_ping(void);
void clear_screensaver_timer(void);

unsigned char main_get_ptt_status(void);

#endif
