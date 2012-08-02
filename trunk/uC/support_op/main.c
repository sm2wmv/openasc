#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "ext_control.h"
#include "../delay.h"

//! Counter to keep track of the numbers of ticks from timer0
unsigned int counter_compare0 = 0;
unsigned char st = 0;


void main(void) {
  cli();
  
  init_ports();
  init_timer_0();
  
  sei();  
  
  while(1) {
  }
}
/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_compare0++;
  
  if ((counter_compare0 % 250) == 0) {
    if (PINA & (1<<0))
      ext_control_set_led_yellow(LED_STATE_OFF);
    else
      ext_control_set_led_yellow(LED_STATE_ON);
  }
}