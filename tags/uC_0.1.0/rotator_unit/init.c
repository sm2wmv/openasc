/*! \file init.c \brief Initialization routines for the driver unit 
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-03-16
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
#include <avr/io.h>
#include <avr/interrupt.h>

#define OCR0_1MS	14

/*
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
void init_timer_0(void) {
   TCCR0A = 0;
   TIMSK0 |= (1<<OCIE0A);         /* enable output compare interrupt */
   TCCR0A  = (1<<WGM01)|(1<<CS02)|(0<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0A   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}

/*!Initializes timer 2, used for the communication bus and the interrupt is caught in bus.c
*/
void init_timer_2(void) {
	TCCR2A = 0;
	TCNT2 = 0;
	TCCR2A = (1<<WGM21) | (0<<WGM20) | (0<<CS22) | (1<<CS21) | (1<<CS20); //Normal operation, toggle on compare, prescale clk/64	
	TIFR2 |= (1<<OCF2A);
	OCR2A = 58;	//Will trigger an interrupt each with an interval of 130us
	TIMSK2 |= (1<<OCIE2A);
}

/*!Set the direction of the ports
*/
void init_ports(void) {
	DDRA = 0x00;
	DDRB = 0x00;
	DDRC = 0x03;
	DDRD = 0xF8;
	DDRE = 0x00;
	DDRF = 0x00;
	DDRG = 0x00;
}
