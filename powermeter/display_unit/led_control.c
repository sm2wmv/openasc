#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

#include "led_control.h"
#include "board.h"

void led_set_fwd(unsigned char level) {
	//Turn off all leds
	PORTE &= ~(1<<LED_FWD_POWER_BIT0);
	PORTB = 0x00;
	PORTG &= ~(1<<LED_FWD_POWER_BIT9);	
	PORTG &= ~(1<<LED_FWD_POWER_BIT10);
	PORTD &= ~(1<<LED_FWD_POWER_BIT11);
	PORTD &= ~(1<<LED_FWD_POWER_BIT12);
	PORTD &= ~(1<<LED_FWD_POWER_BIT13);
	PORTG &= ~(1<<LED_FWD_POWER_BIT14);
	
	if (level > 0)
		PORTE |= (1<<LED_FWD_POWER_BIT0);
	if (level > 1)
		PORTB |= (1<<LED_FWD_POWER_BIT1);
	if (level > 2)
		PORTB |= (1<<LED_FWD_POWER_BIT2);
	if (level > 3)
		PORTB |= (1<<LED_FWD_POWER_BIT3);
	if (level > 4)
		PORTB |= (1<<LED_FWD_POWER_BIT4);
	if (level > 5)
		PORTB |= (1<<LED_FWD_POWER_BIT5);
	if (level > 6)
		PORTB |= (1<<LED_FWD_POWER_BIT6);
	if (level > 7)
		PORTB |= (1<<LED_FWD_POWER_BIT7);
	if (level > 8)
		PORTB |= (1<<LED_FWD_POWER_BIT8);
	if (level > 9)
		PORTG |= (1<<LED_FWD_POWER_BIT9);
	if (level > 10)
		PORTG |= (1<<LED_FWD_POWER_BIT10);
	if (level > 11)
		PORTD |= (1<<LED_FWD_POWER_BIT11);
	if (level > 12)
		PORTD |= (1<<LED_FWD_POWER_BIT12);
	if (level > 13)
		PORTD |= (1<<LED_FWD_POWER_BIT13);
	if (level > 14)
		PORTG |= (1<<LED_FWD_POWER_BIT14);	
}

void led_set_ref(unsigned char level) {
	//Turn off all leds
	PORTG &= ~(1<<LED_REF_POWER_BIT0);
	PORTC &= ~(1<<LED_REF_POWER_BIT1);
	PORTC &= ~(1<<LED_REF_POWER_BIT2);
	PORTC &= ~(1<<LED_REF_POWER_BIT3);
	PORTC &= ~(1<<LED_REF_POWER_BIT4);
	PORTC &= ~(1<<LED_REF_POWER_BIT5);
	PORTE &= ~(1<<LED_REF_POWER_BIT6);
	
	if (level > 0)
		PORTG |= (1<<LED_REF_POWER_BIT0);
	if (level > 1)
		PORTC |= (1<<LED_REF_POWER_BIT1);
	if (level > 2)
		PORTC |= (1<<LED_REF_POWER_BIT2);
	if (level > 3)
		PORTC |= (1<<LED_REF_POWER_BIT3);
	if (level > 4)
		PORTC |= (1<<LED_REF_POWER_BIT4);
	if (level > 5)
		PORTC |= (1<<LED_REF_POWER_BIT5);
	if (level > 6)
		PORTE |= (1<<LED_REF_POWER_BIT6);	
}

