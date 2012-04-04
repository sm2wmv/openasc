/*! \file amplifiers/finnpa/ctrl_board/ctrl_board/init.c
 *  \brief Initialization routines for the control board
 *  \ingroup ctrl_board_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/init.c" \endcode
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

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "board.h"

//! Used for timer compare to match 1 ms
#define OCR0_1MS 14

/*! Init the UART for the computer communication */
void init_usart_computer(void) {
	usart0_init(47); //115.2kbit
	fdevopen((void*)usart0_transmit, (void*)usart0_receive_loopback);
}

/*!
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
void init_timer_0(void) {
	TCCR0A = 0;
	TCCR0A = (1<<WGM01) | (0<<WGM00);
	TCCR0B = (0<<WGM02) | (1<<CS02) | (0<<CS01) | (1<<CS00);	//Prescaler 1024
	TCNT0 = 0;
	OCR0A = OCR0_1MS;       /* match in aprox 1 ms,  */
	TIMSK0 |= (1<<OCIE0A);	//enable the interrupt
}

/*!Initializes timer 2
*/
void init_timer_2(void) {
	TCNT2 = 0;
	TCCR2A = (1<<WGM21) | (0<<WGM20); //Normal operation, toggle on compare, prescale clk/64	
	TCCR2B = (0<<WGM22) | (1<<CS22) | (0<<CS21) | (0<<CS20);
	OCR2A = 30;	//Will run the timer at 130us intervals
	TIMSK2 |= (1<<OCIE2A);
}

/*!Set the direction of the ports
*/
void init_ports(void) {
	/* Check the file board.h for info about ports */
	DDRA = 0x00;
	DDRB = 0xF7;
	DDRC = 0xFF;
	DDRD = 0xFB;
	DDRE = 0x81;
  DDRF = 0x00;
	DDRG = 0x1C;
	DDRH = 0xFE;
	DDRJ = 0x7F;
	DDRK = 0x00;
	DDRL = 0xFF;
	
	PORTH |= (1<<0) | (1<<1);
}
