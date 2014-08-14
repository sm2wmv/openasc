/*! \file ads1115h.h \brief ADS1115 chip interface
 * \author Mikael Larsmark, SM2WMV
 * \date 2012-02-25
 */
//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
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

#ifndef _ADS1115_H_
#define _ADS1115_H_

#define ADS1115_ADDR  0x90
void ads1115_init(void);

unsigned int ads1115_read_ch0(void);

unsigned int ads1115_read_ch1(void);

#endif