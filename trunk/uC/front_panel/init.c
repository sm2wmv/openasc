/*! \file init.c \brief Initialization routines for the front panel
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
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

#define OCR0_1MS 14

void init_usart_computer(void) {
	usart1_init(47,1);
	fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
	
	printf("openASC started\n");
}

/*!Initializes the USART for the communication bus*/
void init_usart(void) {
	//Init the USART between the uC to run at 115.2 kbaud
	usart0_init(7);
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
  OCR0A = OCR0_1MS;                     /* match in aprox 1 ms,  */
	TIMSK0 |= (1<<OCIE0A);	//enable the interrupt
}

/*!Initializes timer 2
*/
void init_timer_2(void) {
	TCNT2 = 0;
	TCCR2A = (1<<WGM21) | (0<<WGM20); //Normal operation, toggle on compare, prescale clk/64	
	TCCR2B = (0<<WGM22) | (0<<CS22) | (1<<CS21) | (1<<CS20);
	OCR2A = 58;	//Will run the timer at 
	TIMSK2 |= (1<<OCIE2A);
}

/*!Set the direction of the ports
*/
void init_ports(void) {
	/* Check the file board.h for info about ports */
	DDRA = 0xFF;
	DDRB = 0xD6;
	DDRC = 0xD7;
	DDRD = 0x5B;
	DDRE = 0x26;
	//DDRE = 0x24;
	DDRF = 0xFF;
	DDRG = 0x23;
	DDRH = 0xAA;
	DDRJ = 0x8A;
	DDRK = 0xBF;
	DDRL = 0xAA;
	
	PORTH |= (1<<1);	//Set USART port high so we don't send continoulsy out on the bus
	
	//Set pullups enabled for all the button inputs
	PORTC |= (1<<BUTTON_ROTATE_BIT);
	PORTC |= (1<<BUTTON_TXRX_BIT);
	PORTD |= (1<<BUTTON_RXANT_BIT);
	PORTD |= (1<<BUTTON_AUX_BIT);
	PORTK |= (1<<BUTTON_MENU_BIT);
	PORTL |= (1<<BUTTON1_TX_BIT) | (1<<BUTTON2_TX_BIT) | (1<<BUTTON3_TX_BIT) | (1<<BUTTON4_TX_BIT);
	PORTH |= (1<<BUTTON1_RX_BIT) | (1<<BUTTON2_RX_BIT) | (1<<BUTTON3_RX_BIT);
	PORTB |= (1<<BUTTON4_RX_BIT);
	PORTG |= (1<<BUTTON_PULSE_BIT);
	
	PORTE = 0x03;
}

/*! Initialize the backlight (Which is pulse width modulated so we can set the contrast) */
void init_backlight(void) {
	//Start PWM on PB7
	TCCR1A = (1<<COM1C1) | (0<<COM1C0) | (0<<WGM11) | (1<<WGM10);
	TCCR1B = (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
	TCCR1C = 0;
	OCR1CH = 0;
	OCR1CL = 0xC8;
}