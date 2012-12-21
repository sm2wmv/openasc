#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "ext_control.h"
#include "usart.h"
#include "../delay.h"
#include "../event_queue.h"
#include "statemachine.h"

#define FLAG_RUN_EVENT_QUEUE 0

static unsigned char curr_state[NR_OF_EVENTS] = {0};
static unsigned char prev_state[NR_OF_EVENTS] = {0};

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

#ifdef DEBUG
void print_state_debug_info(void) {
  PRINTF("CURR_STATE != PREV_STATE\r\n");

  if (curr_state[EVENT_FOOTSWICH_R1_STATE])
    PRINTF("STATE >> FSW_R1 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> FSW_R1 -> NOT ACTIVE\r\n");

  if (curr_state[EVENT_FOOTSWICH_R2_STATE])
    PRINTF("STATE >> FSW_R2 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> FSW_R2 -> NOT ACTIVE\r\n");
  
  if (curr_state[EVENT_R2_SUPPORT_OP_STATE])
    PRINTF("STATE >> SUPPORT MODE -> ACTIVE\r\n");
  else
    PRINTF("STATE >> SUPPORT MODE -> NOT ACTIVE\r\n");

  if (curr_state[EVENT_PRIORITY_STATE] == 0)
    PRINTF("STATE >> PRIORITY STATE -> FIRST WINS\r\n");
  else if (curr_state[EVENT_PRIORITY_STATE] == 1)
    PRINTF("STATE >> PRIORITY STATE -> R1 WINS\r\n");
  else if (curr_state[EVENT_PRIORITY_STATE] == 2)
    PRINTF("STATE >> PRIORITY STATE -> R2 WINS\r\n");
  
  if (curr_state[EVENT_R1_TXRX_ANT_SEL])
    PRINTF("STATE >> RX ANT R1 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> RX ANT R1 -> NOT ACTIVE\r\n");
  
  if (curr_state[EVENT_R2_TXRX_ANT_SEL])
    PRINTF("STATE >> RX ANT R2 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> RX ANT R2 -> NOT ACTIVE\r\n");  
  
  if (curr_state[EVENT_MUTE_ON_TX])
    PRINTF("STATE >> MUTE ON TX -> ACTIVE\r\n");
  else
    PRINTF("STATE >> MUTE ON TX -> NOT ACTIVE\r\n");
  
  PRINTF("\r\n");
}
#endif

int main(void) {
  cli();
  
  init_ports();
  init_timer_0();
  
  usart1_init(7);
  fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
  
  event_queue_init();
  
  statemachine_init();
  
  sei();
 
  PRINTF("\r\n\r\nSupport op board started\r\n");
  PRINTF("------------------------\r\n");
  PRINTF("Version: 0.1b\r\n");
  PRINTF("By: Mikael Larsmark, SM2WMV/SJ2W\r\n");
  PRINTF("Debug mode\r\n\r\n");
  
  while(1) {
    if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
      //Run the event in the queue
      event_run();
      main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
    }
    
    ext_control_read_inputs(curr_state);
    
    if (memcmp(curr_state,prev_state,sizeof(curr_state)) != 0) {
      #ifdef DEBUG
        print_state_debug_info();
      #endif
      
      //Check which event has changed and update the statemachine
      for (unsigned char i=0;i<NR_OF_EVENTS;i++)
        if (prev_state[i] != curr_state[i])
          statemachine_new_event(i,curr_state[i]);
      
      memcpy(prev_state,curr_state,sizeof(curr_state));
    }
  }
}

/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(SIG_OUTPUT_COMPARE0) {
	counter_compare0++;
  
  if ((counter_compare0 % 250) == 0) {
    if (PINA & (1<<0))
      ext_control_led_yellow_clr();
    else
      ext_control_led_yellow_set();
  }
  
  if (event_in_queue()) {
    if (counter_event_timer >= (event_queue_get()).time_target)
      main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
  }
  
  counter_event_timer++;  
}