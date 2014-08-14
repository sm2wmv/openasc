/*! \file delay.h \brief Delay routine
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
 */
//    Copyright (C) 2007  Mikael Larsmark, SM3WMV
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

#ifndef _DELAY_H_
#define _DELAY_H_

#define  F_CPU  14745600   /* The AVR clock frequency in Hertz */

void delay_ms(unsigned char ms);	//MAX 255ms
void delay_s(unsigned char s);

#endif
