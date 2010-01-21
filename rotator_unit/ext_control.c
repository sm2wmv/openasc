/*! \file ext_control.c \brief External control file
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-04-22
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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
#include <avr/io.h>
#include <avr/interrupt.h>

#include "board.h"
#include "../delay.h"

void ext_ctrl_relay1_activate(void) {
	PORTD |= (1<<RELAY1_BIT);
}

void ext_ctrl_relay1_deactivate(void) {
	PORTD &= ~(1<<RELAY1_BIT);
}

void ext_ctrl_relay2_activate(void) {
	PORTD |= (1<<RELAY2_BIT);
}

void ext_ctrl_relay2_deactivate(void) {
	PORTD &= ~(1<<RELAY2_BIT);
}

void ext_ctrl_relay3_activate(void) {
	PORTD |= (1<<RELAY3_BIT);
}

void ext_ctrl_relay3_deactivate(void) {
	PORTD &= ~(1<<RELAY3_BIT);
}

void ext_ctrl_relay4_activate(void) {
	PORTD |= (1<<RELAY4_BIT);
}

void ext_ctrl_relay4_deactivate(void) {
	PORTD &= ~(1<<RELAY4_BIT);
}

void ext_ctrl_fet1_activate(void) {
	PORTC |= (1<<FET1_BIT);
}

void ext_ctrl_fet1_deactivate(void) {
	PORTC &= ~(1<<FET1_BIT);
}

void ext_ctrl_fet2_activate(void) {
	PORTC |= (1<<FET2_BIT);
}

void ext_ctrl_fet2_deactivate(void) {
	PORTC &= ~(1<<FET2_BIT);
}

void ext_ctrl_set_io_dir(unsigned char bit, unsigned char dir) {
	if (dir == 1)
		DDRA |= (1<<bit);
	else
		DDRA &= ~(1<<bit);
}

void ext_ctrl_set_io(unsigned char bit, unsigned char state) {
	if (state == 1)
		PORTA |= (1<<bit);
	else
		PORTA &= ~(1<<bit);
}

unsigned char ext_ctrl_read_io(unsigned char bit) {
	if (PINA & (1<<bit))
		return(1);
	
	return(0);
}