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

enum ptt_state ext_control_get_r1_footswitch_input(void) {
  if ((PIND & (1<<7)) == 0)
    return(PTT_ACTIVE);
  
  return(PTT_DEACTIVE);
}

enum ptt_state ext_control_get_r2_footswitch_input(void) {
  if ((PINF & (1<<4)) == 0)
    return(PTT_ACTIVE);
  
  return(PTT_DEACTIVE);
}

unsigned char ext_control_get_int_vip_state(void) {
  return((PINB >> 2) & 0x0F);
}

enum rx_tx_ant_state ext_control_get_r1_rx_tx_ant_state(void) {
  if (PIND & (1<<4))
    return(TX_ANT);
  
  return(RX_ANT);
}

enum rx_tx_ant_state ext_control_get_r2_rx_tx_ant_state(void) {
  if (PINF & (1<<2))
    return(TX_ANT);
  
  return(RX_ANT);
}

enum mult_support_state ext_control_get_mult_support_state(void) {
  if (PINF & (1<<3))
    return(SUPPORT_STATE);
  
  return(MULT_STATE);
}

enum priority_state ext_control_get_priority_state(void) {
    unsigned char state = (PIND >> 5) & 0x03;
    
    if (state == 0)
      return(FIRST_WINS);
    else if (state == 1)
      return(RUN_WINS);
    
  return(MULT_WINS);
}