/*! \file computer_comm.c
 *  \brief The computer communication routines
 *  \ingroup computer_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-22
 *  \code #include "computer_comm.c" \endcode
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

#include "computer_comm.h"

#include "remote_ctrl.h"

#include "computer_comm_rx_queue.h"
#include "computer_comm_tx_queue.h"
#include "usart.h"

#include "../internal_comm_commands.h"

//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH 20

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

//! Flag if the RX data should be parsed
static unsigned char parse_rx_data = 0;
//! Flag if there is a message to be sent
static unsigned char send_tx_data = 0;
//! The current position in the RX buffer
static unsigned char rx_buffer_pos = 0;
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

//! The computer_comm struct
struct_computer_com computer_com;

static unsigned int comm_timer = 0;

//! Where we save any new computer_comm message
COMM_MESSAGE comm_new_message;

//! The previous data
static unsigned char comm_prev_data = 0;

//! Counter which keep track of when we last did a transmission
static unsigned char comm_counter_tx_timeout = 0;
//! Counter which keeps track of when we last did receive a character
static unsigned char comm_counter_rx_timeout = 0;

//! The number of times the last message has been resent
static unsigned char comm_resend_count = 0;

//! Flag that the message has yet not been acked
static unsigned char comm_msg_not_acked = 0;

/*! \brief Function to be called when a message is recieved and should be parsed/executed */
void (*func_ptr_rx)(COMM_MESSAGE);
/*! \brief Function to be called when we wish to send a message */
void (*func_ptr_tx)(char);

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
      internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR, rx_data_length,
                                   (char *)rx_data_buffer);
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
    
    if (argc > 0) {
      if ((strcmp(argv[0], "addr") == 0) && (argc == 2)) {
        char addr = atoi(argv[1]);
        printf("--- Setting remote address to %d\r\n", addr);
        internal_comm_add_tx_message(INT_COMM_PC_CONNECT_TO_ADDR, 1, &addr);
      }
      else if (strcmp(argv[0], "conv") == 0) {
        printf("--- Entering conversation mode.\r\n"
               "--- Use ESC to return to command mode\r\n");
        command_mode = 0;
        internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR, 0, NULL);
        return;
      }
      else if (strcmp(argv[0], "help") == 0) {
        printf("--- addr <card address> - Set remote card address\r\n");
        printf("--- conv                - Enter conversation mode\r\n");
        printf("--- rc                  - Set remote control\r\n");
      }
      else if (strcmp(argv[0], "arc") == 0) {
        char addr = 0;
        printf("--- ascii remote control mode\r\n");
        internal_comm_add_tx_message(INT_COMM_PC_CONNECT_TO_ADDR, 1, &addr);
      }
      else if (strcmp(argv[0], "remote") == 0) {  //Remote control, not real ASCII
        command_mode = 0;
        
        remote_ctrl_set_active();
        return;
      }
      else {
        printf("--- Unknown or malformed command: %s\r\n", argv[0]);
      }
    }
    
    printf("cmd: ");
  }
}

void computer_interface_tx_message(unsigned char length, char *data) {
  for (unsigned char i=0;i<length;i++)
    tx_data_buffer[i] = data[i];
  
  tx_data_buffer[length] = 0;
  tx_data_length = length;
  
  send_tx_data = 1;
}

/*! \brief Initialize the internal communication 
 *  \param func_ptr_rx The function you wish to call when a new message has been recieved and should be parsed
 *  \param func_ptr_tx The function used to send data to the hardware handeling the data transmission */
void computer_comm_init(void (*func_ptr_rx)(COMM_MESSAGE), void (*func_ptr_tx)(char)) {
	func_ptr_rx = func_ptr_rx;	//To know which function we should call to parse a message
	func_ptr_tx = func_ptr_tx;	//To know which function we should call to send data
	
	computer_com.char_count = 0;
	computer_com.checksum = 0;
	computer_com.flags = 0;
	
	computer_comm_rx_queue_init();
	computer_comm_tx_queue_init();
	
	computer_comm_rx_queue_dropall();
	computer_comm_tx_queue_dropall();
}

/*! \brief Will reset the RX variables */
void __inline__ computer_comm_reset_rx(void) {
	computer_com.char_count = 0;
	computer_com.checksum = 0;
	computer_com.flags = 0;
	comm_prev_data = 0;
}

/*! \brief Polls the RX queue in the internal communication and calls the function defined in computer_comm_init.
 * \return 1 if a message was in the buffer and got parsed, 0 if not
 */
unsigned char computer_comm_poll_rx_queue(void) {
	if (computer_comm_rx_queue_count() > 0) {
		func_ptr_rx(computer_comm_rx_queue_get());	//Send the first message to the function
		
		computer_comm_rx_queue_drop();					//Drop the message from the queue since it's been executed
		return(1);								//Return 1 to show that something was done
	}
	
	return(0);
}

