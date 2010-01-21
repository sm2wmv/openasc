/*! \file interrupt_handler.h \brief Handles different external interrupts
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-26
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

#ifndef _INTERRUPT_HANDLER_H_
#define _INTERRUPT_HANDLER_H_

int ih_poll_buttons(void);
unsigned char ih_poll_ext_devices(void);

#endif