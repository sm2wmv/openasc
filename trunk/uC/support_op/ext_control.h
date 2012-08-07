#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

#include "main.h"

void ext_control_set_led_yellow(enum led_state state);
void ext_control_set_led_red(enum led_state state);

void ext_control_set_relay_k4(enum relay_state state);
void ext_control_set_relay_k5k6(enum relay_state state);
void ext_control_set_relay_k8(enum relay_state state);
void ext_control_set_relay_k9(enum relay_state state);
void ext_control_set_relay_k10(enum relay_state state);
void ext_control_set_relay_k11(enum relay_state state);
void ext_control_set_relay_k13(enum relay_state state);

void ext_control_r1_ptt_radio(enum ptt_state state);
void ext_control_r2_ptt_radio(enum ptt_state state);

void ext_control_r1_ptt_amp(enum ptt_state state);
void ext_control_r2_ptt_amp(enum ptt_state state);

enum ptt_state ext_control_get_r1_footswitch_input(void);
enum ptt_state ext_control_get_r2_footswitch_input(void);

/*! \brief Read the VIP switches on the board 
 *  \return Four bits which represent the configuration of the switches, defines in board.h */
unsigned char ext_control_get_int_vip_state(void);

enum rx_tx_ant_state ext_control_get_r1_rx_tx_ant_state(void);
enum rx_tx_ant_state ext_control_get_r2_rx_tx_ant_state(void);

enum mult_support_state ext_control_get_mult_support_state(void);

enum priority_state ext_control_get_priority_state(void);

#endif

