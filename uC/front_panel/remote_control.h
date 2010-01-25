/*! \file front_panel/remote_control.h
 *  \brief Remote control of the openASC box
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/remote_control.h" \endcode
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

#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_

void remote_control_activate_remote_mode(void);
void remote_control_deactivate_remote_mode(void);
unsigned char remote_control_get_remote_mode(void);
void remote_control_parse_command(unsigned char command, unsigned char length, char *data);

void remote_control_parse_button(unsigned char button);

#define REMOTE_CONTROL_ACTIVATE_MODE		0x01
#define REMOTE_CONTROL_DEACTIVATE_MODE	0x02

//! A button should be pressed
#define REMOTE_CONTROL_BUTTON_PRESSED		0x10
//! Command for sending rx antenna button texts
#define REMOTE_CONTROL_RX_ANT_TEXT			0x11

/* START OF BUTTON PRESSED DEFINES */
#define REMOTE_CTRL_BUTTON_TX1			1
#define REMOTE_CTRL_BUTTON_TX2			2
#define REMOTE_CTRL_BUTTON_TX3			3
#define REMOTE_CTRL_BUTTON_TX4			4

#define REMOTE_CTRL_BUTTON_RX1			5
#define REMOTE_CTRL_BUTTON_RX2			6
#define REMOTE_CTRL_BUTTON_RX3			7
#define REMOTE_CTRL_BUTTON_RX4			8

#define REMOTE_CTRL_BUTTON_RXANT1		9
#define REMOTE_CTRL_BUTTON_RXANT2		10
#define REMOTE_CTRL_BUTTON_RXANT3		11
#define REMOTE_CTRL_BUTTON_RXANT4		12
#define REMOTE_CTRL_BUTTON_RXANT5		13
#define REMOTE_CTRL_BUTTON_RXANT6		14
#define REMOTE_CTRL_BUTTON_RXANT7		15
#define REMOTE_CTRL_BUTTON_RXANT8		16
#define REMOTE_CTRL_BUTTON_RXANT9		17
#define REMOTE_CTRL_BUTTON_RXANT10	18

#define REMOTE_CTRL_BUTTON_TXRX_MODE		19
#define REMOTE_CTRL_BUTTON_RXANT				20
/* END OF BUTTON PRESSED DEFINES */

#endif