/*! \file driver_unit_v2/init.c 
 *  \ingroup driver_unit_v2_group
 *  \brief Initialization routines for the driver unit 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2009-03-16
 *  \code #include "driver_unit_v2/init.c" \endcode
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

#define OCR0_1MS 14

/*
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
void init_timer_0(void) {
   TCCR0 = 0;
   TIMSK |= (1<<OCIE0);         /* enable output compare interrupt */
   TCCR0  = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}

/*!Initializes timer 2, used for the communication bus and the interrupt is caught in bus.c
*/
void init_timer_2(void) {
	TCCR2 = 0;
	TCNT2 = 0;
	TCCR2 = (1<<WGM21) | (0<<WGM20) | (0<<CS22) | (1<<CS21) | (1<<CS20); //Normal operation, toggle on compare, prescale clk/64	
	TIFR |= (1<<OCF2);
	OCR2 = 30;	//Will trigger an interrupt each with an interval of 130us
	TIMSK |= (1<<OCIE2);
}

/*!Set the direction of the ports
*/
void init_ports(void) {
	DDRA = 0x80;
	DDRB = 0xF0;
	DDRC = 0xFF;
	DDRD = 0xF8;
	DDRE = 0x00;
	DDRF = 0x7F;
	DDRG = 0x1C;
	
	//Activate pullup, for POS/NEG driver module sense input
	PORTF = (1<<7);
}
