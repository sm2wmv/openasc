#ifndef _SEQUENCER_H_
#define _SEQUENCER_H_

#include "main.h"

#define R1_AMP_PRE_DELAY    0
#define R1_RADIO_PRE_DELAY  50

#define R2_AMP_PRE_DELAY    0
#define R2_RADIO_PRE_DELAY  50

#define R1_AMP_POST_DELAY    20
#define R1_RADIO_POST_DELAY  0

#define R2_AMP_POST_DELAY    20
#define R2_RADIO_POST_DELAY  0

#define SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON    1
#define SEQUENCER_EVENT_TYPE_PTT_R1_ON        2
#define SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON    3
#define SEQUENCER_EVENT_TYPE_PTT_R2_ON        4

#define SEQUENCER_EVENT_TYPE_PTT_R1_AMP_OFF   5
#define SEQUENCER_EVENT_TYPE_PTT_R1_OFF       6
#define SEQUENCER_EVENT_TYPE_PTT_R2_AMP_OFF   7
#define SEQUENCER_EVENT_TYPE_PTT_R2_OFF       8

unsigned char sequencer_get_ptt_state_r1(void);
unsigned char sequencer_get_ptt_state_r2(void);

void sequencer_enter_r1_sequence(void);
void sequencer_exit_r1_sequence(void);

void sequencer_enter_r2_sequence(void);
void sequencer_exit_r2_sequence(void);

#endif