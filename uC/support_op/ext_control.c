#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_control.h"

void ext_control_set_led_yellow(enum led_state state) {
  if (state == LED_STATE_ON)
    PORTA |= (1<<0);
  else if (state == LED_STATE_OFF)
    PORTA &= ~(1<<0);
}

void ext_control_set_led_red(enum led_state state) {
  if (state == LED_STATE_ON)
    PORTA |= (1<<1);
  else if (state == LED_STATE_OFF)
    PORTA &= ~(1<<1);  
}