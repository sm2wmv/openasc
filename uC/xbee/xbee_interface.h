#ifndef _XBEE_INTERFACE_H_
#define _XBEE_INTERFACE_H_

#define XBEE_RX_BUFFER_SIZE 256

//Timeout, if no character was received within this period then
//the receive buffer is cleared. Time is in ms.
#define XBEE_RX_CHAR_TIMEOUT  100

#define XBEE_MAX_FRAME_DATA_SIZE  50

//Received package
#define XBEE_API_FRAME_RX_PACKAGE 0x90

#define PRINTF_XBEE(...)



typedef struct {
  uint16_t length;
  uint8_t frame_type;
  uint8_t frame_data[XBEE_RX_BUFFER_SIZE-6];
  uint8_t checksum;
} struct_xbee_api_frame;

void xbee_interface_init(void (*func_ptr_tx)(char), void (*func_ptr_parse)(struct_xbee_api_frame));
void xbee_interface_rx_char(uint8_t data);
void xbee_interface_ms_tick(void);

void xbee_interface_transmit_frame(uint8_t frame_type, uint8_t dest_addr[8], uint8_t *rf_data, uint8_t data_len);
void xbee_interface_transmit_remote_at_cmd(uint8_t dest_addr[8], uint8_t at_cmd0, uint8_t at_cmd1, uint8_t parameter);

#endif