/*! \brief Polls the TX queue in the internal communication and sends the data if there is a message in the queue
 *  \return 1 if a message was in the buffer and got sent, 0 if not
 */
unsigned char computer_comm_poll_tx_queue(void) {
  if (computer_com.flags & (1<<COMM_MESSAGE_RESEND)) {
    computer_comm_resend(); 
    
    computer_com.flags &= ~(1<<COMM_MESSAGE_RESEND);
  }
  
  if ((computer_comm_tx_queue_size() > 0) && (comm_msg_not_acked == 0)) {
		//Send the first message in the queue
    #ifdef COMPUTER_COMM_DEBUG
      printf("PSEND\n\r");
    #endif 
    
		computer_comm_send_message(computer_comm_tx_queue_get());
		comm_msg_not_acked = 1;
		return(1);	//Return 1 to show that something was done
	}
	
	return(0);
}

/*! \brief Send an ACK message to the internal communication uart */
void computer_comm_send_ack(void) {
  #ifdef COMPUTER_COMM_DEBUG
    printf("SA\n\r");
  #endif
  
	unsigned char checksum = COMPUTER_COMM_MSG_ACK;
	
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(checksum);
	func_ptr_tx(COMPUTER_COMM_MSG_ACK);
	func_ptr_tx(0); 
	func_ptr_tx(COMPUTER_COMM_MSG_POSTAMBLE);
	
	comm_counter_tx_timeout = 0;
}

/*! \brief Send a NACK message to the internal communication uart */
void computer_comm_send_nack(void) {
	unsigned char checksum = COMPUTER_COMM_MSG_NACK;
	
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(checksum);
	func_ptr_tx(COMPUTER_COMM_MSG_NACK);
	func_ptr_tx(0);
	func_ptr_tx(COMPUTER_COMM_MSG_POSTAMBLE);
	
	comm_counter_tx_timeout = 0;
}

/*! \brief Send a message to the internal communication uart 
 *  \param tx_message The message we wish to send */
void computer_comm_send_message(COMM_MESSAGE tx_message) {
	#ifdef COMPUTER_COMM_DEBUG
		printf("S: 0x%02X\n",tx_message.cmd);
	#endif
	
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(COMPUTER_COMM_MSG_PREAMBLE);
	func_ptr_tx(tx_message.checksum);
	func_ptr_tx(tx_message.cmd);
	func_ptr_tx(tx_message.length);
	
	for (unsigned char i=0;i<tx_message.length;i++)
		func_ptr_tx(tx_message.data[i]);
	
  func_ptr_tx(COMPUTER_COMM_MSG_POSTAMBLE);
	
	comm_counter_tx_timeout = 0;
}

/*! \brief Add a message to the transmit queue 
 *  \param command The command we wish to perform
 *  \param length The length of the data field 
 *  \param data The data we wish to send */
void computer_comm_add_tx_message(unsigned char command, unsigned char length, char *data) {
	#ifdef COMPUTER_COMM_DEBUG
		printf("ADD 0x%02X\n",command);
    printf("TX_BUF_SIZE: %i\n\r",computer_comm_tx_queue_size());
    printf("RX_BUF_SIZE: %i\n\r",computer_comm_rx_queue_count());
	#endif
	
	if (computer_comm_tx_queue_size() < COMPUTER_COMM_TX_QUEUE_SIZE) {
	  COMM_MESSAGE new_mess;
    
    new_mess.checksum = command + length;
    
    new_mess.cmd = command;
    new_mess.length = length;
    
    for (unsigned char i=0;i<length;i++) {
      new_mess.checksum += data[i];
      new_mess.data[i] = data[i];
      
      #ifdef COMPUTER_COMM_DEBUG
        printf("0x%02X ",new_mess.data[i]);
      #endif
    }
    
    #ifdef COMPUTER_COMM_DEBUG
      printf("\n");
    #endif

    computer_comm_tx_queue_add(new_mess);
  }
}

void __inline__ computer_comm_message_acked(void) {
	comm_new_message.checksum = 0;
	comm_new_message.cmd = 0;
	comm_new_message.length = 0;
				
	comm_msg_not_acked = 0;
	comm_counter_tx_timeout = 0;
	
	//Last TXed message was acked, lets drop the message from the TX queue
	computer_comm_tx_queue_drop();
  
  #ifdef COMPUTER_COMM_DEBUG
    printf("ACKED: %i\n\r",computer_comm_tx_queue_size());
  #endif
  
	comm_resend_count = 0;
  
  computer_comm_reset_rx();
}

void __inline__ computer_comm_message_nacked(void) {
	computer_comm_resend();
}

