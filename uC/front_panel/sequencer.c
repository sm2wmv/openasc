#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "radio_interface.h"
#include "sequencer.h"
#include "main.h"
#include "led_control.h"
#include "usart.h"
#include "../global.h"
#include "../event_queue.h"
#include "antenna_ctrl.h"

#define PTT_ACTIVE_FOOTSWITCH			0
#define PTT_ACTIVE_RADIO_SENSE		1
#define PTT_ACTIVE_COMPUTER_RTS		2

//TODO: Fix so that we don't get problems with having computer AND footswitch PTTed at the same time if
//      the different amp/radio/inhibit are enabled on different inputs...THIS IS VERY IMPORTANT THAT IT WORKS
//      PROPERLY!!! Maybe solution is to only use FIRST WINS?
//TODO: Finish the radio sense input stuff
//TODO: Finish the computer rts input stuff

//! The status of the PTT, see defines above
unsigned char ptt_active = 0;

struct_ptt ptt_sequencer;

void sequencer_init_dummy(void) {
	ptt_sequencer.footswitch.radio_pre_delay = 100;
	ptt_sequencer.footswitch.radio_post_delay = 0;
	ptt_sequencer.footswitch.amp_pre_delay = 50;
	ptt_sequencer.footswitch.amp_post_delay = 50;
	ptt_sequencer.footswitch.antennas_post_delay = 100;
	ptt_sequencer.footswitch.active = (1<<SEQUENCER_PTT_RADIO_ENABLED) | (1<<SEQUENCER_PTT_AMP_ENABLED) | (1<<SEQUENCER_PTT_INHIBIT_ENABLED);

	ptt_sequencer.radio_sense.amp_pre_delay = 0;
	ptt_sequencer.radio_sense.amp_post_delay = 50;
	ptt_sequencer.radio_sense.antennas_post_delay = 200;
	ptt_sequencer.radio_sense.active = (1<<SEQUENCER_PTT_AMP_ENABLED) | (1<<SEQUENCER_PTT_INHIBIT_ENABLED);
	
	ptt_sequencer.ptt_input = (1<<PTT_INPUT_FOOTSWITCH) | (1<<PTT_INPUT_RADIO_SENSE_LO) | (1<<PTT_INPUT_INHIBIT_POLARITY);// | (1<<PTT_INPUT_COMPUTER_RTS);
}

void sequencer_footsw_pressed(void) {
	if (ptt_sequencer.ptt_input & (1<<PTT_INPUT_FOOTSWITCH)) {
		//Check so that the computer does not already PTT the device 
		if (antenna_ctrl_antenna_selected() != 0) {
			if (ptt_sequencer.footswitch.active & (1<<SEQUENCER_PTT_AMP_ENABLED)) {
				event_add_message((void *)radio_amp_ptt_active, ptt_sequencer.footswitch.amp_pre_delay * 10, SEQUENCER_EVENT_TYPE_PTT_AMP_ON);
			}
			
			if (ptt_sequencer.footswitch.active & (1<<SEQUENCER_PTT_RADIO_ENABLED)) {
				event_add_message((void *)radio_ptt_active, ptt_sequencer.footswitch.radio_pre_delay * 10, SEQUENCER_EVENT_TYPE_PTT_RADIO_ON);
			}
			
			if (ptt_sequencer.footswitch.active & (1<<SEQUENCER_PTT_INHIBIT_ENABLED)) {
				if ((ptt_sequencer.ptt_input & (1<<PTT_INPUT_INHIBIT_POLARITY)) == 0)	{
					event_add_message((void *)radio_inhibit_low, ptt_sequencer.footswitch.inhibit_pre_delay * 10, SEQUENCER_EVENT_TYPE_PTT_INHIBIT_ON);
				}
				else {
					event_add_message((void *)radio_inhibit_high, ptt_sequencer.footswitch.inhibit_pre_delay * 10, SEQUENCER_EVENT_TYPE_PTT_INHIBIT_ON);
				}
			}
		}
		
		//Show that the PTT is (also) activated by the footswitch now
		ptt_active |= (1<<PTT_ACTIVE_FOOTSWITCH);
	}
}

