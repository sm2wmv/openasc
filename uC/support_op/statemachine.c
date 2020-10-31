#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "statemachine.h"
#include "sequencer.h"
#include "../global.h"
#include "../event_queue.h"
#include "ext_control.h"
#include "main.h" 

#define K3_SET      ext_control_relay_k3_set()
#define K3_CLR      ext_control_relay_k3_clr()
#define K4_SET       ext_control_relay_k4_set()
#define K4_CLR       ext_control_relay_k4_clr()
#define K5K6_SET     ext_control_relay_k5k6_set()
#define K5K6_CLR     ext_control_relay_k5k6_clr()
#define K8_SET       ext_control_relay_k8_set()
#define K8_CLR       ext_control_relay_k8_clr()
#define K9_SET       ext_control_relay_k9_set()
#define K9_CLR       ext_control_relay_k9_clr()
#define K10_SET      ext_control_relay_k10_set()
#define K10_CLR      ext_control_relay_k10_clr()
#define K11_SET      ext_control_relay_k11_set()
#define K11_CLR      ext_control_relay_k11_clr()
#define K12_SET      ext_control_relay_k12_set()
#define K12_CLR      ext_control_relay_k12_clr()
#define K14_SET      ext_control_relay_k14_set()
#define K14_CLR      ext_control_relay_k14_clr()
#define TRANS_SET    ext_control_relay_transfer_set()
#define TRANS_CLR    ext_control_relay_transfer_clr()

uint8_t prev_input_state = 0;
uint16_t prev_output_state = 0;

//Load the current state
/**
 * @brief Initialize the statemachine
 *
 * @param  ...
 * @return void
 **/
void statemachine_init(void) {
 
}


void statemachine_new_event(uint8_t new_state, uint16_t *input_table, uint16_t *output_table, uint8_t table_size) {
  int16_t input_state_id = -1;
  PRINTF("STATEMACHINE NEW EVENT\r\n");

  PRINTF("NEW STATE: %i\r\n", new_state);
  
  //Check if we are PTTing, if so then we ignore the TX ANT selection change until we stop PTT
  if ((ext_control_get_inband_footswitch_input() == 1) || (ext_control_get_inband_footswitch_input() == 1)) {
    if ((new_state & (1<<EVENT_INBAND_TX_SELECT_STATE)) != (prev_input_state & (1<<EVENT_INBAND_TX_SELECT_STATE)))
      new_state ^= (1<<EVENT_INBAND_TX_SELECT_STATE);
  }
  
  for (uint8_t i=0;i<table_size;i++)
    if (new_state == input_table[i]) {
      input_state_id = i;
      break;
    }
    
  PRINTF("INPUT_STATE_ID: %i\r\n",input_state_id);  

  if (input_state_id != -1) {
    statemachine_execute(output_table[input_state_id]);
    
    prev_input_state = new_state;
  }
  else {
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");  
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");  
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");  
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");  
    PRINTF("!!! ERROR: UNKNOWN STATE !!!\r\n");
  }
}

/**
 * @brief Executes the state machine
 *
 * @param  ...
 * @return void
 **/
