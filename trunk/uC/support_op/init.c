#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//! Used for timer compare to match 1 ms
#define OCR0_1MS 7

/*! Initialize timer0 to use the main crystal clock and the output
  * compare interrupt feature to generate an interrupt approximately
  * once per millisecond to use as a general purpose time base. */
void init_timer_0(void) {
   TCCR0 = 0;
   TIMSK |= (1<<OCIE0);                               /* enable output compare interrupt */
   TCCR0  = (1<<WGM01)|(1<<CS02)|(1<<CS01)|(1<<CS00); /* CTC, prescale = 1024 */
   TCNT0  = 0;
   OCR0   = OCR0_1MS;                                 /* match in aprox 1 ms,  */
}

/*!Set the direction of the ports
*/
void init_ports(void) {
	DDRA = 0x03;
	DDRB = 0x81;
	DDRC = 0x00;
	DDRD = 0x08;
	DDRE = 0xFC;
	DDRF = 0x63;
	DDRG = 0x0B;
  
  //Set pull up on MUTE ON TX
  PORTB |= (1<<2);
}
