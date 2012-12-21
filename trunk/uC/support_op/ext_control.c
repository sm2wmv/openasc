#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_control.h"

void ext_control_led_yellow_set(void) {
  PORTA |= (1<<0);
}

void ext_control_led_yellow_clr(void) {
  PORTA &= ~(1<<0);
}

void ext_control_led_red_set(void) {
  PORTA |= (1<<1);
}

void ext_control_led_red_clr(void) {
  PORTA &= ~(1<<1);
}

void ext_control_relay_k4_set(void) {
  PORTB |= (1<<0);
}

void ext_control_relay_k4_clr(void) {
  PORTB &= ~(1<<0);
}

void ext_control_relay_k5k6_set(void) {
  PORTE |= (1<<7);
}

void ext_control_relay_k5k6_clr(void) {
  PORTE &= ~(1<<7);
}

void ext_control_relay_k8_set(void) {
  PORTE |= (1<<6);
}

void ext_control_relay_k8_clr(void) {
  PORTE &= ~(1<<6);
}

void ext_control_relay_k9_set(void) {
  PORTE |= (1<<5);
}

void ext_control_relay_k9_clr(void) {
  PORTE &= ~(1<<5);
}

void ext_control_relay_k10_set(void) {
  PORTE |= (1<<3);
}

void ext_control_relay_k10_clr(void) {
  PORTE &= ~(1<<3);
}

void ext_control_relay_k11_set(void) {
  PORTE |= (1<<4);
}

void ext_control_relay_k11_clr(void) {
  PORTE &= ~(1<<4);
}

void ext_control_relay_k13_set(void) {
  PORTE |= (1<<2);
}

void ext_control_relay_k13_clr(void) {
  PORTE &= ~(1<<2);
}

void ext_control_relay_transfer_set(void) {
  
}

void ext_control_relay_transfer_clr(void) {
  
}

void ext_control_r1_ptt_radio_set(void) {
  PORTG |= (1<<0);
  PORTG |= (1<<3);
  PORTF |= (1<<6);
}

void ext_control_r1_ptt_radio_clr(void) {
  PORTG &= ~(1<<0);
  PORTG &= ~(1<<3);
  PORTF &= ~(1<<6);
}

void ext_control_r2_ptt_radio_set(void) {
  PORTG |= (1<<1);
  PORTF |= (1<<1);
  PORTF |= (1<<5);
}

void ext_control_r2_ptt_radio_clr(void) {
  PORTG &= ~(1<<1);
  PORTF &= ~(1<<1);
  PORTF &= ~(1<<5);
}

void ext_control_r1_ptt_amp_set(void) {
  PORTB |= (1<<7);
}

void ext_control_r1_ptt_amp_clr(void) {
  PORTB &= ~(1<<7);
}

void ext_control_r2_ptt_amp_set(void) {
  PORTF |= (1<<0);
}

void ext_control_r2_ptt_amp_clr(void) {
  PORTF &= ~(1<<0);
}

unsigned char ext_control_get_r1_footswitch_input(void) {
  if ((PIND & (1<<7)) == 0)
    return(1);
  
  return(0);
}

unsigned char ext_control_get_r2_footswitch_input(void) {
  if ((PINF & (1<<4)) == 0)
    return(1);
  
  return(0);
}

unsigned char ext_control_get_int_vip_state(void) {
  return((PINB >> 2) & 0x0F);
}

unsigned char ext_control_get_r1_rx_tx_ant_state(void) {
  if (PIND & (1<<4))
    return(0);
  
  return(1);
}

unsigned char ext_control_get_r2_rx_tx_ant_state(void) {
  if (PINF & (1<<2))
    return(0);
  
  return(1);
}

unsigned char ext_control_get_mult_support_state(void) {
  if (PINF & (1<<3))
    return(1);
  
  return(0);
}

unsigned char ext_control_get_priority_state(void) {
  unsigned char state = (PIND >> 5) & 0x03;
  
  if (state == 1)
    return(1);  //R1 WIN
  else if (state == 2)
    return(2); //R2 WIN
    
  return(0); //FIRST WIN
}

unsigned char ext_control_get_mute_on_tx_state(void) {
  if (PINB & (1<<2))
    return(1);
  
  return(0);
}

void ext_control_read_inputs(unsigned char *curr_state) {
  curr_state[EVENT_FOOTSWICH_R1_STATE] = ext_control_get_r1_footswitch_input();
  curr_state[EVENT_FOOTSWICH_R2_STATE] = ext_control_get_r2_footswitch_input();
  curr_state[EVENT_R2_SUPPORT_OP_STATE] = ext_control_get_mult_support_state();
  curr_state[EVENT_PRIORITY_STATE] = ext_control_get_priority_state();
  curr_state[EVENT_R1_TXRX_ANT_SEL] = ext_control_get_r1_rx_tx_ant_state();
  curr_state[EVENT_R2_TXRX_ANT_SEL] = ext_control_get_r2_rx_tx_ant_state();
  curr_state[EVENT_MUTE_ON_TX] = ext_control_get_mute_on_tx_state();
}