void statemachine_execute(uint16_t output_state) {
  PRINTF("OUTPUT STATE: %i\r\n",output_state);
  PRINTF("PREV OUTPUT STATE: %i\r\n",prev_output_state);
  
  if (output_state & (1<<OFFSET_PTT_OPENASC_RUN)) {
    PRINTF("PTT ASC RUN: ACTIVE\r\n");
    ext_control_run_ptt_asc_set();
    
    prev_output_state |= (1<<OFFSET_PTT_OPENASC_RUN);
  }
  else {
    PRINTF("PTT ASC RUN: DEACTIVE\r\n");
    ext_control_run_ptt_asc_clr();

    prev_output_state &= ~(1<<OFFSET_PTT_OPENASC_RUN);
  }
  
  if ((output_state & (1<<OFFSET_PTT_RUN_RADIO)) != (prev_output_state & (1<<OFFSET_PTT_RUN_RADIO))) {
    if (output_state & (1<<OFFSET_PTT_RUN_RADIO)) {
      PRINTF("PTT RUN: ACTIVE\r\n");

      sequencer_enter_run_sequence();
      prev_output_state |= (1<<OFFSET_PTT_RUN_RADIO);
    }
    else {
      PRINTF("PTT RUN: DEACTIVE\r\n");
      
      sequencer_exit_run_sequence();
      prev_output_state &= ~(1<<OFFSET_PTT_RUN_RADIO);
    }
  }
  
  if ((output_state & (1<<OFFSET_PTT_RUN_AMP)) != (prev_output_state & (1<<OFFSET_PTT_RUN_AMP))) {
    
    if (output_state & (1<<OFFSET_PTT_RUN_AMP)) {
      PRINTF("PTT RUN AMP: ACTIVE\r\n");

      sequencer_enter_run_amp_sequence();
      prev_output_state |= (1<<OFFSET_PTT_RUN_AMP);
    }
    else {
      PRINTF("PTT RUN AMP: DEACTIVE\r\n");
      
      sequencer_exit_run_amp_sequence();
      prev_output_state &= ~(1<<OFFSET_PTT_RUN_AMP);
    }
  }  
  
  if ((output_state & (1<<OFFSET_PTT_INBAND_RADIO)) != (prev_output_state & (1<<OFFSET_PTT_INBAND_RADIO))) {
    if (output_state & (1<<OFFSET_PTT_INBAND_RADIO)) {
      PRINTF("PTT INBAND: ACTIVE\r\n");
      
      sequencer_enter_inband_sequence();
      prev_output_state |= (1<<OFFSET_PTT_INBAND_RADIO);
    }
    else {
      PRINTF("PTT INBAND: DEACTIVE\r\n");
      
      sequencer_exit_inband_sequence();
      prev_output_state &= ~(1<<OFFSET_PTT_INBAND_RADIO);
    }
  }

  if (output_state & (1<<OFFSET_PTT_INBAND_AMP)) {
    PRINTF("PTT INBAND AMP: ACTIVE\r\n");
    
    sequencer_enter_inband_amp_sequence();
    prev_output_state |= (1<<OFFSET_PTT_INBAND_AMP);
  }
  else {
    PRINTF("PTT INBAND AMP: DEACTIVE\r\n");
    
    sequencer_exit_inband_amp_sequence();
    prev_output_state &= ~(1<<OFFSET_PTT_INBAND_AMP);
  }

  if (output_state & (1<<OFFSET_RELAY_TRANSFER)) {
      TRANS_SET;
      prev_output_state |= output_state & (1<<OFFSET_RELAY_TRANSFER);
  }
  else {
    TRANS_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_TRANSFER));
  }

  if (output_state & (1<<OFFSET_RELAY_K4K12)) {
    K4_SET;
    K12_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K4K12);
  }
  else {
    K4_CLR;
    K12_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K4K12));
  }
  
  if (output_state & (1<<OFFSET_RELAY_K3)) {
    K3_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K3);
  }
  else {
    K3_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K3));
  }

  if (output_state & (1<<OFFSET_RELAY_K5K6)) {
    K5K6_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K5K6);
  }
  else {
    K5K6_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K5K6));
  }

  if (output_state & (1<<OFFSET_RELAY_K8)) {
    K8_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K8);
  }
  else {
    K8_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K8));
  }
  
  if (output_state & (1<<OFFSET_RELAY_K9)) {
    K9_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K9);
  }
  else {
    K9_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K9));
  }
  
  if (output_state & (1<<OFFSET_RELAY_K10)) {
    K10_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K10);
  }
  else {
    K10_CLR;  
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K10));
  }
  
  if (output_state & (1<<OFFSET_RELAY_K11)) {
    K11_SET;
    prev_output_state |= output_state & (1<<OFFSET_RELAY_K11);
  }
  else {
    K11_CLR;
    prev_output_state &= ~(output_state & (1<<OFFSET_RELAY_K11));
  }
}

