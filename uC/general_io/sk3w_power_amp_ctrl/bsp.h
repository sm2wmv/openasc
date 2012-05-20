#ifndef _BSP_H_
#define _BSP_H_

#include <avr/io.h>


void bsp_init_timer_2(void);

uint8_t bsp_get_tx_active(void);
void bsp_set_pa_ptt(unsigned char band, int on);
void bsp_set_pa_mains(unsigned char band, int on);
void bsp_set_pa_reset(unsigned char band, int on);

uint8_t bsp_mains_is_on(unsigned char band);

#endif
