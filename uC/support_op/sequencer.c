#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "sequencer.h"
#include "../global.h"
#include "../event_queue.h"
#include "ext_control.h"
#include "main.h"

enum ptt_state ptt_state_r1 = PTT_DEACTIVE;
enum ptt_state ptt_state_r2 = PTT_DEACTIVE;

void r1_ptt_amp_on(void) {
  ext_control_r1_ptt_amp(PTT_ACTIVE);
}

void r1_ptt_amp_off(void) {
  ext_control_r1_ptt_amp(PTT_DEACTIVE);
}

void r2_ptt_amp_on(void) {
  ext_control_r2_ptt_amp(PTT_ACTIVE);
}

void r2_ptt_amp_off(void) {
  ext_control_r2_ptt_amp(PTT_DEACTIVE);
}

void r1_ptt_radio_on(void) {
  ext_control_r1_ptt_radio(PTT_ACTIVE);
}

void r1_ptt_radio_off(void) {
  ext_control_r1_ptt_radio(PTT_DEACTIVE);
}

void r2_ptt_radio_on(void) {
  ext_control_r2_ptt_radio(PTT_ACTIVE);
}

void r2_ptt_radio_off(void) {
  ext_control_r2_ptt_radio(PTT_DEACTIVE);
}

enum ptt_state sequencer_get_ptt_state_r1(void) {
	return(ptt_state_r1);
}

enum ptt_state sequencer_get_ptt_state_r2(void) {
  return(ptt_state_r2);
}

void sequencer_enter_r1_sequence(void) {
  event_add_message((void *)r1_ptt_amp_on, R1_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON);
  event_add_message((void *)r1_ptt_radio_on, R1_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_ON);
  
  ptt_state_r1 = PTT_ACTIVE;
}

void sequencer_exit_r1_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R1_ON) == 0) {
    event_add_message((void *)r1_ptt_radio_off, R1_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_OFF);
  }
        
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON) == 0) {
    event_add_message((void *)r1_ptt_amp_off, R1_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_OFF);
  }
  
  ptt_state_r1 = PTT_DEACTIVE;
}

void sequencer_enter_r2_sequence(void) {
  event_add_message((void *)r2_ptt_amp_on, R2_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON);
  event_add_message((void *)r2_ptt_radio_on, R2_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_ON);
  
  ptt_state_r2 = PTT_ACTIVE;
}

void sequencer_exit_r2_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R2_ON) == 0) {
    event_add_message((void *)r2_ptt_radio_off, R2_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_OFF);
  }
        
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON) == 0) {
    event_add_message((void *)r2_ptt_amp_off, R2_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_AMP_OFF);
  }
  
  ptt_state_r2 = PTT_DEACTIVE;
}