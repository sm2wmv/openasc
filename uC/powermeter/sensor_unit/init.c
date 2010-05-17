#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#define OCR0_1MS	14

/*
  * Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base.
  */
void init_timer_0(void) {
	TCCR0 = 0;
	TIMSK |= (1<<OCIE0);         /* enable output compare interrupt */
	TCCR0  = (1<<WGM01)|(1<<CS02)|(0<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
	TCNT0  = 0;
	OCR0   = OCR0_1MS;                     /* match in aprox 1 ms,  */
}

/*!Initializes timer 2, used for the communication bus and the interrupt is caught in bus.c
 */
void init_timer_2(void) {
	TCCR2 = 0;
	TCNT2 = 0;
	TCCR2 = (1<<WGM21) | (0<<WGM20) | (1<<CS22) | (0<<CS21) | (0<<CS20); //Normal operation, toggle on compare, prescale clk/64	
	TIFR |= (1<<OCF2);
	OCR2 = 30;	//Will trigger an interrupt each with an interval of 130us
	TIMSK |= (1<<OCIE2);
}

void init_ports(void) {
	DDRA = 0xFC;
	PORTA = 0x00;
	DDRB = 0x1F;
	PORTB = 0x00;
	DDRC = 0x03;
	PORTC = 0x00;
	DDRD = 0x86;
	PORTD = (1<<0) | (1<<1) | (1<<3) | (1<<4) | (1<<5);
	
	//PORTD = (1<<3) | (1<<4) | (1<<5);
}

