/*! \file computer_interface.h \brief Interface towards the computer
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-05-01
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

#ifndef _COMPUTER_INTERFACE_H_
#define _COMPUTER_INTERFACE_H_

void computer_interface_init(void);
void computer_interface_send_data(void);
void computer_interface_parse_data(void);
void computer_interface_activate_setup(void);
void computer_interface_deactivate_setup(void);

unsigned char computer_interface_is_active(void);

#endif