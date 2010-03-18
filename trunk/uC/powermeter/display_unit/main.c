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

void set_lcd_contrast(unsigned char level) {
	
}

void set_lcd_backlight(unsigned char level) {
	
}

int main(void) {
	init_ports();
	
	init_backlight_contrast();
	lcdInit();
	lcdClear();
	lcdHome();
	
	output_show_display(0);
	//delay_s(1);
	lcdClear();

	current_coupler.power_limit = 12000;
	
	status.curr_fwd_power = 10450;
	status.curr_ref_power = 100;
	status.curr_vswr = 1.12f;
	
	output_show_display(1);

	while(1) {
		
	}
}