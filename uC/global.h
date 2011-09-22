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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "avrlibdefs.h"
#include "avrlibtypes.h"

//These two macros make sure that you don't enable the global interrupt
//if it was disabled already
#define CLI unsigned char status_sreg = (SREG & 0x80); cli();
#define SEI if (status_sreg) sei();	

// CPU clock speed
#define F_CPU        14745000               		// 14.745MHz processor
#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#define asm(x) __asm__ __volatile__ (x)

#define BAND_UNDEFINED	0
#define BAND_160M				1
#define BAND_80M				2
#define BAND_40M				3
#define BAND_30M				4
#define BAND_20M				5
#define BAND_17M				6
#define BAND_15M				7
#define BAND_12M				8
#define BAND_10M				9

//! If it's neither the low or high portion of a band
#define BAND_UNDEFINED	0
//! The low portion of the band
#define BAND_LOW				1
//! The high portion of the band
#define BAND_HIGH				2

#define ANTENNA_1		0
#define ANTENNA_2		1
#define ANTENNA_3		2
#define ANTENNA_4		3

//! RX antenna name length
#define RX_ANTENNA_NAME_LENGTH        13
//! RX antenna output str length
#define RX_ANTENNA_OUTPUT_STR_LENGTH  10
//! RX antenna band output str length
#define RX_ANTENNA_BAND_OUTPUT_STR_LENGTH 10

//! The max size of the antenna output str length
#define ANTENNA_TEXT_SIZE         10
//! The max size of the output combination length
#define ANTENNA_OUTPUT_COMB_SIZE  10
//! The max size of the band output str
#define BAND_OUTPUT_STR_SIZE      15
//! The max size of the sub menu array output str size
#define SUB_MENU_ARRAY_STR_SIZE   10
//! The size of the name of a 4-SQ
#define SUB_MENU_ARRAY_NAME_SIZE  4
//! The max size of the sub menu array output str size
#define SUB_MENU_STACK_STR_SIZE 10
//! The size of the name of a stack
#define SUB_MENU_STACK_NAME_SIZE 10

#endif