void led_change_fwd_single(unsigned char level, enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		switch (level) {
			case 1:	PORTE |= (1<<LED_FWD_POWER_BIT0);
				break;
			case 2: PORTB |= (1<<LED_FWD_POWER_BIT1);
				break;
			case 3: PORTB |= (1<<LED_FWD_POWER_BIT2);
				break;
			case 4: PORTB |= (1<<LED_FWD_POWER_BIT3);
				break;
			case 5: PORTB |= (1<<LED_FWD_POWER_BIT4);
				break;
			case 6: PORTB |= (1<<LED_FWD_POWER_BIT5);
				break;
			case 7: PORTB |= (1<<LED_FWD_POWER_BIT6);
				break;
			case 8: PORTB |= (1<<LED_FWD_POWER_BIT7);
				break;
			case 9: PORTB |= (1<<LED_FWD_POWER_BIT8);
				break;
			case 10: PORTG |= (1<<LED_FWD_POWER_BIT9);
				break;
			case 11: PORTG |= (1<<LED_FWD_POWER_BIT10);
				break;
			case 12: PORTD |= (1<<LED_FWD_POWER_BIT11);
				break;
			case 13: PORTD |= (1<<LED_FWD_POWER_BIT12);
				break;
			case 14: PORTD |= (1<<LED_FWD_POWER_BIT13);
				break;
			case 15: PORTG |= (1<<LED_FWD_POWER_BIT14);
				break;
		}
	}
	else if (state == LED_STATE_OFF) {
		switch (level) {
			case 1:	PORTE &= ~(1<<LED_FWD_POWER_BIT0);
				break;
			case 2: PORTB &= ~(1<<LED_FWD_POWER_BIT1);
				break;
			case 3: PORTB &= ~(1<<LED_FWD_POWER_BIT2);
				break;
			case 4: PORTB &= ~(1<<LED_FWD_POWER_BIT3);
				break;
			case 5: PORTB &= ~(1<<LED_FWD_POWER_BIT4);
				break;
			case 6: PORTB &= ~(1<<LED_FWD_POWER_BIT5);
				break;
			case 7: PORTB &= ~(1<<LED_FWD_POWER_BIT6);
				break;
			case 8: PORTB &= ~(1<<LED_FWD_POWER_BIT7);
				break;
			case 9: PORTB &= ~(1<<LED_FWD_POWER_BIT8);
				break;
			case 10: PORTG &= ~(1<<LED_FWD_POWER_BIT9);
				break;
			case 11: PORTG &= ~(1<<LED_FWD_POWER_BIT10);
				break;
			case 12: PORTD &= ~(1<<LED_FWD_POWER_BIT11);
				break;
			case 13: PORTD &= ~(1<<LED_FWD_POWER_BIT12);
				break;
			case 14: PORTD &= ~(1<<LED_FWD_POWER_BIT13);
				break;
			case 15: PORTG &= ~(1<<LED_FWD_POWER_BIT14);
				break;
		}
	}
}

void led_change_ref_single(unsigned char level, enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		switch(level) {
			case 1: PORTG |= (1<<LED_REF_POWER_BIT0);
				break;
			case 2: PORTC |= (1<<LED_REF_POWER_BIT1);
				break;
			case 3: PORTC |= (1<<LED_REF_POWER_BIT2);
				break;
			case 4: PORTC |= (1<<LED_REF_POWER_BIT3);
				break;
			case 5: PORTC |= (1<<LED_REF_POWER_BIT4);
				break;
			case 6: PORTC |= (1<<LED_REF_POWER_BIT5);
				break;
			case 7: PORTE |= (1<<LED_REF_POWER_BIT6);
				break;
		}
	}
	else if (state == LED_STATE_OFF) {
		switch(level) {
			case 1: PORTG &= ~(1<<LED_REF_POWER_BIT0);
			break;
			case 2: PORTC &= ~(1<<LED_REF_POWER_BIT1);
			break;
			case 3: PORTC &= ~(1<<LED_REF_POWER_BIT2);
			break;
			case 4: PORTC &= ~(1<<LED_REF_POWER_BIT3);
			break;
			case 5: PORTC &= ~(1<<LED_REF_POWER_BIT4);
			break;
			case 6: PORTC &= ~(1<<LED_REF_POWER_BIT5);
			break;
			case 7: PORTE &= ~(1<<LED_REF_POWER_BIT6);
			break;
		}
	}
}

void led_set_vswr_state(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTG |= (1<<LED_VSWR_ALARM_BIT);
	else
		PORTG &= ~(1<<LED_VSWR_ALARM_BIT);
}
