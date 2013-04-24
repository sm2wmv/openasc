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

#define R1_FSW      state_list[EVENT_FOOTSWICH_R1_STATE]
#define R2_FSW      state_list[EVENT_FOOTSWICH_R2_STATE]
#define SOSTATE     state_list[EVENT_R2_SUPPORT_OP_STATE]
#define PRIORITY    state_list[EVENT_PRIORITY_STATE]
#define R1_RXANT    state_list[EVENT_R1_TXRX_ANT_SEL]
#define R2_RXANT    state_list[EVENT_R2_TXRX_ANT_SEL]
#define MUTE_ON_TX  state_list[EVENT_MUTE_ON_TX]

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
#define K13_SET      ext_control_relay_k13_set()
#define K13_CLR      ext_control_relay_k13_clr()
#define K14_SET      ext_control_relay_k14_set()
#define K14_CLR      ext_control_relay_k14_clr()
#define TRANS_SET    ext_control_relay_transfer_set()
#define TRANS_CLR    ext_control_relay_transfer_clr()


//Load the current state
/**
 * @brief Initialize the statemachine
 *
 * @param  ...
 * @return void
 **/
void statemachine_init(void) {
  R1_FSW = ext_control_get_r1_footswitch_input();
  R2_FSW = ext_control_get_r2_footswitch_input();
  SOSTATE = ext_control_get_mult_support_state();
  PRIORITY = ext_control_get_priority_state();
  R1_RXANT = ext_control_get_r1_rx_tx_ant_state();
  R2_RXANT = ext_control_get_r2_rx_tx_ant_state();
  MUTE_ON_TX = ext_control_get_mute_on_tx_state();
}

/**
 * @brief A new event has occured
 *
 * @param event Which type of event has occured
 * @param state The state of the event
 * @return void
 **/
void statemachine_new_event(unsigned char event, unsigned char state) {
  state_list[event] = state;
  if (event == EVENT_FOOTSWICH_R1_STATE) {
    if (ext_control_get_r1_footswitch_input()) {
      PRINTF("EVENT_FOOTSWICH_STATE_R1_STATE = ACTIVE\r\n");
    }
    else {
      PRINTF("EVENT_FOOTSWICH_STATE_R1_STATE = DEACTIVE\r\n");      
    }
  }
  
  if (event == EVENT_FOOTSWICH_R2_STATE) {
    if (ext_control_get_r2_footswitch_input()) {
      PRINTF("EVENT_FOOTSWICH_STATE_R2_STATE = ACTIVE\r\n");
    }
    else {
      PRINTF("EVENT_FOOTSWICH_STATE_R2_STATE = DEACTIVE\r\n");      
    }
  }
  
  PRINTF("STATEMACHINE >> NEW EVENT\r\n");
  
  statemachine_execute();
}

/**
 * @brief Executes the state machine
 *
 * @param  ...
 * @return void
 **/
