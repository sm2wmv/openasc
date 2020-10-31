#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_control.h"
#include "main.h"

uint8_t state_ptt_asc = 0;

void ext_control_led_yellow_set(void) {
  PORTA |= (1<<0);
}

void ext_control_led_yellow_clr(void) {
  PORTA &= ~(1<<0);
}

void ext_control_led_red_set(void) {
  PORTA |= (1<<1);
  
  PRINTF("EXT_CTRL >> RED LED SET\r\n");
}

void ext_control_led_red_clr(void) {
  PORTA &= ~(1<<1);
  
  PRINTF("EXT_CTRL >> RED LED CLR\r\n");
}

void ext_control_relay_k4_set(void) {
  PORTB |= (1<<0);
  
  PRINTF("EXT_CTRL >> K4 SET\r\n");
}

void ext_control_relay_k4_clr(void) {
  PORTB &= ~(1<<0);
  
  PRINTF("EXT_CTRL >> K4 CLR\r\n");
}

void ext_control_relay_k5k6_set(void) {
  PORTE |= (1<<7);
  
  PRINTF("EXT_CTRL >> K5K6 SET\r\n");
}

void ext_control_relay_k5k6_clr(void) {
  PORTE &= ~(1<<7);
  
  PRINTF("EXT_CTRL >> K5K6 CLR\r\n");
}

void ext_control_relay_k8_set(void) {
  PORTE |= (1<<6);
  
  PRINTF("EXT_CTRL >> K8 SET\r\n");
}

void ext_control_relay_k8_clr(void) {
  PORTE &= ~(1<<6);
  
  PRINTF("EXT_CTRL >> K8 CLR\r\n");
}

void ext_control_relay_k9_set(void) {
  PORTE |= (1<<5);
  
  PRINTF("EXT_CTRL >> K9 SET\r\n");
}

void ext_control_relay_k9_clr(void) {
  PORTE &= ~(1<<5);
  
  PRINTF("EXT_CTRL >> K9 CLR\r\n");
}

void ext_control_relay_k10_set(void) {
  PORTE |= (1<<3);
  
  PRINTF("EXT_CTRL >> K10 SET\r\n");
}

void ext_control_relay_k10_clr(void) {
  PORTE &= ~(1<<3);
  
  PRINTF("EXT_CTRL >> K10 CLR\r\n");
}

void ext_control_relay_k11_set(void) {
  PORTE |= (1<<4);
  
  PRINTF("EXT_CTRL >> K11 SET\r\n");
}

void ext_control_relay_k11_clr(void) {
  PORTE &= ~(1<<4);
  
  PRINTF("EXT_CTRL >> K11 CLR\r\n");
}

void ext_control_relay_k3_set(void) {
  PORTE |= (1<<2);
  
  PRINTF("EXT_CTRL >> K3 SET\r\n");
}

void ext_control_relay_k3_clr(void) {
  PORTE &= ~(1<<2);
  
  PRINTF("EXT_CTRL >> K3 CLR\r\n");
}

void ext_control_relay_k12_set(void) {
  PORTA |= (1<<6);
  
  PRINTF("EXT_CTRL >> K12 SET\r\n");
}

void ext_control_relay_k12_clr(void) {
  PORTA &= ~(1<<6);
  
  PRINTF("EXT_CTRL >> K12 CLR\r\n");
}

void ext_control_relay_k14_set(void) {
  PORTA |= (1<<7);
  
  PRINTF("EXT_CTRL >> K14 SET\r\n");
}

void ext_control_relay_k14_clr(void) {
  PORTA &= ~(1<<7);
  
  PRINTF("EXT_CTRL >> K14 CLR\r\n");
}

void ext_control_relay_transfer_clear_all(void) {
  PORTA &= ~(1<<4);
  PORTA &= ~(1<<5);
  
  PRINTF("EXT_CTRL >> TRANSFER CLEAR ALL\r\n");
}

void ext_control_relay_transfer_set(void) {
  PORTA |= (1<<4);
  event_add_message(ext_control_relay_transfer_clear_all,TRANSFER_RELAY_PULSE_TIME,EVENT_QUEUE_ID_TRANSFER_RELAY);
  
  PRINTF("EXT_CTRL >> TRANSFER SET\r\n");
}

void ext_control_relay_transfer_clr(void) {
  PORTA |= (1<<5);
  event_add_message(ext_control_relay_transfer_clear_all,TRANSFER_RELAY_PULSE_TIME,EVENT_QUEUE_ID_TRANSFER_RELAY);
  
  PRINTF("EXT_CTRL >> TRANSFER CLR\r\n");
}

void ext_control_run_ptt_asc_set(void) {
  PORTB &= ~(1<<4);
  state_ptt_asc = 1;
  PRINTF("EXT_CTRL >> PTT ASC SET\r\n");
}

void ext_control_run_ptt_asc_clr(void) {
  PORTB |= (1<<4);
  
  state_ptt_asc = 0;
  PRINTF("EXT_CTRL >> PTT ASC CLR\r\n");
}
void ext_control_run_ptt_radio_set(void) {
  PORTG |= (1<<1);	//Set RUN PTT LED on RUN BOX
  PORTG |= (1<<3);  //PTT RUN RADIO
  PORTF |= (1<<6);	//Set RUN PTT LED on INBAND BOX
  
  PRINTF("EXT_CTRL >> PTT RUN SET\r\n");
}

