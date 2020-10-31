#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "main.h"
#include "board.h"
#include "init.h"
#include "ext_control.h"
#include "usart.h"
#include "../delay.h"
#include "../event_queue.h"
#include "statemachine.h"
#include "sequencer.h"

uint32_t event_counter = 0;

uint16_t run_win_state_table_input[] = {0,1,2,3,4,5,6,7,8,9,10,11,16,17,18,19,20,21,22,23,24,25,26,27,32,33,34,35,36,37,38,39,40,41,42,43,48,49,50,51,52,53,54,55,56,57,58,59};
uint16_t run_win_state_table_output[] = {0,10,21,21,7424,7434,21,21,4224,4234,4245,4245,3584,3594,3605,3605,6912,6922,2325,2325,7808,7818,7829,7829,0,109,21,21,6912,109,2325,2325,4224,4205,6357,6357,2624,109,2133,2133,7488,4461,2325,2325,6848,4205,7829,7829};

uint16_t inband_win_state_table_input[] = {0,1,2,3,4,5,6,7,8,9,10,11,16,17,18,19,20,21,22,23,24,25,26,27,32,33,34,35,36,37,38,39,40,41,42,43,48,49,50,51,52,53,54,55,56,57,58,59};
uint16_t inband_win_state_table_output[] = {0,10,21,10,7424,7434,21,7434,4224,4234,4245,4234,3584,3594,3605,3594,6912,6922,2325,6922,7808,7818,7829,7818,0,109,21,109,6912,109,2325,109,4224,4205,6357,4205,2624,109,2133,109,7488,4461,2325,4461,6848,4205,7829,4205};

uint8_t openasc_ptt_in_state = 0;

#define FLAG_RUN_EVENT_QUEUE 0

uint8_t curr_state = 0;
uint8_t prev_state = 0;

unsigned char flag_poll_inputs = 1;

//! Counter to keep track of the numbers of ticks from timer0
unsigned long counter_compare0 = 0;

unsigned char counter_poll_inputs = 0;

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

//! Here we check that the openASC ptt output matches the PTT input to the box
//! This is so that we are sure that no error has occured in the openASC system.
//! If that happens we are not allowed to transmit
void main_check_openasc_state(void) {
  if (ext_control_get_run_ptt_asc() != 0)
    if ((curr_state & (1<<EVENT_ASC_INPUT_STATE)) != 0) {
      PRINTF("openASC PTT STATE: OK\r\n");
    }
    else {
      sequencer_exit_inband_sequence();
      sequencer_exit_run_amp_sequence();
      sequencer_exit_run_sequence();

      PRINTF("openASC PTT STATE: NOT OK\r\n");
    }
}

#ifdef DEBUG
void print_state_debug_info(void) {
  PRINTF("CURR_STATE != PREV_STATE\r\n");

  PRINTF("EVENT #: %i\r\n",event_counter);
  
  if (curr_state & (1<<EVENT_FOOTSWICH_RUN_STATE))
    PRINTF("STATE >> FSW_R1 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> FSW_R1 -> NOT ACTIVE\r\n");

  if (curr_state & (1<<EVENT_FOOTSWICH_INBAND_STATE))
    PRINTF("STATE >> FSW_R2 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> FSW_R2 -> NOT ACTIVE\r\n");
  
  if (curr_state & (1<<EVENT_INBAND_TX_SELECT_STATE))
    PRINTF("STATE >> INBAND TX SEL -> RUN\r\n");
  else
    PRINTF("STATE >> INBAND TX SEL -> INBAND\r\n");

  if (curr_state & (1<<EVENT_PRIORITY_STATE) == 0)
    PRINTF("STATE >> PRIORITY STATE -> FIRST WINS\r\n");
  else if (curr_state & (1<<EVENT_PRIORITY_STATE) == 1)
    PRINTF("STATE >> PRIORITY STATE -> R1 WINS\r\n");
  else if (curr_state & (1<<EVENT_PRIORITY_STATE) == 2)
    PRINTF("STATE >> PRIORITY STATE -> R2 WINS\r\n");
  
  if (curr_state & (1<<EVENT_RUN_RX_ANT_SELECT))
    PRINTF("STATE >> RX ANT R1 -> ACTIVE\r\n");
  else
    PRINTF("STATE >> RX ANT R1 -> NOT ACTIVE\r\n");
  
  if (curr_state & (1<<EVENT_INBAND_RX_ANT_SELECT))
    PRINTF("STATE >> RX ANT R2 -> RX\r\n");
  else
    PRINTF("STATE >> RX ANT R2 -> TX\r\n");  
  
  if (curr_state & (1<<EVENT_INBAND_RX_ANT_R1_SELET))
    PRINTF("STATE >> RX ANT R2 -> R1\r\n");
  
  if (curr_state & (1<<EVENT_ASC_INPUT_STATE))
    PRINTF("STATE >> ASC INPUT STATE -> ACTIVE\r\n");
  else
    PRINTF("STATE >> ASC INPUT STATE -> DEACTIVE\r\n");
  
  if (ext_control_get_run_ptt_asc())
    PRINTF("STATE >> ASC OUTPUT STATE -> ACTIVE\r\n");
  else
    PRINTF("STATE >> ASC OUTPUT STATE -> DEACTIVE\r\n");

  
  PRINTF("\r\n");
}
#endif


