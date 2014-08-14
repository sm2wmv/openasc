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

unsigned char ptt_state_r1 = PTT_DEACTIVE;
unsigned char ptt_state_r2 = PTT_DEACTIVE;

unsigned char sequencer_get_ptt_state_r1(void) {
	return(ptt_state_r1);
}

unsigned char sequencer_get_ptt_state_r2(void) {
  return(ptt_state_r2);
}

void sequencer_enter_r1_sequence(void) {
  event_add_message((void *)ext_control_r1_ptt_amp_set, R1_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON);
  event_add_message((void *)ext_control_r1_ptt_radio_set, R1_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_ON);
  
  ptt_state_r1 = PTT_ACTIVE;
  
  PRINTF("SEQUENCER >> R1 ENTERED\r\n");
}

void sequencer_exit_r1_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R1_ON) == 0) {
    event_add_message((void *)ext_control_r1_ptt_radio_clr, R1_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_OFF);
  }
        
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON) == 0) {
    event_add_message((void *)ext_control_r1_ptt_amp_clr, R1_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_OFF);
  }
  
  ptt_state_r1 = PTT_DEACTIVE;
  
  PRINTF("SEQUENCER >> R1 EXITED\r\n");  
}

void sequencer_enter_r2_sequence(void) {
  event_add_message((void *)ext_control_r2_ptt_amp_set, R2_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON);
  event_add_message((void *)ext_control_r2_ptt_radio_set, R2_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_ON);
  
  ptt_state_r2 = PTT_ACTIVE;
  
  PRINTF("SEQUENCER >> R2 ENTERED\r\n");
}

void sequencer_exit_r2_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R2_ON) == 0) {
    event_add_message((void *)ext_control_r2_ptt_radio_clr, R2_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_OFF);
  }
        
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON) == 0) {
    event_add_message((void *)ext_control_r2_ptt_amp_clr, R2_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_AMP_OFF);
  }
  
  ptt_state_r2 = PTT_DEACTIVE;
  
  PRINTF("SEQUENCER >> R2 EXITED\r\n");
}

void sequencer_enter_r2_amp1_sequence(void) {
  event_add_message((void *)ext_control_r1_ptt_amp_set, R1_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON);
  event_add_message((void *)ext_control_r2_ptt_radio_set, R2_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_ON);
  
  ptt_state_r2 = PTT_ACTIVE;
  
  PRINTF("SEQUENCER >> R2 AMP1 ENTERED\r\n");
}

void sequencer_exit_r2_amp1_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R2_ON) == 0) {
    event_add_message((void *)ext_control_r2_ptt_radio_clr, R2_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R2_OFF);
  }
        
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON) == 0) {
    event_add_message((void *)ext_control_r1_ptt_amp_clr, R1_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_R1_AMP_OFF);
  }
  
  ptt_state_r2 = PTT_DEACTIVE;
  
  PRINTF("SEQUENCER >> R2 AMP1 EXITED\r\n");
}
