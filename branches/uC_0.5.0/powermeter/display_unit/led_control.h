#ifndef _LED_CONTROL_H_
#define _LED_CONTROL_H_

enum enum_led_state {
	LED_STATE_ON,LED_STATE_OFF
};

void led_set_fwd(unsigned char level);
void led_set_ref(unsigned char level);
void led_change_fwd_single(unsigned char level, enum enum_led_state state);
void led_change_ref_single(unsigned char level, enum enum_led_state state);
void led_set_vswr_state(enum enum_led_state state);

#endif