void main_execute_statemachine(void) {
  curr_state = ext_control_read_inputs();
  
  event_counter++;
  
  if (prev_state != curr_state) {
    #ifdef DEBUG
      print_state_debug_info();
    #endif
      
    if (ext_control_get_priority_state() == PRIORITY_R1_WINS) {
      PRINTF("USE THE R1 WIN TABLE\r\n");
      PRINTF("STATE: %i\r\n",curr_state);
      //We remove the priority part and ASC in the state since that has got nothing to do with the truth table sent in
      statemachine_new_event(curr_state & 0x3F, (uint16_t *) run_win_state_table_input,(uint16_t *)run_win_state_table_output,sizeof(run_win_state_table_input));
    }
    else if (ext_control_get_priority_state() == PRIORITY_R2_WINS) {
      //We remove the priority part since that has got nothing to do with the truth table sent in
      statemachine_new_event(curr_state & 0x3F, (uint16_t *) inband_win_state_table_input,(uint16_t *)inband_win_state_table_output,sizeof(inband_win_state_table_input));
    }
    else { //First WINS
      if (((curr_state & (1<<EVENT_FOOTSWICH_RUN_STATE)) != 0) && ((curr_state & (1<<EVENT_FOOTSWICH_INBAND_STATE)) != 0)) {
        if (prev_state & (1<<EVENT_FOOTSWICH_RUN_STATE))
          statemachine_new_event(curr_state & 0x3F, (uint16_t *) run_win_state_table_input,(uint16_t *)run_win_state_table_output,sizeof(run_win_state_table_input));
        else 
          statemachine_new_event(curr_state & 0x3F, (uint16_t *) inband_win_state_table_input,(uint16_t *)inband_win_state_table_output,sizeof(inband_win_state_table_input));
      }
      else {
        if ((prev_state & (1<<EVENT_FOOTSWICH_RUN_STATE)) != (curr_state & (1<<EVENT_FOOTSWICH_RUN_STATE)))
          statemachine_new_event(curr_state & 0x3F, (uint16_t *) run_win_state_table_input,(uint16_t *)run_win_state_table_output,sizeof(run_win_state_table_input));
        else if ((prev_state & (1<<EVENT_FOOTSWICH_INBAND_STATE)) != (curr_state & (1<<EVENT_FOOTSWICH_INBAND_STATE)))
          statemachine_new_event(curr_state & 0x3F, (uint16_t *) inband_win_state_table_input,(uint16_t *)inband_win_state_table_output,sizeof(inband_win_state_table_input));
        else  //If neither of the radios have pushed the PTT and are in RX mode it does not matter which tables we select
          statemachine_new_event(curr_state & 0x3F, (uint16_t *) run_win_state_table_input,(uint16_t *)run_win_state_table_output,sizeof(run_win_state_table_input));
      }
  
    }
  }
  
  event_add_message(main_check_openasc_state,OPENASC_PTT_CHECK_TIME,EVENT_CHECK_OPENASC_PTT);
  
  prev_state = curr_state;
  
  flag_poll_inputs = 1;
  counter_poll_inputs = 0;
}
  
uint8_t main_poll_external_state(void) {
  
  return(0);
}
        

int main(void) {
  cli();
  
  init_ports();
  init_timer_0();
  
  usart1_init(7);
  fdevopen((void*)usart1_transmit, (void*)usart1_receive_loopback);
  
  event_queue_init();
  
  statemachine_init();
  
  sei();
 
  flag_poll_inputs = 1;
  
  PRINTF("\r\n\r\nSupport op board started\r\n");
  PRINTF("------------------------\r\n");
  PRINTF("Version: 0.2b\r\n");
  PRINTF("By: Mikael Larsmark, SM2WMV/SJ2W\r\n");
  PRINTF("Debug mode\r\n\r\n");

  while(1) {
    if (main_flags & (1<<FLAG_RUN_EVENT_QUEUE)) {
      //Run the event in the queue
      event_run();
      main_flags &= ~(1<<FLAG_RUN_EVENT_QUEUE);
      PRINTF("EVENT >> RUNNING (M) - %i\r\n",event_queue_get().id);
    }
               
    if (flag_poll_inputs == 1) {
      if (counter_poll_inputs >= COUNTER_POLL_INPUT_LIMIT) {
        curr_state = ext_control_read_inputs();
        
        if (curr_state != prev_state) {
          event_add_message(main_execute_statemachine,KEYBOUNCE_TIME,EVENT_CHECK_INPUTS);
          
          flag_poll_inputs = 0;
        }

        counter_poll_inputs = 0;    
      }
    }
  }
}

/*! \brief Output compare 0 interrupt - "called" with 1ms intervals*/
ISR(TIMER0_COMP_vect) {
	counter_compare0++;
  
  if ((counter_compare0 % 250) == 0) {
    if (PINA & (1<<0))
      ext_control_led_yellow_clr();
    else
      ext_control_led_yellow_set();
  }
  
  if (event_in_queue()) {
    if (counter_event_timer >= (event_queue_get()).time_target) {
      if (event_queue_get().id >= EVENT_HIGH_PRIORITY_LIMIT) {
        //Run the event in the queue
        event_run();
        PRINTF("EVENT >> RUNNING (I) - %i\r\n",event_queue_get().id);
      }
      else
        main_flags |= (1<<FLAG_RUN_EVENT_QUEUE);
    }
  }
  
  counter_poll_inputs++;
  
  counter_event_timer++;  
}
