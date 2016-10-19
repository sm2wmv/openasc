/*! \file xbee/init.c
 *  \defgroup xbee_group XBEE interface version 1
 *  \brief Init file of the XBEE 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2016-10-12
 *  \code #include "xbee/init.c" \endcode
 */
//    Copyright (C) 2016  Mikael Larsmark, SM2WMV
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

//! Used for timer compare to match 1 ms
#define OCR0_1MS 14

/*! Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base. */
void init_timer_0(void) {
   TCCR0 = 0;
   TIMSK |= (1<<OCIE0);         /* enable output compare interrupt */
   TCCR0  = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}

/*! Initializes timer 2, used for the communication bus and the interrupt is caught in bus.c */
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
	DDRA = (1<<2);
	DDRB = 0x00;
	DDRC = 0x00;
	DDRD = 0x08;
	DDRE = 0x01;
	DDRF = 0x00;
	DDRG = 0x00;

	PORTD |= (1<<2);
	PORTD |= (1<<3);	
	PORTE |= (1<<0);
	PORTE |= (1<<1);
  
  //Set XBEE reset high
  PORTA |= (1<<2);
}
