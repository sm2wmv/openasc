/*! \file front_panel/eeprom_m24.h
 *  \brief EEPROM hardware functions
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/eeprom_m24.h" \endcode
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

#ifndef _EEPROM_M24_H_
#define _EEPROM_M24_H_

//! The address of the external EEPROM
#define EEPROM_M24_ADDR	0xA0

unsigned char eeprom_m24_write_byte(unsigned int eeprom_address, unsigned char value);
unsigned char eeprom_m24_read_byte(unsigned int eeprom_address);

unsigned char eeprom_m24_write_block(unsigned int start_address, unsigned int length, unsigned char *data);
unsigned char eeprom_m24_read_block(unsigned int start_address, unsigned int length, unsigned char *data);

#endif
