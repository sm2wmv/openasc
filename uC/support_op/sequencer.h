#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include "main.h"

#define RUN_AMP_PRE_DELAY    30
#define RUN_RADIO_PRE_DELAY  120

#define INBAND_AMP_PRE_DELAY    30
#define INBAND_RADIO_PRE_DELAY  120

#define RUN_AMP_POST_DELAY    10
#define RUN_RADIO_POST_DELAY  0

#define INBAND_AMP_POST_DELAY    10
#define INBAND_RADIO_POST_DELAY  0


unsigned char sequencer_get_ptt_state_run(void);
unsigned char sequencer_get_ptt_state_inband(void);

void sequencer_enter_run_sequence(void);
void sequencer_exit_run_sequence(void);
void sequencer_enter_run_amp_sequence(void);
void sequencer_exit_run_amp_sequence(void);

void sequencer_enter_inband_sequence(void);
void sequencer_exit_inband_sequence(void);
void sequencer_enter_inband_amp_sequence(void);
void sequencer_exit_inband_amp_sequence(void);

void sequencer_enter_inband_amp1_sequence(void);
void sequencer_exit_inband_amp1_sequence(void);

#endif
