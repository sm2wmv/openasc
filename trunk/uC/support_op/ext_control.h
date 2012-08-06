#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

enum led_state {
  LED_ON,
  LED_OFF
};

enum relay_state {
  RELAY_ON,
  RELAY_OFF
};

enum ptt_state {
  PTT_ACTIVE,
  PTT_DEACTIVE
};

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

#endif
