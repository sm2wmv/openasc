#ifndef _EXT_CONTROL_H_
#define _EXT_CONTROL_H_

#include "../../../global.h"

#define EXT_CONTROL_MODE_SW_BIT       0
#define EXT_CONTROL_DISPLAY_UP_BIT    1
#define EXT_CONTROL_DISPLAY_DOWN_BIT  2
#define EXT_CONTROL_TUNE_UP_BIT       3
#define EXT_CONTROL_TUNE_DOWN_BIT     4
#define EXT_CONTROL_LOAD_UP_BIT       5
#define EXT_CONTROL_LOAD_DOWN_BIT     6
#define EXT_CONTROL_SAVE_BIT          7
#define EXT_CONTROL_RECALL_BIT        8
#define EXT_CONTROL_SEGMENT_HIGH_BIT  9
#define EXT_CONTROL_SEGMENT_LOW_BIT   10
#define EXT_CONTROL_BAND_CHANGE_BIT   11

#define SEQUENCER_EVENT_GRID_RELAY_ON   1
#define SEQUENCER_EVENT_GRID_RELAY_OFF  2
#define SEQUENCER_EVENT_TXRX_RELAY_ON   3
#define SEQUENCER_EVENT_TXRX_RELAY_OFF  4

//! The time from when the PTT input went low until we pull the bias relay
#define INTERVAL_PRE_TIME_GRID  20
//! The time from when the PTT input went low until we pull the TX/RX relay
#define INTERVAL_PRE_TIME_TXRX  0

//! The time from when the PTT input went high until we release the bias relay
#define INTERVAL_POST_TIME_GRID  0
//! The time from when the PTT input went high until we release the TX/RX relay
#define INTERVAL_POST_TIME_TXRX  10

void ext_control_init(void);
unsigned char ext_control_get_current_band_pos(void);
unsigned int ext_control_poll_inputs(void);
unsigned int ext_control_get_curr_state(void);

void ext_control_set_mains_on(void);
void ext_control_set_mains_off(void);

void ext_control_activate_band_relay(unsigned char band);

void ext_control_set_hv_on(void);
void ext_control_set_hv_off(void);

void ext_control_set_ptt_active(void);
void ext_control_set_ptt_deactive(void);

unsigned char ext_control_get_ptt_input_state(void);
unsigned char ext_control_get_ptt_status(void);

void ext_control_out_of_bounds_active(void);
void ext_control_out_of_bounds_deactive(void);

#endif