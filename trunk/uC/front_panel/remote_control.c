/*! \file front_panel/remote_control.c
 *  \brief Remote control of the openASC box
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/remote_control.c" \endcode
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>x
#include <avr/interrupt.h>
#include <string.h>

#include "../global.h"
#include "event_handler.h"
#include "remote_control.h"
#include "antenna_ctrl.h"

//! Flag that the remote control is active
#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

//! Flags used in the remote control
unsigned char remote_control_flags;

/*! \brief Activate the remote control mode */
void remote_control_activate_remote_mode(void) {
	remote_control_flags |= (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Deactivate the remote control mode */
void remote_control_deactivate_remote_mode(void) {
	remote_control_flags &= ~(1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Get the current remote control mode 
 *  \return 1 if remote mode is active, 0 if it is not active */
unsigned char remote_control_get_remote_mode(void) {
	return((remote_control_flags & (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE)) >> FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Parse a button press event, will perform an action depending on which button we wish to press
 *  \param button The button we wish to press */
void remote_control_parse_button(unsigned char button) {
	event_process_task(button);
}

/*! \brief Parse a remote control command and perform the proper action
 *  \param command The command we wish to parse
 *  \param length The length of the data
 *  \param data The data content */
void remote_control_parse_command(unsigned char command, unsigned char length, char *data) {
	if (command == REMOTE_CONTROL_ACTIVATE_MODE) {
		remote_control_activate_remote_mode();
		
		internal_comm_add_tx_message(INT_COMM_PC_CTRL_ACK,0,0);
	}
	else if (command == REMOTE_CONTROL_DEACTIVATE_MODE) {
		remote_control_deactivate_remote_mode();
		
		internal_comm_add_tx_message(INT_COMM_PC_CTRL_ACK,0,0);
	}
	else if (command == REMOTE_CONTROL_BUTTON_PRESSED) {
		if (length > 0)
			remote_control_parse_button(data[0]);
		
		internal_comm_add_tx_message(INT_COMM_PC_CTRL_ACK,0,0);
	}
	else if (command == REMOTE_CONTROL_RX_ANT_TEXT) {
		char buf[RX_ANTENNA_NAME_LENGTH+3];
		unsigned char buffer_pos=0;

		buffer_pos=0;
		buf[buffer_pos++] = REMOTE_CONTROL_RX_ANT_TEXT;
			
		buf[buffer_pos++] = data[0]; //Antenna index
		memcpy(&buf[buffer_pos],antenna_ctrl_get_rx_antenna_name(data[0]),strlen(antenna_ctrl_get_rx_antenna_name(data[0])));
		buffer_pos += strlen(antenna_ctrl_get_rx_antenna_name(data[0]));
		buf[buffer_pos] = 0;

		internal_comm_add_tx_message(INT_COMM_PC_CTRL, buffer_pos, &buf);
	}
	else if (command == REMOTE_CONTROL_ANT_TEXT) {
		unsigned char buf[ANTENNA_TEXT_SIZE+3];
		unsigned char buffer_pos=0;
		
		buf[buffer_pos++] = REMOTE_CONTROL_ANT_TEXT;
			
		buf[buffer_pos++] = data[0];	//Antenna index
		memcpy(&buf[buffer_pos],antenna_ctrl_get_antenna_text(data[0]),antenna_ctrl_get_antenna_text_length(data[0]));
		buffer_pos += antenna_ctrl_get_antenna_text_length(data[0]);
		buf[buffer_pos] = 0;
			
		internal_comm_add_tx_message(INT_COMM_PC_CTRL, buffer_pos, &buf);
	}
	else if (command == REMOTE_CONTROL_CHANGE_BAND) {
		if (data[0] <= BAND_10M) {
			main_set_band_change_mode(BAND_CHANGE_MODE_MANUAL);
			main_set_new_band(data[0]);
		}
		else {
			main_set_band_change_mode(BAND_CHANGE_MODE_AUTO);
		}
		
		internal_comm_add_tx_message(INT_COMM_PC_CTRL_ACK,0,0);
	}
	else if (command == REMOTE_CONTROL_GET_STATUS) {
		unsigned char buf[4];
		unsigned char buffer_pos = 0;
		
		buf[buffer_pos++] = REMOTE_CONTROL_GET_STATUS;
		buf[buffer_pos++] = main_get_current_band();
		buf[buffer_pos++] = antenna_ctrl_antenna_selected();
		buf[buffer_pos++] = antenna_ctrl_rx_antenna_selected();
		
		internal_comm_add_tx_message(INT_COMM_PC_CTRL, 4, &buf);
	}
}