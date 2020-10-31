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

unsigned char ptt_state_run = PTT_DEACTIVE;
unsigned char ptt_state_inband = PTT_DEACTIVE;

unsigned char sequencer_get_ptt_state_run(void) {
	return(ptt_state_run);
}

unsigned char sequencer_get_ptt_state_inband(void) {
  return(ptt_state_inband);
}

void sequencer_enter_run_sequence(void) {
  event_add_message((void *)ext_control_run_ptt_radio_set, RUN_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_RUN_ON);
  
  ptt_state_run = PTT_ACTIVE;
  
  PRINTF("SEQUENCER >> RUN ENTERED\r\n");
}

void sequencer_exit_run_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_RUN_ON) == 0) {
    event_add_message((void *)ext_control_run_ptt_radio_clr, RUN_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_RUN_OFF);
  }
        
  
  ptt_state_run = PTT_DEACTIVE;
  
  PRINTF("SEQUENCER >> RUN EXITED\r\n");  
}

void sequencer_enter_run_amp_sequence(void) {
  event_add_message((void *)ext_control_run_ptt_amp_set, RUN_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_RUN_AMP_ON);
  
  PRINTF("SEQUENCER >> RUN AMP ENTERED\r\n");
}

void sequencer_exit_run_amp_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_RUN_AMP_ON) == 0) {
    event_add_message((void *)ext_control_run_ptt_amp_clr, RUN_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_RUN_AMP_OFF);
  }
  
  PRINTF("SEQUENCER >> RUN AMP EXITED\r\n");  
}

void sequencer_enter_inband_sequence(void) {
  event_add_message((void *)ext_control_inband_ptt_radio_set, INBAND_RADIO_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_INBAND_ON);
  
  ptt_state_inband = PTT_ACTIVE;
  
  PRINTF("SEQUENCER >> INBAND ENTERED\r\n");
}

void sequencer_exit_inband_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_INBAND_ON) == 0) {
    event_add_message((void *)ext_control_inband_ptt_radio_clr, INBAND_RADIO_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_INBAND_OFF);
  }
  
  ptt_state_inband = PTT_DEACTIVE;
  
  PRINTF("SEQUENCER >> INBAND EXITED\r\n");
}


void sequencer_enter_inband_amp_sequence(void) {
  event_add_message((void *)ext_control_inband_ptt_amp_set, INBAND_AMP_PRE_DELAY, SEQUENCER_EVENT_TYPE_PTT_INBAND_AMP_ON);
  
  PRINTF("SEQUENCER >> INBAND AMP ENTERED\r\n");
}

void sequencer_exit_inband_amp_sequence(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_INBAND_AMP_ON) == 0) {
    event_add_message((void *)ext_control_inband_ptt_amp_clr, INBAND_AMP_POST_DELAY, SEQUENCER_EVENT_TYPE_PTT_INBAND_AMP_OFF);
  }
  
  PRINTF("SEQUENCER >> INBAND AMP EXITED\r\n");
}
