#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>

#include "lcd.h"
#include "led_control.h"
#include "input.h"

unsigned char output_str[20];
unsigned char str_len=0;

void output_show_display(unsigned char index) {
	if (index == 0) {
		lcdPrintData("  PowerMeter v1.0",17);
		lcdGotoXY(0,1);
		lcdPrintData("     by SM2WMV",14);
	}
	else if (index == 1) {
		lcdClear();
		lcdGotoXY(0,0);
		
		sprintf(output_str,"FWD: %iW",status.curr_fwd_power);
		str_len = strlen(output_str);
					
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(12,0);
		sprintf(output_str,"SWR:%1.2f",input_calculate_vswr());
		str_len = strlen(output_str);
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(0,1);
		lcdProgressBar(status.curr_fwd_power,current_coupler.power_limit,20);
	}
	else if (index == 2) {
		lcdClear();
		lcdGotoXY(0,0);
		
		sprintf(output_str,"FWD: %iW",status.curr_fwd_power);
		str_len = strlen(output_str);
					
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(12,0);
		sprintf(output_str,"SWR:%1.2f",input_calculate_vswr());
		str_len = strlen(output_str);
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(0,1);	
		sprintf(output_str,"REF: %iW",status.curr_ref_power);
		str_len = strlen(output_str);
					
		lcdPrintData(output_str,str_len);
	}
}

void output_update_leds(void) {
	if (settings.bargraph_mode == BARGRAPH_MODE_RMS) {
		led_set_fwd(((float)(status.curr_fwd_power) / (float)(settings.led_fwd_power_high))*15);
		led_set_ref(((float)(status.curr_ref_power) / (float)(settings.led_ref_power_high))*7);
	}
	
	if ((status.curr_vswr*100) >= settings.vswr_warn_limit) {
		led_set_vswr_state(LED_STATE_ON);
	}
}