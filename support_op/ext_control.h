#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

#include "main.h"

void ext_control_led_yellow_set(void);
void ext_control_led_yellow_clr(void);
void ext_control_led_red_set(void);
void ext_control_led_red_clr(void);

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
void ext_control_relay_k13_set(void);
void ext_control_relay_k13_clr(void);
void ext_control_relay_k14_set(void);
void ext_control_relay_k14_clr(void);
void ext_control_relay_transfer_set(void);
void ext_control_relay_transfer_clr(void);

void ext_control_r1_ptt_radio_set(void);
void ext_control_r1_ptt_radio_clr(void);
void ext_control_r2_ptt_radio_set(void);
void ext_control_r2_ptt_radio_clr(void);

void ext_control_r1_ptt_amp_set(void);
void ext_control_r1_ptt_amp_clr(void);
void ext_control_r2_ptt_amp_set(void);
void ext_control_r2_ptt_amp_clr(void);

unsigned char ext_control_get_r1_footswitch_input(void);
unsigned char ext_control_get_r2_footswitch_input(void);

/*! \brief Read the VIP switches on the board 
 *  \return Four bits which represent the configuration of the switches, defines in board.h */
unsigned char ext_control_get_int_vip_state(void);

unsigned char ext_control_get_r1_rx_tx_ant_state(void);
unsigned char ext_control_get_r2_rx_tx_ant_state(void);

unsigned char ext_control_get_mult_support_state(void);

unsigned char ext_control_get_priority_state(void);

unsigned char ext_control_get_mute_on_tx_state(void);

void ext_control_read_inputs(unsigned char *curr_state);

#endif

