#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include "main.h"

unsigned char state_list[NR_OF_EVENTS];

void statemachine_init(void);
void statemachine_new_event(unsigned char event, unsigned char state);
void statemachine_execute(void);

#endif