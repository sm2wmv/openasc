#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "board.h"
#include "rotary_encoder.h"

unsigned char encoder_last_state = 0;
unsigned char encoder_current_state = 0;

unsigned char poll_encoder_state(void) {
	return(((PINE & (1<<PULSE_SENSOR_BIT1))>>PULSE_SENSOR_BIT1) | ((PINE & (1<<PULSE_SENSOR_BIT2))>>(PULSE_SENSOR_BIT2-1)));
}

//Returns 0 if nothing happened, -1 if rotary CCW and 1 if CW
int rotary_encoder_poll(void) {
	int retval = 0;
	encoder_current_state = poll_encoder_state();

	if (encoder_current_state != encoder_last_state) {
		if (((encoder_current_state == 3) && (encoder_last_state == 2)) || ((encoder_current_state == 0) && (encoder_last_state == 1)))
			retval = 1;
		else if (((encoder_current_state == 3) && (encoder_last_state == 1)) || ((encoder_current_state == 0) && (encoder_last_state == 2)))
			retval = -1;

		encoder_last_state = encoder_current_state;
	}

	return(retval);
}