#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include "main.h"

#define R1_AMP_PRE_DELAY    20
#define R1_RADIO_PRE_DELAY  70

#define R2_AMP_PRE_DELAY    20
#define R2_RADIO_PRE_DELAY  70

#define R1_AMP_POST_DELAY    20
#define R1_RADIO_POST_DELAY  0

#define R2_AMP_POST_DELAY    20
#define R2_RADIO_POST_DELAY  0


unsigned char sequencer_get_ptt_state_r1(void);
unsigned char sequencer_get_ptt_state_r2(void);

void sequencer_enter_r1_sequence(void);
void sequencer_exit_r1_sequence(void);

void sequencer_enter_r2_sequence(void);
void sequencer_exit_r2_sequence(void);

#endif