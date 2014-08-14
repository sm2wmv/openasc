#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "main.h"
#include "ext_control.h"
#include "../../../delay.h"
#include "../../../event_queue.h"

unsigned int curr_input_state = 0;
unsigned int prev_input_state = 0;

#define EXT_CONTROL_MAINS_STATUS  0
#define EXT_CONTROL_HV_STATUS     1
#define EXT_CONTROL_PTT_STATUS    2

static unsigned char curr_state;

void ext_control_init(void) {
  curr_input_state = 0;
  
  /* Loads the current switch positions into the state buffer */
  if ((PINA & (1<<1)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_MODE_SW_BIT);
  if ((PINA & (1<<0)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_DISPLAY_UP_BIT);
  if ((PINK & (1<<0)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_DISPLAY_DOWN_BIT);
  if ((PINK & (1<<1)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SAVE_BIT);
  if ((PINK & (1<<2)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_RECALL_BIT);
  if ((PINA & (1<<7)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_TUNE_UP_BIT);
  if ((PINA & (1<<6)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_TUNE_DOWN_BIT);
  if ((PINA & (1<<5)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_LOAD_UP_BIT);
  if ((PINA & (1<<4)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_LOAD_DOWN_BIT);  
  if ((PINA & (1<<3)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SEGMENT_HIGH_BIT);
  if ((PINA & (1<<2)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SEGMENT_LOW_BIT);
  
//  prev_input_state = curr_input_state;
}

void ext_control_set_mains_on(void) {
  PORTG &= ~(1<<2); //PTT OFF
  PORTJ &= ~(1<<6); //HV OFF
  curr_state &= ~(1<<EXT_CONTROL_HV_STATUS);
  curr_state &= ~(1<<EXT_CONTROL_PTT_STATUS);  
  
  PORTJ |= (1<<5);  //Turn mains on
  
  curr_state |= (1<<EXT_CONTROL_MAINS_STATUS);
}

void ext_control_set_mains_off(void) {
  PORTG &= ~(1<<2); //PTT OFF
  PORTJ &= ~(1<<6); //HV OFF
  curr_state &= ~(1<<EXT_CONTROL_HV_STATUS);
  curr_state &= ~(1<<EXT_CONTROL_PTT_STATUS);
  
  PORTJ &= ~(1<<5); //MAINS OFF
  curr_state &= ~(1<<EXT_CONTROL_MAINS_STATUS);
}

void ext_control_activate_band_relay(unsigned char band) {
  //Reset bit 2-7
  PORTC &= 0x03;
  
  if (band == BAND_160M)
    PORTC |= (1<<4);
  else if (band == BAND_80M)
    PORTC |= (1<<3);
  else if (band == BAND_40M)
    PORTC |= (1<<2);
  else if (band == BAND_20M)
    PORTC |= (1<<5);
  else if (band == BAND_15M)
    PORTC |= (1<<6);
  else if (band == BAND_10M)
    PORTC |= (1<<7);
  
  main_update_tune_sequence_status(TUNE_SEQUENCE_RELAY_DONE);
}

void ext_control_set_hv_on(void) {
  //Make sure that PTT is not pulled
  PORTG &= ~(1<<2); //PTT OFF
  curr_state &= ~(1<<EXT_CONTROL_PTT_STATUS);
  
  PORTJ |= (1<<6); //HV ON
  curr_state |= (1<<EXT_CONTROL_HV_STATUS);
}

void ext_control_set_hv_off(void) {
  //Make sure that PTT is not pulled
  PORTG &= ~(1<<2); //PTT OFF
  curr_state &= ~(1<<EXT_CONTROL_PTT_STATUS);
  
  PORTJ &= ~(1<<6); //HV OFF 
  curr_state &= ~(1<<EXT_CONTROL_HV_STATUS);
}

void ext_control_set_grid_relay_on(void) {
  PORTJ |= (1<<3);
}

void ext_control_set_grid_relay_off(void) {
  PORTJ &= ~(1<<3);
}

void ext_control_set_txrx_relay_on(void) {
  PORTJ |= (1<<0);
}

void ext_control_set_txrx_relay_off(void) {
  PORTJ &= ~(1<<0);
}

void ext_control_set_ptt_active(void) {
  event_add_message((void *)ext_control_set_txrx_relay_on, INTERVAL_PRE_TIME_TXRX, SEQUENCER_EVENT_TXRX_RELAY_ON);
  event_add_message((void *)ext_control_set_grid_relay_on, INTERVAL_PRE_TIME_GRID, SEQUENCER_EVENT_GRID_RELAY_ON);
  
  //Activate the PTT LED
  PORTE |= (1<<6);
  
  //Set the PTT status
  curr_state |= (1<<EXT_CONTROL_PTT_STATUS);
}

void ext_control_set_ptt_deactive(void) {
  if (event_queue_drop_id(SEQUENCER_EVENT_TXRX_RELAY_ON) == 0) {
    event_add_message((void *)ext_control_set_txrx_relay_off, INTERVAL_POST_TIME_TXRX, SEQUENCER_EVENT_TXRX_RELAY_OFF);
  }
  
  if (event_queue_drop_id(SEQUENCER_EVENT_GRID_RELAY_ON) == 0) {
    event_add_message((void *)ext_control_set_grid_relay_off, INTERVAL_POST_TIME_GRID, SEQUENCER_EVENT_GRID_RELAY_OFF);
  }

  //Deactivate the PTT LED
  PORTE &= ~(1<<6);
  
  //UNSet the PTT status
  curr_state &= ~(1<<EXT_CONTROL_PTT_STATUS);
}

/*!\brief Returns the position of the band selection switch 
 * \return 1 = 160m, 2 = 80m, 3 = 40m, 5 = 20m, 7 = 15m, 9 = 10m, 0 = undefined */
unsigned char ext_control_get_current_band_pos(void) {
  unsigned char val = BAND_UNDEFINED;
  
  if ((PINE & (1<<3)) == 0)
    val = BAND_10M;
  else if ((PINE & (1<<2)) == 0)
    val = BAND_15M;
  else if ((PINK & (1<<6)) == 0)
    val = BAND_20M;
  else if ((PINK & (1<<5)) == 0)
    val = BAND_40M;  
  else if ((PINK & (1<<4)) == 0)
    val = BAND_80M;  
  else if ((PINK & (1<<3)) == 0)
    val = BAND_160M;
  
  return(val);
}

unsigned int ext_control_poll_inputs(void) {
  unsigned int retval = 0;
  curr_input_state = 0;
  
  /* Loads the current switch positions into the curr_input_state buffer */
  if ((PINA & (1<<1)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_MODE_SW_BIT);
  if ((PINA & (1<<0)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_DISPLAY_UP_BIT);
  if ((PINK & (1<<0)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_DISPLAY_DOWN_BIT);
  if ((PINK & (1<<1)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SAVE_BIT);
  if ((PINK & (1<<2)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_RECALL_BIT);
  if ((PINA & (1<<7)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_TUNE_UP_BIT);
  if ((PINA & (1<<6)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_TUNE_DOWN_BIT);
  if ((PINA & (1<<5)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_LOAD_UP_BIT);
  if ((PINA & (1<<4)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_LOAD_DOWN_BIT);  
  if ((PINA & (1<<3)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SEGMENT_HIGH_BIT);
  if ((PINA & (1<<2)) == 0)
    curr_input_state |= (1<<EXT_CONTROL_SEGMENT_LOW_BIT);
    
  if (curr_input_state != prev_input_state) {
    retval = prev_input_state ^ curr_input_state;
    
    prev_input_state = curr_input_state;
  }
  
  return(retval);
}

unsigned char ext_control_get_ptt_input_state(void) {
  if (PING & (1<<5))
    return(0);
  
  return(1);
}

unsigned int ext_control_get_curr_state(void) {
  return(curr_input_state);
}

unsigned char ext_control_get_ptt_status(void) {
  return(curr_state & (1<<EXT_CONTROL_PTT_STATUS));
}

void ext_control_out_of_bounds_active(void) {
	PORTG |= (1<<2);
}

void ext_control_out_of_bounds_deactive(void) {
	PORTG &= ~(1<<2);
}