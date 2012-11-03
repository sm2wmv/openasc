#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "statematchine.h"
#include "sequencer.h"
#include "../global.h"
#include "../event_queue.h"
#include "ext_control.h"
#include "main.h" 

statemachine_event current_state = 0;

//Load the current state
void statemachine_init(void) {
  if (ext_control_get_r1_footswitch_input() == PTT_ACTIVE) {
    current_state |= (1<<EVENT_FOOTSWICH_R1_ACTIVE);
    current_state &= ~(1<<EVENT_FOOTSWICH_R1_DEACTIVE);
  }
  else {
    current_state |= (1<<EVENT_FOOTSWICH_R1_DEACTIVE);
    current_state &= ~(1<<EVENT_FOOTSWICH_R1_ACTIVE);
  }
  
  if (ext_control_get_r2_footswitch_input() == PTT_ACTIVE) {
    current_state |= (1<<EVENT_FOOTSWICH_R2_ACTIVE);
    current_state &= ~(1<<EVENT_FOOTSWICH_R2_DEACTIVE);
  }
  else {
    current_state |= (1<<EVENT_FOOTSWICH_R2_DEACTIVE);
    current_state &= ~(1<<EVENT_FOOTSWICH_R2_ACTIVE);
  }  
  
  
}

void statemachine_new_state(unsigned char event) {
  
}