void ext_control_run_ptt_radio_clr(void) {
  PORTG &= ~(1<<1);  	//Clear RUN PTT LED on RUN BOX
  PORTG &= ~(1<<3);	 	//PTT OFF RUN RADIO
  PORTF &= ~(1<<6);		//CLR RUN PTT LED on INBAND BOX
  
  PRINTF("EXT_CTRL >> PTT RUN CLR\r\n");
}

void ext_control_inband_ptt_radio_set(void) {
  PORTG |= (1<<0);
  PORTF |= (1<<1);
  PORTF |= (1<<5);
  
  PRINTF("EXT_CTRL >> PTT INBAND SET\r\n");
}

void ext_control_inband_ptt_radio_clr(void) {
  PORTG &= ~(1<<0);
  PORTF &= ~(1<<1);
  PORTF &= ~(1<<5);
  
  PRINTF("EXT_CTRL >> PTT INBAND CLR\r\n");
}

void ext_control_run_ptt_amp_set(void) {
  PORTB |= (1<<7);
  
  PRINTF("EXT_CTRL >> PTT RUN AMP SET\r\n");
}

void ext_control_run_ptt_amp_clr(void) {
  PORTB &= ~(1<<7);
  
  PRINTF("EXT_CTRL >> PTT RUN AMP CLR\r\n");
}

void ext_control_inband_ptt_amp_set(void) {
  PORTF |= (1<<0);
  
  PRINTF("EXT_CTRL >> PTT INBAND AMP SET\r\n");
}

void ext_control_inband_ptt_amp_clr(void) {
  PORTF &= ~(1<<0);
  
  PRINTF("EXT_CTRL >> PTT INBAND AMP CLR\r\n");
}

unsigned char ext_control_get_run_openasc_ptt_input(void) {
  if ((PINB & (1<<5)) == 0)
    return(1);
  
  return(0);
}

unsigned char ext_control_get_run_footswitch_input(void) {
  if ((PIND & (1<<7)) == 0)
    return(1);
  
  return(0);
}

unsigned char ext_control_get_inband_footswitch_input(void) {
  if ((PINF & (1<<4)) == 0)
    return(1);
  
  return(0);
}

unsigned char ext_control_get_int_vip_state(void) {
  return((PINB >> 2) & 0x0F);
}

unsigned char ext_control_get_run_rx_state(void) {
  if (PINB & (1<<5))
    return(0);
  
  return(1);
}

unsigned char ext_control_get_inband_rx_state(void) {
	if ((PINF & (1<<3)) && (PINA & (1<<3))) {
		return(0); //R1 ANT
	}
	else if (((PINF & (1<<3)) == 0) && (PINA & (1<<3))) {
		return(1); //RX ANT
	}
	else
		return(0);//TX ANT
}

unsigned char ext_control_get_inband_tx_selelct_state(void) {
	if (PINF & (1<<2))
    return(0);
  
  return(1);
}

unsigned char ext_control_get_inband_rx_r1_state(void) {
	if ((PINF & (1<<3)) && (PINA & (1<<3))) {
		return(1); //R1 ANT
	}
	else if (((PINF & (1<<3)) == 0) && (PINA & (1<<3))) {
		return(0); //RX ANT
	}
	else
		return(0);//TX ANT  
}

unsigned char ext_control_get_openasc_ptt_in(void) {
  if (PIND & (1<<4))
    return(0);
    
  return(1);
}

unsigned char ext_control_get_priority_state(void) {
  unsigned char state = (PIND >> 5) & 0x03;

  if (state == 1)
    return(1);  //R1 WIN
  else if (state == 2)
    return(2); //R2 WIN
    
  return(0); //FIRST WIN
}


uint8_t ext_control_read_inputs() {
  uint8_t state = 0;
  
  if (ext_control_get_run_footswitch_input())
    state |= (1<<EVENT_FOOTSWICH_RUN_STATE);
    
  if (ext_control_get_inband_footswitch_input())
    state |= (1<<EVENT_FOOTSWICH_INBAND_STATE);

  if (ext_control_get_inband_tx_selelct_state())
    state |= (1<<EVENT_INBAND_TX_SELECT_STATE);

  if (ext_control_get_priority_state())
    state |= (1<<EVENT_PRIORITY_STATE);
  
  if (ext_control_get_run_rx_state())
    state |= (1<<EVENT_RUN_RX_ANT_SELECT);
  
  if (ext_control_get_inband_rx_state())
    state |= (1<<EVENT_INBAND_RX_ANT_SELECT);
  
  if (ext_control_get_inband_rx_r1_state())
    state |= (1<<EVENT_INBAND_RX_ANT_R1_SELET);
  
  if (ext_control_get_openasc_ptt_in())
    state |= (1<<EVENT_ASC_INPUT_STATE);
  
  return(state);
}

uint8_t ext_control_get_run_ptt_asc(void) {
  return(state_ptt_asc);
}
