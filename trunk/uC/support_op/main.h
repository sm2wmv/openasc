#ifndef _MAIN_H_
#define _MAIN_H_

#define PTT_DEACTIVE  0
#define PTT_ACTIVE    1

#define NR_OF_EVENTS  7

#define EVENT_FOOTSWICH_R1_STATE          0
#define EVENT_FOOTSWICH_R2_STATE          1
#define EVENT_R2_SUPPORT_OP_STATE         2
#define EVENT_PRIORITY_STATE              3
#define EVENT_R1_TXRX_ANT_SEL             4
#define EVENT_R2_TXRX_ANT_SEL             5
#define EVENT_MUTE_ON_TX                  6

#define KEYBOUNCE_TIME 5

/*! START EVENT QUEUE IDs */
#define EVENT_QUEUE_ID_TRANSFER_RELAY     125

//! Limit of an high priority event, if the value is above this it is counted as high priority
#define EVENT_HIGH_PRIORITY_LIMIT 100

#define EVENT_CHECK_INPUTS  120
#define SEQUENCER_EVENT_TYPE_PTT_R1_AMP_ON    101
#define SEQUENCER_EVENT_TYPE_PTT_R1_ON        102
#define SEQUENCER_EVENT_TYPE_PTT_R2_AMP_ON    103
#define SEQUENCER_EVENT_TYPE_PTT_R2_ON        104

#define SEQUENCER_EVENT_TYPE_PTT_R1_AMP_OFF   105
#define SEQUENCER_EVENT_TYPE_PTT_R1_OFF       106
#define SEQUENCER_EVENT_TYPE_PTT_R2_AMP_OFF   107
#define SEQUENCER_EVENT_TYPE_PTT_R2_OFF       108

/*! END EVENT QUEUE IDs */

//! Time between each poll of the inputs (in ms)
#define COUNTER_POLL_INPUT_LIMIT 2

//! The pulse time of the transfer relay in ms
#define TRANSFER_RELAY_PULSE_TIME 25

#ifdef DEBUG
#define PRINTF printf
#else 
#define PRINTF(...)
#endif

void main_execute_statemachine(void);
void event_add_message(void (*func), unsigned int offset, unsigned char id);

void __inline__ r1_ptt_amp_on(void);

#endif
