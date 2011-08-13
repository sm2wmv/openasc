#include <stdio.h>
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>

#include "main.h"

void init_ports(void) {
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xE8;
	DDRE = 0x9E;
	DDRF = 0x1C;
	DDRG = 0x1F;
}

void init_backlight_contrast(void) {
	TCCR3A = (1<<COM3A1) | (0<<COM3A0) | (0<<WGM31) | (1<<WGM30);// | (1<<COM3B1) | (0<<COM3B0);
	TCCR3B = (0<<WGM33) | (1<<WGM32) | (0<<CS32) | (1<<CS31) | (0<<CS30);
	TCCR3C = 0;
	OCR3AH = 0;
	OCR3AL = (DEFAULT_BACKLIGHT)*2.55f;
	
	//OCR3BH = 0;
	//OCR3BL = 0x10;
}
