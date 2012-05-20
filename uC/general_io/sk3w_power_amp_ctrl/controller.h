#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_


typedef struct {
  uint8_t addr;
  uint8_t band;
} Controller;


void controller_init(void);

int8_t controller_set_band(uint8_t ctrlr, uint8_t fm_addr, uint8_t band);

uint8_t controller_band(uint8_t cntrlr);

uint8_t controller_band_list(const Controller **band_list);

int8_t controller_toggle_mains(uint8_t ctrlr);

void controller_set_tx_active(uint8_t ctrlr, int8_t is_active);


#endif /* _CONTROLLER_H_ */
