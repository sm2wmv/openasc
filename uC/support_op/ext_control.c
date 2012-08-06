#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_control.h"

void ext_control_set_led_yellow(enum led_state state) {
  if (state == LED_ON)
    PORTA |= (1<<0);
  else if (state == LED_OFF)
    PORTA &= ~(1<<0);
}

void ext_control_set_led_red(enum led_state state) {
  if (state == LED_ON)
    PORTA |= (1<<1);
  else if (state == LED_OFF)
    PORTA &= ~(1<<1);  
}

void ext_control_set_relay_k4(enum relay_state state) {
  if (state == RELAY_ON)
    PORTB |= (1<<0);
  else if (state == RELAY_OFF)
    PORTB &= ~(1<<0);
}

void ext_control_set_relay_k5k6(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<7);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<7);
}

void ext_control_set_relay_k8(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<6);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<6);
}

void ext_control_set_relay_k9(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<5);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<5);
}

void ext_control_set_relay_k11(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<4);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<4);
}

void ext_control_set_relay_k10(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<3);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<3);
}

void ext_control_set_relay_k13(enum relay_state state) {
  if (state == RELAY_ON)
    PORTE |= (1<<2);
  else if (state == RELAY_OFF)
    PORTE &= ~(1<<2);
}

void ext_control_r1_ptt_radio(enum ptt_state state) {
  if (state == PTT_ACTIVE) {
    PORTG |= (1<<0);
    PORTG |= (1<<3);
    PORTF |= (1<<6);
  }
  else if (state == PTT_DEACTIVE) {
    PORTG &= ~(1<<0);
    PORTG &= ~(1<<3);
    PORTF &= ~(1<<6);
  }
}

void ext_control_r2_ptt_radio(enum ptt_state state) {
  if (state == PTT_ACTIVE) {
    PORTG |= (1<<1);
    PORTF |= (1<<1);
    PORTF |= (1<<5);
  }
  else if (state == PTT_DEACTIVE) {
    PORTG &= ~(1<<1);
    PORTF &= ~(1<<1);
    PORTF &= ~(1<<5);
  }
}

void ext_control_r1_ptt_amp(enum ptt_state state) {
  if (state == PTT_ACTIVE) {
    PORTB |= (1<<7);
  }
  else if (state == PTT_DEACTIVE) {
    PORTB &= ~(1<<7);
  }
}

void ext_control_r2_ptt_amp(enum ptt_state state) {
  if (state == PTT_ACTIVE) {
    PORTF |= (1<<0);
  }
  else if (state == PTT_DEACTIVE) {
    PORTF &= ~(1<<0);
  }
}