void statemachine_execute(void) {
  if (state_list[EVENT_R2_SUPPORT_OP_STATE] == 0) {
    K4_CLR;
    K8_CLR;
    K10_CLR;
    TRANS_CLR;
    
    //STATE #5,6,7
    if ((R1_FSW == 1) || (R2_FSW == 1)) {
      PRINTF("STATE #5,6,7\r\n");
      
      if (R1_FSW == 1) {
        if (sequencer_get_ptt_state_r1() != PTT_ACTIVE)
          sequencer_enter_r1_sequence();
      }
      
      if (R2_FSW == 1) {
        if (sequencer_get_ptt_state_r2() != PTT_ACTIVE)
          sequencer_enter_r2_sequence();      
      }
    }
    else if ((R1_RXANT == 0) && (R2_RXANT == 0)) { //STATE #1
      K5K6_CLR;
      K9_CLR;
      K11_CLR;
      K13_CLR;
      
      PRINTF("STATE #1\r\n");
      
      if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
        sequencer_exit_r1_sequence();
      
      if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
        sequencer_exit_r2_sequence();          
    }
    else if ((R1_RXANT == 1) && (R2_RXANT == 0)) { //STATE #2
      K5K6_SET;
      K9_SET;
      K11_CLR;
      K13_CLR;
      K14_CLR;
      
      if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
        sequencer_exit_r1_sequence();

      if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
        sequencer_exit_r2_sequence();          
      
      PRINTF("STATE #2\r\n");
    }
    else if ((R1_RXANT == 0) && (R2_RXANT == 1)) { //STATE #3
      K5K6_CLR;
      K9_CLR;
      K11_SET;
      K13_SET;
      K12_CLR;
      
      PRINTF("STATE #3\r\n");
      
      if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
        sequencer_exit_r1_sequence();
      
      if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
        sequencer_exit_r2_sequence();                
    }
    else if ((R1_RXANT == 1) && (R2_RXANT == 1)) { //STATE #4
      K5K6_SET;
      K9_SET;
      K11_SET;
      K13_SET;
      K14_CLR;
      K12_CLR;
      
      PRINTF("STATE #4\r\n");
      
      if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
        sequencer_exit_r1_sequence();
      
      if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
        sequencer_exit_r2_sequence();          
    }
  }
  else {
    if ((R1_FSW == 0) && (R2_FSW == 0)) { //STATE 8-11
      TRANS_CLR;
      
      if ((R1_RXANT == 0) && (R2_RXANT == 0)) { //STATE #8
        K4_SET;
        K5K6_CLR;
        K8_SET;
        K10_SET;
        K12_CLR;
        K13_SET;
        
        PRINTF("STATE #8\r\n");
        
        if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
          sequencer_exit_r1_sequence();
        
        if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
          sequencer_exit_r2_sequence();        
      }
      else if ((R1_RXANT == 1) && (R2_RXANT == 0)) { //STATE #9
        K4_SET;
        K5K6_SET;
        K8_CLR;
        K9_SET;
        K10_SET;
        K13_SET;
        K14_CLR;
        
        if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
          sequencer_exit_r1_sequence();
        
        if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
          sequencer_exit_r2_sequence();        
        
        PRINTF("STATE #9\r\n");
      }
      else if ((R1_RXANT == 0) && (R2_RXANT == 1)) { //STATE #10
        K4_SET;
        K5K6_CLR;
        K10_CLR;
        K11_SET;
        K12_SET;
        K13_SET;
        K14_CLR;
        
        PRINTF("STATE #10\r\n");
        
        if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
          sequencer_exit_r1_sequence();
        
        if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
          sequencer_exit_r2_sequence();        
      }
      else if ((R1_RXANT == 1) && (R2_RXANT == 1)) { //STATE #11
        K4_SET;
        K5K6_SET;
        K8_CLR;
        K9_SET;
        K10_CLR;
        K11_SET;
        K12_SET;
        K13_SET;
        K14_CLR;        

        PRINTF("STATE #11\r\n");
        
        if (sequencer_get_ptt_state_r1() == PTT_ACTIVE)
          sequencer_exit_r1_sequence();
        
        if (sequencer_get_ptt_state_r2() == PTT_ACTIVE)
          sequencer_exit_r2_sequence();
      }
    }
    else {  //STATE 12-31
      if (PRIORITY == 1) {  //R1 has got priority
        if ((R1_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #12 + #16
          K4_SET;
          K10_CLR;
          K11_SET;
          K12_SET;
          K13_SET;
          K14_CLR;
          TRANS_CLR;
          PRINTF("STATE #12+#16\r\n");
          
          sequencer_enter_r1_sequence();
        }
        else if ((R1_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #13 + #17
          K4_SET;
          K10_CLR;
          K11_CLR;
          K13_SET;
          K14_CLR;
          TRANS_CLR;
          PRINTF("STATE #13+#17\r\n");
          
          sequencer_enter_r1_sequence();
        }
        else if ((R2_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #14
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_SET;
          K12_CLR;
          K13_SET;
          K14_SET;
          TRANS_SET;
          PRINTF("STATE #14\r\n");
          
          sequencer_enter_r2_sequence();
        }
        else if ((R2_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #15
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_CLR;
          K12_CLR;
          K13_SET;
          TRANS_SET;
          
          PRINTF("STATE #15\r\n");
          
          sequencer_enter_r2_sequence();
        }
      }
      else if (PRIORITY == 2) { //R2 has got priority
        if ((R2_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #20 + 22
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_SET;
          K12_CLR;
          K13_SET;
          K14_SET;
          TRANS_SET;
          PRINTF("STATE #20\r\n");

          sequencer_enter_r2_sequence();
        }
        else if ((R2_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #21 + 23
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_CLR;
          K12_CLR;
          K13_SET;
          TRANS_SET;
          PRINTF("STATE #21\r\n");
          
          sequencer_enter_r2_sequence();
        }
        else if ((R1_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #18 
          K4_SET;
          K10_CLR;
          K11_SET;
          K12_SET;
          K13_SET;
          K14_CLR;
          TRANS_CLR;
          PRINTF("STATE #18\r\n");
          
          sequencer_enter_r1_sequence();          
        }
        else if ((R1_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #19
          K4_SET;
          K10_CLR;
          K11_CLR;
          K13_SET;
          K14_CLR;
          TRANS_CLR;
          PRINTF("STATE #19\r\n");
          
          sequencer_enter_r1_sequence();          
        }        
      }
      else if (PRIORITY == 0) { //First wins
        if ((R1_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #24
          K4_SET;
          K10_CLR;
          K11_SET;
          K13_SET;
          TRANS_CLR;
          PRINTF("STATE #24\r\n");
        }
        else if ((R1_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #25
          K4_SET;
          K10_CLR;
          K11_CLR;
          K13_SET;
          TRANS_CLR;
          PRINTF("STATE #25\r\n");
        }
        else if ((R2_FSW == 1) && (MUTE_ON_TX == 0)) { //STATE #26
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_SET;
          K13_SET;
          TRANS_SET;
          PRINTF("STATE #26\r\n");
        }
        else if ((R2_FSW == 1) && (MUTE_ON_TX == 1)) { //STATE #27
          K4_SET;
          K5K6_SET;
          K8_CLR;
          K9_CLR;
          K13_SET;
          TRANS_SET;
          PRINTF("STATE #27\r\n");
        }        
      }
    }
  }
}