/*! \brief Will trigger a resend of the last message */
void computer_comm_resend(void) {
  if (comm_resend_count < COMPUTER_COMM_RESEND_COUNT) {
		if (comm_msg_not_acked == 1) {
			computer_comm_send_message(computer_comm_tx_queue_get());

			#ifdef COMPUTER_COMM_DEBUG
				printf("RS[%i]: %u\n",comm_resend_count,comm_timer);
			#endif
			
			comm_resend_count++;
		}
	}
	else {
		#ifdef COMPUTER_COMM_DEBUG
			printf("RESEND ERROR\n");
      
		#endif
			
		computer_comm_tx_queue_drop();
		comm_counter_tx_timeout = 0;
		computer_comm_reset_rx();
		comm_resend_count = 0;
		comm_msg_not_acked = 0;
	}
}

//! Interrupt when a byte has been received from the UART
ISR(ISR_COMPUTER_COMM_USART_RECV) {
	unsigned char data = COMPUTER_COMM_UDR;

  computer_com.char_count++;
	comm_counter_rx_timeout = 0;

  //Check if we are in remote control mode, then we handle the data differently
  if (remote_ctrl_get_active_status() == 0) {
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
  else {
    if (computer_com.flags && (1<<COMM_PREAMBLE_FOUND)) {
      //Check if this is a postamble
      if ((computer_com.char_count >= 4) && (data == COMPUTER_COMM_MSG_POSTAMBLE)) {
        if (comm_new_message.length == (computer_com.char_count-4)) {
          if (comm_new_message.checksum == computer_com.checksum) {
            #ifdef COMPUTER_COMM_DEBUG
              printf("AM 0x%02X\n",comm_new_message.cmd);
            #endif
            
            //If there is room in the queue the message gets added and acked, otherwise not
            if (computer_comm_rx_queue_count() < COMPUTER_COMM_RX_QUEUE_SIZE) {
              if ((comm_new_message.cmd != COMPUTER_COMM_MSG_ACK) && (comm_new_message.cmd != COMPUTER_COMM_MSG_NACK)) {
                computer_comm_send_ack();
              
                //Add the message to the RX queue
                computer_comm_rx_queue_add(comm_new_message);
                //Send ack to acknowledge we recieved the package
              }
              else {
                if (comm_new_message.cmd == COMPUTER_COMM_MSG_ACK)
                  computer_comm_message_acked();
                else if (comm_new_message.cmd == COMPUTER_COMM_MSG_ACK)
                  computer_comm_message_nacked();
              }
            }
          }
          else {
            computer_comm_send_nack();
          }

          computer_com.flags &= ~(1<<COMM_PREAMBLE_FOUND);
          computer_com.char_count = 0;
          comm_prev_data = 0;
        }
        else {
          if ((computer_com.char_count - 4) < COMM_MESSAGE_DATA_SIZE) {
            comm_new_message.data[computer_com.char_count - 4] = data;
            computer_com.checksum += data;
          }
        }
      }
      else {
        switch(computer_com.char_count) {
          case 1 :
            comm_new_message.checksum = data;
            break;
          case 2 :
            comm_new_message.cmd = data;
            computer_com.checksum = data;
            break;        
          case 3 :  
            comm_new_message.length = data;
            computer_com.checksum += data;
            break;
          default:  
            if ((computer_com.char_count - 4) < COMM_MESSAGE_DATA_SIZE) {
                comm_new_message.data[computer_com.char_count - 4] = data;
                computer_com.checksum += data;
              }
          break;
        }
      }
    }
    else if (data == COMPUTER_COMM_MSG_PREAMBLE) {
      //Is this a preamble or just a variable sent?
      if (comm_prev_data == COMPUTER_COMM_MSG_PREAMBLE) {
        computer_com.flags |= (1 << COMM_PREAMBLE_FOUND);
        computer_com.char_count = 0;
        computer_com.checksum = 0;
      }
    }

  }
  
	
	comm_prev_data = data;
}

//! Interrupt when data has been received from the UART
ISR(ISR_COMPUTER_COMM_USART_DATA) {
	
}

/*! \brief Function which should be called each ms */
void computer_comm_1ms_timer(void) {
	if ((computer_com.char_count > 0) && (comm_counter_rx_timeout >= COMPUTER_COMM_RX_TIMEOUT)) {
		comm_counter_rx_timeout = 0;
    
		computer_comm_reset_rx();
	}
		
	comm_timer++;
		
	if ((comm_msg_not_acked == 1) && (comm_counter_tx_timeout >= COMPUTER_COMM_TX_TIMEOUT)) {
		comm_counter_tx_timeout = 0;
		
		computer_com.flags |= (1<<COMM_MESSAGE_RESEND);
	}
	
	if (comm_msg_not_acked != 0)
		comm_counter_tx_timeout++;
	
	if (computer_com.char_count != 0)
		comm_counter_rx_timeout++;
}