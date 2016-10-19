#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "xbee_interface.h"

static uint16_t rx_buffer_pos = 0;

static uint8_t xbee_preamble_found = 0;

uint8_t frame_cnt = 0;

struct_xbee_api_frame new_frame;

void xbee_interface_reset_rx(void);

volatile static uint8_t last_rxed_char = 0;

/*! \brief Function to be called when we wish to send a character */
static void (*f_ptr_tx)(char);
/*! \brief Function to call when a frame has been RXed which should be parsed */
static void (*f_ptr_parse)(struct_xbee_api_frame);

void xbee_interface_init(void (*func_ptr_tx)(char), void (*func_ptr_parse)(struct_xbee_api_frame)) {
  f_ptr_tx = func_ptr_tx;
  f_ptr_parse = func_ptr_parse;

  xbee_interface_reset_rx();
}

void xbee_interface_reset_rx(void) {
  xbee_preamble_found = 0;
  rx_buffer_pos = 0;
}

void xbee_interface_transmit_frame(uint8_t frame_type, uint8_t dest_addr[8], uint8_t *rf_data, uint8_t data_len) {
  uint16_t len = 14 + data_len;
  uint8_t checksum;


  f_ptr_tx(0x7E);
  f_ptr_tx(0x00); //Currently we never send long messages
  f_ptr_tx(len);
  f_ptr_tx(frame_type);
  checksum = frame_type;
  f_ptr_tx(frame_cnt);
  checksum += frame_cnt;
  frame_cnt++;

  //Set the destination address
  for (uint8_t i=0;i<8;i++) {
    f_ptr_tx(dest_addr[i]);
    checksum += dest_addr[i];
  }

  f_ptr_tx(0xFF);
  checksum += 0xFF;
  f_ptr_tx(0xFE);
  checksum += 0xFE;

  //Broadcast radius
  f_ptr_tx(0x00);
  checksum += 0x00;

  //Transmit options
  f_ptr_tx(0x00);
  checksum += 0x00;

  for (uint8_t i=0;i<data_len;i++) {
    f_ptr_tx(rf_data[i]);
    checksum += rf_data[i];
  }

  //Send the checksum
  f_ptr_tx(0xFF-checksum);
}

void xbee_interface_transmit_remote_at_cmd(uint8_t dest_addr[8], uint8_t at_cmd0, uint8_t at_cmd1, uint8_t parameter) {
  uint16_t len = 16;
  uint8_t checksum;

  f_ptr_tx(0x7E);
  f_ptr_tx(0x00); //Currently we never send long messages
  f_ptr_tx(len);
  f_ptr_tx(0x17); //Remote AT command
  checksum = 0x17;
  f_ptr_tx(01);
  checksum += 01;//frame_cnt;
  frame_cnt++;

  //Set the destination address
  for (uint8_t i=0;i<8;i++) {
    f_ptr_tx(dest_addr[i]);
    checksum += dest_addr[i];
  }

  f_ptr_tx(0xFF);
  checksum += 0xFF;
  f_ptr_tx(0xFE);
  checksum += 0xFE;

  f_ptr_tx(0x02);
  checksum += 0x02;

  f_ptr_tx(at_cmd0);
  checksum += at_cmd0;
  
  f_ptr_tx(at_cmd1);
  checksum += at_cmd1;
  
  f_ptr_tx(parameter);
  checksum += parameter;
  
  //Send the checksum
  f_ptr_tx(0xFF-checksum);
}

void xbee_interface_rx_char(uint8_t data) {
  last_rxed_char = 0;
  rx_buffer_pos++;

  if (xbee_preamble_found == 1) {
    if (rx_buffer_pos == 1) {
      new_frame.length = (data << 8);
    }
    else if (rx_buffer_pos == 2) {
      new_frame.length |= data;
    }
    else if (rx_buffer_pos == 3) {
      new_frame.frame_type = data;
      new_frame.checksum = data;
    }
    else if ((rx_buffer_pos-3) == new_frame.length) {
      PRINTF_XBEE("XBEE >> CHECKSUM FOUND: 0x%02X\n", data);

      if (((new_frame.checksum + data) & 0xFF) == 0xFF) {
        PRINTF_XBEE("XBEE >> CHECKSUM OK\n");
        f_ptr_parse(new_frame);
      }
      else {
        PRINTF_XBEE("XBEE >> CHECKSUM FAILED\n");
      }

      xbee_preamble_found = 0;


      PRINTF_XBEE("XBEE >> FRAME\n");
      PRINTF_XBEE("XBEE >> -----\n");
      PRINTF_XBEE("XBEE >> Length: %i\n",new_frame.length);
      PRINTF_XBEE("XBEE >> Type: 0x%02X\n", new_frame.frame_type);

      for (uint8_t i=0;i<new_frame.length-1;i++)
        PRINTF_XBEE("XBEE >> DATA[%i]: 0x%02X\n", i, new_frame.frame_data[i]);

      PRINTF_XBEE("XBEE >> CHECKSUM: 0x%02X\n", new_frame.checksum);
    }
    else if (rx_buffer_pos < XBEE_RX_BUFFER_SIZE) {
      new_frame.frame_data[rx_buffer_pos-4] = data;
      new_frame.checksum += data;
    }
    else {
      xbee_interface_reset_rx();
    }
  }
  else if (data == 0x7E) {
    xbee_preamble_found = 1;
    rx_buffer_pos = 0;
    new_frame.checksum = 0;
  }
}

void xbee_interface_ms_tick(void) {
  last_rxed_char++;

  if (last_rxed_char > XBEE_RX_CHAR_TIMEOUT) {
    xbee_interface_reset_rx();
    last_rxed_char = 0;
  }
}
