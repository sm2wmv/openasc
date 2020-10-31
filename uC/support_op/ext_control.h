#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

#include "main.h"

void ext_control_led_yellow_set(void);
void ext_control_led_yellow_clr(void);
void ext_control_led_red_set(void);
void ext_control_led_red_clr(void);

void ext_control_relay_k3_set(void);
void ext_control_relay_k3_clr(void);
void ext_control_relay_k4_set(void);
void ext_control_relay_k4_clr(void);
void ext_control_relay_k5k6_set(void);
void ext_control_relay_k5k6_clr(void);
void ext_control_relay_k8_set(void);
void ext_control_relay_k8_clr(void);
void ext_control_relay_k9_set(void);
void ext_control_relay_k9_clr(void);
void ext_control_relay_k10_set(void);
void ext_control_relay_k10_clr(void);
void ext_control_relay_k11_set(void);
void ext_control_relay_k11_clr(void);
void ext_control_relay_k12_set(void);
void ext_control_relay_k12_clr(void);
void ext_control_relay_k14_set(void);
void ext_control_relay_k14_clr(void);
void ext_control_relay_transfer_set(void);
void ext_control_relay_transfer_clr(void);

uint8_t ext_control_get_run_ptt_asc(void);

void ext_control_run_ptt_asc_set(void);
void ext_control_run_ptt_asc_clr(void);

void ext_control_run_ptt_radio_set(void);
void ext_control_run_ptt_radio_clr(void);
void ext_control_inband_ptt_radio_set(void);
void ext_control_inband_ptt_radio_clr(void);

void ext_control_run_ptt_amp_set(void);
void ext_control_run_ptt_amp_clr(void);
void ext_control_inband_ptt_amp_set(void);
void ext_control_inband_ptt_amp_clr(void);

unsigned char ext_control_get_run_openasc_ptt_input(void);
unsigned char ext_control_get_run_footswitch_input(void);
unsigned char ext_control_get_inband_footswitch_input(void);

/*! \brief Read the VIP switches on the board 
 *  \return Four bits which represent the configuration of the switches, defines in board.h */
unsigned char ext_control_get_int_vip_state(void);

unsigned char ext_control_get_run_rx_state(void);
unsigned char ext_control_get_inband_rx_state(void);
unsigned char ext_control_get_inband_rx_r1_state(void);

unsigned char ext_control_get_inband_tx_selelct_state(void);

unsigned char ext_control_get_priority_state(void);

uint8_t ext_control_read_inputs();

#endif

