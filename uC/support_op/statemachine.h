#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

#include "main.h"

unsigned char state_list[NR_OF_EVENTS];

void statemachine_init(void);
void statemachine_new_event(uint8_t new_state, uint16_t *input_table, uint16_t *output_table, uint8_t table_size);
void statemachine_execute(uint16_t output_state);

#endif