void sequencer_footsw_released(void) {
	//Check if the footswitch input is enabled
	if (ptt_sequencer.ptt_input & (1<<PTT_INPUT_FOOTSWITCH)) {
		//If the computer does PTT, we cannot stop the sequence, the computer will
		if (((ptt_sequencer.ptt_input & (1<<PTT_INPUT_COMPUTER_RTS)) && ((ptt_active & (1<<PTT_ACTIVE_COMPUTER_RTS)) == 0)) || (ptt_sequencer.ptt_input & (1<<PTT_INPUT_COMPUTER_RTS)) == 0) {
			//Checks so that the footswitch wasn't relased premature, if it was, we drop the message to PTT the radio or amp
			//That way if we accidentally press the footswitch shortly we don't need to go through the sequencer cycle if the
			//delays are longer than the footswitch was pressed (not very likely though)
			if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_RADIO_ON) == 0) {
				event_add_message((void *)radio_ptt_deactive, ptt_sequencer.footswitch.radio_post_delay * 10, SEQUENCER_EVENT_TYPE_PTT_RADIO_OFF);
			}
				
			if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_AMP_ON) == 0) {
				event_add_message((void *)radio_amp_ptt_deactive, ptt_sequencer.footswitch.amp_post_delay * 10, SEQUENCER_EVENT_TYPE_PTT_AMP_OFF);
			}
				
			if (event_queue_drop_id(SEQUENCER_EVENT_TYPE_PTT_INHIBIT_ON) == 0) {
				if ((ptt_sequencer.ptt_input & (1<<PTT_INPUT_INHIBIT_POLARITY)) == 0)	//Inhibit polarity = active low
					event_add_message((void *)radio_inhibit_high, ptt_sequencer.footswitch.inhibit_post_delay * 10, SEQUENCER_EVENT_TYPE_PTT_INHIBIT_OFF);
				else
					event_add_message((void *)radio_inhibit_low, ptt_sequencer.footswitch.inhibit_post_delay * 10, SEQUENCER_EVENT_TYPE_PTT_INHIBIT_OFF);
			}
		}

		ptt_active &= ~(1<<PTT_ACTIVE_FOOTSWITCH);
	}
}

void sequencer_computer_rts_activated(void) {
	printf("COMPUTER_RTS_ACTIVE\n");
}

void sequencer_computer_rts_deactivated(void) {
	printf("COMPUTER_RTS_DEACTIVE\n");
}

void sequencer_radio_sense_activated(void) {
}

void sequencer_radio_sense_deactivated(void) {
}

/*! Retrieve the polarity of the Computer RTS signal 
    \return 1 if the polarity is active low (inverted), 0 otherwise */
unsigned char sequencer_get_rts_polarity() {
	return(ptt_sequencer.ptt_input & (1<<PTT_INPUT_INVERTED_COMPUTER_RTS));
}

/*! Retrieve the polarity of the radio sense signal
    \return 1 if the polarity is active low (inverted), 0 otherwise */
unsigned char sequencer_get_sense_polarity() {
	return(ptt_sequencer.ptt_input & (1<<PTT_INPUT_INVERTED_RADIO_SENSE));
}

/*! Will return if the PTT is active or not
    \return The state of the ptt_active variable, 0 if nothing is PTTing the radio */
unsigned char sequencer_ptt_active(void) {
	return(ptt_active);
}

/*! Retrieve if the radio sense should be sensed from upper floor or bottom
    \return 0 if lower floor, 1 if upper floor */
unsigned char sequencer_get_radio_sense(void) {
	if ((ptt_sequencer.ptt_input & (1<<PTT_INPUT_RADIO_SENSE_UP)) != 0)
		return(1);
	else
		return(0);
}