#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "ext_control.h"
#include "../delay.h"
#include "../event_queue.h"

#define FLAG_RUN_EVENT_QUEUE 0

//! Counter to keep track of the numbers of ticks from timer0
unsigned long counter_compare0 = 0;
unsigned char st = 0;

unsigned char main_flags = 0;

//!After the counter reaches half of it's limit we remove that number from it by calling the function event_queue_wrap()
static unsigned int counter_event_timer = 0;

/*! Add a message to the event queue which will be run at the correct time
 *  \param func A function pointer to the function we want to run
 *  \param offset the time in ms when we want our function to be run
 *  \param id Which type of event this is */
void event_add_message(void (*func), unsigned int offset, unsigned char id) {
  EVENT_MESSAGE event;
  
  if (event_in_queue() == 0)
    counter_event_timer = 0;

  event.func = func;
  event.time_target = counter_event_timer + offset;
  event.id = id;
  
  event_queue_add(event);
}

/*! \brief Run the first function in the event queue */
void event_run(void) {
  if (event_in_queue()) {
    EVENT_MESSAGE event = event_queue_get();
    
    event_queue_drop();
    
    //Run the function in the event queue
    event.func();
  }
}

int main(void) {
  cli();
  
  init_ports();
  init_timer_0();
  
  event_queue_init();
  
  sei();
 
  while(1) {
    if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
      //Run the event in the queue
      event_run();
      main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
    }
  }
}
/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_compare0++;
  
  if ((counter_compare0 % 250) == 0) {
    if (PINA & (1<<0))
      ext_control_set_led_yellow(LED_OFF);
    else
      ext_control_set_led_yellow(LED_ON);
  }
  
  if (event_in_queue()) {
    if (counter_event_timer >= (event_queue_get()).time_target)
      main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
  }
  
  counter_event_timer++;  
}