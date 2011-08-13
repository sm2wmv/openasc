/*! \file remote_ctrl_board/computer_interface.c
 *  \brief Remote control board for the station
 *  \ingroup remote_ctrl_board_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-04-16
 *  \code #include "remote_ctrl_board/main.c" \endcode
 */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <avr/wdt.h>

#include "main.h"
#include "board.h"
#include "usart.h"
#include "computer_interface.h"

#include "../delay.h"
#include "../global.h"
#include "../event_queue.h"
//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH 15

//! The length of the computer TX BUFFER
#define COMPUTER_TX_BUFFER_LENGTH 25

//! The maximum number of command arguments, including command name
#define MAX_CMD_ARGS 5

//! Character for enter
#define CHAR_ENTER 13
//! Character for space
#define CHAR_SPACE 32
//! Character for ESC
#define CHAR_ESC 27
//! Character for backspace
#define CHAR_BS 8

//! Counter with 1 ms ticks
static unsigned int counter_1ms = 0;
//! Flag if the RX data should be parsed
static unsigned char parse_rx_data = 0;
//! Flag if there is a message to be sent
static unsigned char send_tx_data = 0;
//! The current position in the RX buffer
static unsigned char rx_buffer_pos = 0;
//! The current position in the TX buffer
static unsigned char tx_buffer_pos = 0;
//! The length of the current tx message
static unsigned char tx_data_length = 0;
//! The length of the current rx message
static unsigned char rx_data_length = 0;
//! Indicate if we are in command mode or transparent communications mode
static unsigned char command_mode = 1;

//! The rx buffer of length COMPUTER_RX_BUFFER_LENGTH
static unsigned char rx_data_buffer[COMPUTER_RX_BUFFER_LENGTH+1];
//! The tx buffer of length COMPUTER_TX_BUFFER_LENGTH
static unsigned char tx_data_buffer[COMPUTER_TX_BUFFER_LENGTH];


void computer_interface_send_data(void) {
  if (send_tx_data) {
    send_tx_data = 0;
     
    for (unsigned char i=0;i<tx_data_length;i++)
      usart1_transmit(tx_data_buffer[i]);
    
    tx_data_length = 0;
  }
}

void computer_interface_parse_data(void) {
  if (parse_rx_data) {
    parse_rx_data = 0;
    rx_buffer_pos = 0;
    
    if (!command_mode)
    {
//      internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR, rx_data_length,(char *)rx_data_buffer);
      return;
    }

      // Get the command and its arguments
    unsigned char argc = 0;
    char *argv[MAX_CMD_ARGS];
    argv[0] = NULL;
    unsigned char pos = 0;
    for (pos=0; pos < rx_data_length; ++pos) {
      if (argv[argc] == NULL) {
        if (rx_data_buffer[pos] != CHAR_SPACE) {
          argv[argc] = (char*)(rx_data_buffer + pos);
        }
      }
      else {
        if (rx_data_buffer[pos] == CHAR_SPACE) {
          rx_data_buffer[pos] = '\0';
          if (argc >= MAX_CMD_ARGS-1) {
            break;
          }
          ++argc;
          argv[argc] = NULL;
        }
      }
    }
    if (argv[argc] != NULL) {
      ++argc;
    }
    
    printf("\r\n");
    
    if (argc > 0)
    {
      if (strcmp(argv[0], "help") == 0) {
        printf("ver -- Firmware version\r\n");
      }
      else if (strncmp(argv[0], "ver",3) == 0) {
        printf("Firmware version: %s\r\n",FIRMWARE_REV);
      }
      else {
        printf("--- Unknown or malformed command: %s\r\n", argv[0]);
      }
    }
    
    printf("cmd: ");
  }
}

void computer_interface_init(void) {
  parse_rx_data = 0;
  send_tx_data = 0;
}

void computer_interface_1ms_tick(void) {
  counter_1ms++;
}

ISR(SIG_USART0_DATA) {

}

/*! Interrupt which is called when a byte has been received */
ISR(SIG_USART0_RECV) {
  unsigned char data = UDR0;

  if (data == CHAR_ENTER) {
    rx_data_length = rx_buffer_pos;
    rx_data_buffer[rx_buffer_pos] = 0;
    parse_rx_data = 1;
  }
  else if (data == CHAR_ESC) {
    command_mode = 1;
    rx_data_length = 0;
    rx_buffer_pos = 0;
    parse_rx_data = 1;
  }
  else if (data == CHAR_BS) {
    if (rx_buffer_pos > 0)
    {
      usart1_transmit(CHAR_BS);
      usart1_transmit(CHAR_SPACE);
      usart1_transmit(CHAR_BS);
      --rx_buffer_pos;
    }
  }
  else {
    if (rx_buffer_pos < COMPUTER_RX_BUFFER_LENGTH) {
      usart1_transmit(data);
      rx_data_buffer[rx_buffer_pos++] = data;
    }
  }
}
