/*! \file remote_commands.h
 *  \brief Remote control mode commands
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-11
 *  \code #include "remote_commands.h" \endcode */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

#ifndef _REMOTE_COMMANDS_H_
#define _REMOTE_COMMANDS_H_

#define COMPUTER_COMM_REMOTE_SET_STATUS            0xA0
#define COMPUTER_COMM_REMOTE_BAND_INFO             0xA1
#define COMPUTER_COMM_REMOTE_ANT_TEXT              0xA2
#define COMPUTER_COMM_REMOTE_RXANT_TEXT            0xA3
#define COMPUTER_COMM_REMOTE_ANT_INFO              0xA4
#define COMPUTER_COMM_REMOTE_SUBMENU_ARRAY_TEXT    0xA5
#define COMPUTER_COMM_REMOTE_SUBMENU_STACK_TEXT    0xA6
#define COMPUTER_COMM_REMOTE_ANT_DIR_INFO          0xA7
#define COMPUTER_COMM_REMOTE_BUTTON_EVENT          0xA8
#define COMPUTER_COMM_REMOTE_ROTATE_ANTENNA        0xA9
#define COMPUTER_COMM_REMOTE_CHANGE_BAND           0xAA
#define COMPUTER_COMM_REMOTE_ROTATE_MANUAL         0xAB
#define COMPUTER_COMM_REMOTE_CLEAR_ERRORS          0xAC
#define COMPUTER_COMM_REMOTE_PTT_STATUS            0xAD
#define COMPUTER_COMM_SET_SUBMENU_OPTION           0xAE
#define COMPUTER_COMM_REBOOT                       0xAF

#endif