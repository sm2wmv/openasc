#ifndef _MAIN_H_
#define _MAIN_H_

enum led_state {
  LED_ON,
  LED_OFF
};

enum relay_state {
  RELAY_ON,
  RELAY_OFF
};

enum ptt_state {
  PTT_ACTIVE,
  PTT_DEACTIVE
};

enum rx_tx_ant_state {
  TX_ANT,
  RX_ANT
};

enum mult_support_state {
  MULT_STATE,
  SUPPORT_STATE
};

enum priority_state {
  FIRST_WINS,
  RUN_WINS,
  MULT_WINS
};

void event_add_message(void (*func), unsigned int offset, unsigned char id);

void __inline__ r1_ptt_amp_on(void);

#endif
