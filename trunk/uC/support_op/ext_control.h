#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

enum led_state {
  LED_STATE_ON,
  LED_STATE_OFF
};

void ext_control_set_led_yellow(enum led_state state);
void ext_control_set_led_red(enum led_state state);

#endif
