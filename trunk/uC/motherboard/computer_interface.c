/*! \file motherboard/computer_interface.c 
 *  \ingroup motherboard_group
 *  \brief Interface towards the computer
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "computer_interface.c" \endcode */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

//Debug flag for the computer communication protocol
//#define COM_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "computer_interface.h"
#include "usart.h"
#include "main.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH	128

//! The length of the computer RX BUFFER
#define COMPUTER_TX_BUFFER_LENGTH	25

//! Character for enter
#define CHAR_ENTER 13
//! Character for space
#define CHAR_SPACE 32

//! Counter with 1 ms ticks
unsigned int counter_1ms = 0;
//! Flag if the RX data should be parsed
unsigned char parse_rx_data = 0;
//! Flag if there is a message to be sent
unsigned char send_tx_data = 0;
//! The current position in the RX buffer
unsigned char rx_buffer_pos = 0;
//! The current position in the TX buffer
unsigned char tx_buffer_pos = 0;
//! The length of the current tx message
unsigned char tx_data_length = 0;
//! The length of the current rx message
unsigned char rx_data_length = 0;

//! The rx buffer of length COMPUTER_RX_BUFFER_LENGTH
unsigned char rx_data_buffer[COMPUTER_RX_BUFFER_LENGTH];
//! The tx buffer of length COMPUTER_TX_BUFFER_LENGTH
unsigned char tx_data_buffer[COMPUTER_TX_BUFFER_LENGTH];

void computer_interface_send_data(void) {
  if (send_tx_data) {
    send_tx_data = 0;
     
    for (unsigned char i=0;i<tx_data_length;i++)
      usart1_transmit(tx_data_buffer[i]);
    
    tx_data_length = 0;
  }
}

void computer_interface_parse_data(void) {
  //! REMEMBER THAT NO MESSAGES CAN NOT BE OVER 15 CHARACTERS IN LENGTH!
  
  if (parse_rx_data) {
    parse_rx_data = 0;
    rx_buffer_pos = 0;
    
    unsigned char pos = 0;
    unsigned char cmd_len = 0;
    char cmd[5];
    
    if (rx_data_length == 0) {
      printf("\n\rcmd: ");
      
      return; 
    }

    //Get the command
    while(pos < rx_data_length) {
      if (rx_data_buffer[pos] != CHAR_SPACE) {
        cmd[pos] = rx_data_buffer[pos];
      }
      else
        break;
      
      pos++;
    }
       
    cmd[pos] = 0;
    
    printf("cmd: %s\n\r",cmd);
    
    if (strcmp(cmd,"c") == 0) {
      //Fix so that it parses the right addr etc
      char temp = 1;
      
      internal_comm_add_tx_message(INT_COMM_PC_CONNECT_TO_ADDR, 1, &temp);
    }
    else if (strcmp(cmd,"t") == 0) {
      //Just testing to send data
      internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR,6,"HEJSAN");
    }
    else {
      printf("?\n\r");
    }
  }
}

void computer_interface_tx_message(unsigned char length, char *data) {
  for (unsigned char i=0;i<length;i++)
    tx_data_buffer[i] = data[i];
  
  tx_data_buffer[length] = 0;
  tx_data_length = length;
  
  send_tx_data = 1;
}

void computer_interface_init(void) {
  parse_rx_data = 0;
  send_tx_data = 0;
}

void computer_interface_1ms_tick(void) {
  counter_1ms++;
}

ISR(SIG_USART1_DATA) {

}

/*! Interrupt which is called when a byte has been received */
ISR(SIG_USART1_RECV) {
	unsigned char data = UDR1;

  usart1_transmit(data);
  
  if (data == CHAR_ENTER) {
    rx_data_length = rx_buffer_pos;
    rx_data_buffer[rx_buffer_pos] = 0;
    parse_rx_data = 1;
  }
  else {  
    if (rx_buffer_pos <= (COMPUTER_RX_BUFFER_LENGTH-1))
      rx_data_buffer[rx_buffer_pos++] = data;
    else {
      rx_buffer_pos = 0;
      rx_data_buffer[rx_buffer_pos++] = data;
    }
  }
}
