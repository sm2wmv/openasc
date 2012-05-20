#ifndef _PA_H_
#define _PA_H_

#define PA_CTRLR_UNUSED      255

void pa_init(void);
void pa_set_controller(uint8_t band, uint8_t ctrlr);
void pa_clear_controller(uint8_t band);
uint8_t pa_controller(uint8_t band);
int8_t pa_toggle_mains(uint8_t band);
void pa_set_tx_active(uint8_t band, int8_t on);
uint8_t pa_op_status(uint8_t band);

extern void pa_op_status_changed(uint8_t band);
extern void pa_ctrlr_changed(uint8_t band);

#endif /* _PA_H_ */
