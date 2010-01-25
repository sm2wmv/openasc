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

#include "event_handler.h"
#include "remote_control.h"

#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

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
	switch (button) {
		case REMOTE_CTRL_BUTTON_TX1:
			event_tx_button1_pressed();
			break;
		case REMOTE_CTRL_BUTTON_TX2:
			event_tx_button2_pressed();
			break;			
		case REMOTE_CTRL_BUTTON_TX3:
			event_tx_button3_pressed();
			break;
		case REMOTE_CTRL_BUTTON_TX4:
			event_tx_button4_pressed();
		case REMOTE_CTRL_BUTTON_RXANT1:
			event_set_rx_antenna(1);
			break;
		case REMOTE_CTRL_BUTTON_RXANT2:
			event_set_rx_antenna(2);
			break;
		case REMOTE_CTRL_BUTTON_RXANT3:
			event_set_rx_antenna(3);
			break;
		case REMOTE_CTRL_BUTTON_RXANT4:
			event_set_rx_antenna(4);
			break;
		case REMOTE_CTRL_BUTTON_RXANT5:
			event_set_rx_antenna(5);
			break;
		case REMOTE_CTRL_BUTTON_RXANT6:
			event_set_rx_antenna(6);
			break;
		case REMOTE_CTRL_BUTTON_RXANT7:
			event_set_rx_antenna(7);
			break;
		case REMOTE_CTRL_BUTTON_RXANT8:
			event_set_rx_antenna(8);
			break;
		case REMOTE_CTRL_BUTTON_RXANT9:
			event_set_rx_antenna(9);
			break;
		case REMOTE_CTRL_BUTTON_RXANT10:
			event_set_rx_antenna(10);
			break;
		default:
			break;
	}
}

/*! \brief Parse a remote control command and perform the proper action
 *  \param command The command we wish to parse
 *  \param length The length of the data
 *  \param data The data content */
void remote_control_parse_command(unsigned char command, unsigned char length, char *data) {
	switch(command) {
		case REMOTE_CONTROL_ACTIVATE_MODE:
			remote_control_activate_remote_mode();
			break;
		case REMOTE_CONTROL_DEACTIVATE_MODE:
			remote_control_deactivate_remote_mode();
		case REMOTE_CONTROL_BUTTON_PRESSED:
			if (length > 0)
				remote_control_parse_button(data[0]);
			break;
		default:
			break;
	}
}