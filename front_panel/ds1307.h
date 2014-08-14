/*! \file front_panel/ds1307.h
 *  \ingroup front_panel_group 
 *  \brief Realtime clock
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/ds1307.h" \endcode
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

#ifndef _DS1307_H_
#define _DS1307_H_

//! The external I2C address of the DS1307 realtime clock
#define DS1307_ADDR	0x68

void ds1307_init(void);
void ds1307_read(void);

unsigned char ds1307_get_seconds(void);
unsigned char ds1307_get_minutes(void);
unsigned char ds1307_get_hours(void);

void ds1307_set_time(char *data);

#endif