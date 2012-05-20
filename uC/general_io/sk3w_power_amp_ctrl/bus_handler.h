#ifndef _BUS_HANDLER_H_
#define _BUS_HANDLER_H_


void bus_handler_init(void);
void bus_handler_tick(void);
void bus_handler_poll_core(void);
void bus_handler_poll(void);

void send_ascii_data(unsigned char to_addr, const char *fmt, ...);

#endif
