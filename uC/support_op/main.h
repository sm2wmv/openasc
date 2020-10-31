#ifndef _MAIN_H_
#define _MAIN_H_

//#define DEBUG


#define PRIORITY_FIRST_WINS   0
#define PRIORITY_R1_WINS      1
#define PRIORITY_R2_WINS      2

#define PTT_DEACTIVE  0
#define PTT_ACTIVE    1

#define NR_OF_EVENTS  8


//Also input table offsets
#define EVENT_FOOTSWICH_INBAND_STATE          0
#define EVENT_FOOTSWICH_RUN_STATE             1
#define EVENT_INBAND_RX_ANT_R1_SELET          2
#define EVENT_INBAND_RX_ANT_SELECT	          3
#define EVENT_RUN_RX_ANT_SELECT  	            4
#define EVENT_INBAND_TX_SELECT_STATE          5
#define EVENT_PRIORITY_STATE                  6
#define EVENT_ASC_INPUT_STATE                 7

//Output table offsets
#define OFFSET_PTT_OPENASC_RUN    0
#define OFFSET_PTT_INBAND_AMP     1
#define OFFSET_PTT_RUN_AMP        2
#define OFFSET_PTT_INBAND_RADIO   3
#define OFFSET_PTT_RUN_RADIO      4
#define OFFSET_RELAY_TRANSFER     5
#define OFFSET_RELAY_K4K12        6
#define OFFSET_RELAY_K11          7
#define OFFSET_RELAY_K10          8
#define OFFSET_RELAY_K9           9
#define OFFSET_RELAY_K8           10
#define OFFSET_RELAY_K5K6         11
#define OFFSET_RELAY_K3           12

#define KEYBOUNCE_TIME 10

/*! START EVENT QUEUE IDs */
#define EVENT_QUEUE_ID_TRANSFER_RELAY     125

//! Limit of an high priority event, if the value is above this it is counted as high priority
#define EVENT_HIGH_PRIORITY_LIMIT 100

#define EVENT_CHECK_INPUTS  120
#define EVENT_CHECK_OPENASC_PTT 121
#define SEQUENCER_EVENT_TYPE_PTT_RUN_AMP_ON    		101
#define SEQUENCER_EVENT_TYPE_PTT_RUN_ON        		102
#define SEQUENCER_EVENT_TYPE_PTT_INBAND_AMP_ON    103
#define SEQUENCER_EVENT_TYPE_PTT_INBAND_ON        104

#define SEQUENCER_EVENT_TYPE_PTT_RUN_AMP_OFF   		105
#define SEQUENCER_EVENT_TYPE_PTT_RUN_OFF       		106
#define SEQUENCER_EVENT_TYPE_PTT_INBAND_AMP_OFF   107
#define SEQUENCER_EVENT_TYPE_PTT_INBAND_OFF       108


/*! END EVENT QUEUE IDs */


//! Time between each poll of the inputs (in ms)
#define COUNTER_POLL_INPUT_LIMIT 2

//! The pulse time of the transfer relay in ms
#define TRANSFER_RELAY_PULSE_TIME 25

//! The between an event has occured and until we check the state of the openASC PTT_ACTIVE
#define OPENASC_PTT_CHECK_TIME  20

#ifdef DEBUG
#define PRINTF printf
#else 
#define PRINTF(...)
#endif

void main_execute_statemachine(void);
void event_add_message(void (*func), unsigned int offset, unsigned char id);
uint8_t main_check_ptt_active_run();
uint8_t main_check_ptt_active_inband();

void __inline__ r1_ptt_amp_on(void);

#endif
