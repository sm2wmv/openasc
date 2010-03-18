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
		lcdGotoXY(0,0);
		if (status.curr_fwd_power > 9999) {
			sprintf(output_str,"FWD:%iW",status.curr_fwd_power);
			str_len = 10;
		}
		else {
			sprintf(output_str,"FWD: %iW",status.curr_fwd_power);
			str_len = 10;
		}
					
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(12,0);
		sprintf(output_str,"SWR:%1.2f",input_calculate_vswr());
		str_len = 8;
		lcdPrintData(output_str,str_len);
		
		lcdGotoXY(0,1);
		lcdProgressBar(status.curr_fwd_power/100,current_coupler.power_limit/100,20);
	}
}