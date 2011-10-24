#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "main.h"
#include "board.h"
#include "led_control.h"
#include "input.h"
#include "init.h"
#include "lcd.h"
#include "output.h"
#include "a2d.h"

void set_lcd_contrast(unsigned char level) {
	
}

void set_lcd_backlight(unsigned char level) {
	OCR3AL = level*2.55f;
}

int main(void) {
	init_ports();
	
	init_backlight_contrast();
	lcdInit();
	lcdClear();
	lcdHome();
	
	a2dInit();
	a2dSetPrescaler(ADC_PRESCALE_DIV16);
	a2dSetReference(ADC_REFERENCE_AREF);
	
	output_show_display(0);
	led_set_fwd(15);
	led_set_ref(7);
	led_set_vswr_state(LED_STATE_ON);
	delay_s(1);
	lcdClear();
	
	led_set_vswr_state(LED_STATE_OFF);
	
	current_coupler.fwd_scale_value[0] = 3782;
	current_coupler.ref_scale_value[0] = 3782;
	current_coupler.power_limit = 200;
	
	settings.vswr_warn_limit = 10;
	settings.vswr_shutdown_limit = 300;
	settings.led_fwd_power_high = 120;
	settings.led_ref_power_high = 20;
	
	output_update_leds();
	
	short res = 0;
	
	unsigned char count=0;
	
	while(1) {
		status.curr_fwd_ad_value = a2dConvert10bit(ADC_CH_ADC0);
		status.curr_ref_ad_value = a2dConvert10bit(ADC_CH_ADC1);
		
		input_calculate_power();
		status.curr_vswr = input_calculate_vswr();
		
		if (count > 250) {
			output_show_display(2);
			count = 0;
		}

		count++;
		output_update_leds();
		delay_ms(1);
	}
}