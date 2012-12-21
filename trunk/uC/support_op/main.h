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

#ifdef DEBUG
#define PRINTF printf
#else 
#define PRINTF(...)
#endif

void event_add_message(void (*func), unsigned int offset, unsigned char id);

void __inline__ r1_ptt_amp_on(void);

#endif
