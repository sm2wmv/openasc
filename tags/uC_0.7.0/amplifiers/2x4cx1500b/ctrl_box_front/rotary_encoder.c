/*! \file 4cx1500b/ctrl_box_front/rotary_encoder.c
 *  \brief Rotary encoder functions
 *  \ingroup amp_front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "4cx1500b/ctrl_box_front/rotary_encoder.c" \endcode
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>

#include "board.h"
#include "rotary_encoder.h"

//! The last state of the encoder
unsigned char encoder_last_state = 0;

//! The current state of the encoder
unsigned char encoder_current_state = 0;

/*! \brief Poll the rotary encoder pin states 
 *  \return The state of the rotary encoder pins */
unsigned char poll_encoder_state(void) {
	return(((PINE & (1<<PULSE_SENSOR_BIT1))>>PULSE_SENSOR_BIT1) | ((PINE & (1<<PULSE_SENSOR_BIT2))>>(PULSE_SENSOR_BIT2-1)));
}

/*! \brief Poll the rotary encoder 
 *  \return Returns 0 if nothing happened, -1 if rotary CCW and 1 if CW */
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