/*! \file internal_comm.c
 *  \brief The internal communication routines
 *  \ingroup internal_comm_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm.c" \endcode
 */
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "internal_comm.h"

#include "internal_comm_rx_queue.h"
#include "internal_comm_tx_queue.h"

#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#include "../front_panel/led_control.h"
	#include "../front_panel/errors.h"
	#include "../front_panel/main.h"
#endif

//! The uc_com struct
struct_uc_com uc_com;

unsigned int timer = 0;

//! Where we save any new uc_comm message
UC_MESSAGE uc_new_message;

//! The previous data
unsigned char prev_data = 0;

unsigned char allowed_to_send = 1;

//! Counter which keep track of when we last did a transmission
unsigned char counter_tx_timeout = 0;
//! Counter which keeps track of when we last did receive a character
unsigned char counter_rx_timeout = 0;

//! The number of times the last message has been resent
unsigned char resend_count = 0;

//! Flag that the message has yet not been acked
unsigned char msg_not_acked = 0;

/*! \brief Function to be called when a message is recieved and should be parsed/executed */
void (*f_ptr_rx)(UC_MESSAGE);
/*! \brief Function to be called when we wish to send a message */
void (*f_ptr_tx)(char);

/*! \brief Initialize the internal communication 
 *  \param func_ptr_rx The function you wish to call when a new message has been recieved and should be parsed
 *  \param func_ptr_tx The function used to send data to the hardware handeling the data transmission */
void internal_comm_init(void (*func_ptr_rx)(UC_MESSAGE), void (*func_ptr_tx)(char)) {
	f_ptr_rx = func_ptr_rx;	//To know which function we should call to parse a message
	f_ptr_tx = func_ptr_tx;	//To know which function we should call to send data
	
	uc_com.char_count = 0;
	uc_com.checksum = 0;
	uc_com.flags = 0;
	
	int_comm_rx_queue_init();
	int_comm_tx_queue_init();
	
	int_comm_rx_queue_dropall();
	int_comm_tx_queue_dropall();
}

/*! \brief Will reset the RX variables */
void __inline__ internal_comm_reset_rx(void) {
	uc_com.char_count = 0;
	uc_com.checksum = 0;
	uc_com.flags = 0;
	prev_data = 0;
}

/*! \brief Polls the RX queue in the internal communication and calls the function defined in internal_comm_init.
 * \return 1 if a message was in the buffer and got parsed, 0 if not
 */
unsigned char internal_comm_poll_rx_queue(void) {
	if (int_comm_rx_queue_count() > 0) {
		f_ptr_rx(int_comm_rx_queue_get());	//Send the first message to the function
		
		int_comm_rx_queue_drop();					//Drop the message from the queue since it's been executed
		return(1);								//Return 1 to show that something was done
	}
	
	return(0);
}

/*! \brief Polls the TX queue in the internal communication and sends the data if there is a message in the queue
 *  \return 1 if a message was in the buffer and got sent, 0 if not
 */
unsigned char internal_comm_poll_tx_queue(void) {
  if (uc_com.flags & (1<<UC_MESSAGE_RESEND)) {
    internal_comm_resend(); 
    
    uc_com.flags &= ~(1<<UC_MESSAGE_RESEND);
  }
  
  if ((int_comm_tx_queue_size() > 0) && (msg_not_acked == 0)) {
		//Send the first message in the queue
    #ifdef INT_COMM_DEBUG
      printf("PSEND\n\r");
    #endif 
    
		internal_comm_send_message(int_comm_tx_queue_get());
		msg_not_acked = 1;
		return(1);	//Return 1 to show that something was done
	}
	
	return(0);
}

/*! \brief Send an ACK message to the internal communication uart */
void internal_comm_send_ack(void) {
  #ifdef INT_COMM_DEBUG
    printf("SA\n\r");
  #endif
  
	unsigned char checksum = UC_COMM_MSG_ACK;
	
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(checksum);
	f_ptr_tx(UC_COMM_MSG_ACK);
	f_ptr_tx(0); 
	f_ptr_tx(UC_COMM_MSG_POSTAMBLE);
	
	counter_tx_timeout = 0;
}

/*! \brief Send a NACK message to the internal communication uart */
void internal_comm_send_nack(void) {
	unsigned char checksum = UC_COMM_MSG_NACK;
	
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(checksum);
	f_ptr_tx(UC_COMM_MSG_NACK);
	f_ptr_tx(0);
	f_ptr_tx(UC_COMM_MSG_POSTAMBLE);
	
	counter_tx_timeout = 0;
}

/*! \brief Send a message to the internal communication uart 
 *  \param tx_message The message we wish to send */
void internal_comm_send_message(UC_MESSAGE tx_message) {
	#ifdef INT_COMM_DEBUG
		printf("S: 0x%02X\n",tx_message.cmd);
	#endif
	
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(tx_message.checksum);
	f_ptr_tx(tx_message.cmd);
	f_ptr_tx(tx_message.length);
	
	for (unsigned char i=0;i<tx_message.length;i++)
		f_ptr_tx(tx_message.data[i]);
	
	f_ptr_tx(UC_COMM_MSG_POSTAMBLE);
	
	counter_tx_timeout = 0;
}

/*! \brief Add a message to the transmit queue 
 *  \param command The command we wish to perform
 *  \param length The length of the data field 
 *  \param data The data we wish to send */
void internal_comm_add_tx_message(unsigned char command, unsigned char length, char *data) {
	#ifdef INT_COMM_DEBUG
		printf("ADD 0x%02X\n",command);
    printf("TX_BUF_SIZE: %i\n\r",int_comm_tx_queue_size());
    printf("RX_BUF_SIZE: %i\n\r",int_comm_rx_queue_count());
	#endif
	
	if (int_comm_tx_queue_size() < INTERNAL_COMM_TX_QUEUE_SIZE) {
	  UC_MESSAGE new_mess;
    
    new_mess.checksum = command + length;
    
    new_mess.cmd = command;
    new_mess.length = length;
    
    for (unsigned char i=0;i<length;i++) {
      new_mess.checksum += data[i];
      new_mess.data[i] = data[i];
      
      #ifdef INT_COMM_DEBUG
        printf("0x%02X ",new_mess.data[i]);
      #endif
    }
    
    #ifdef INT_COMM_DEBUG
      printf("\n");
    #endif

    int_comm_tx_queue_add(new_mess);
  }
  else {
    #ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
      led_set_error(LED_STATE_ON);

      //Set the error flag
      //error_handler_set(ERROR_TYPE_INT_COMM_TX_FULL,1,0);
    #endif
  }
}

void __inline__ internal_comm_message_acked(void) {
	uc_new_message.checksum = 0;
	uc_new_message.cmd = 0;
	uc_new_message.length = 0;
				
	msg_not_acked = 0;
	counter_tx_timeout = 0;
	
	//Last TXed message was acked, lets drop the message from the TX queue
	int_comm_tx_queue_drop();
  
  #ifdef INT_COMM_DEBUG
    printf("ACKED: %i\n\r",int_comm_tx_queue_size());
  #endif
  
	resend_count = 0;
  
  internal_comm_reset_rx();
}

void __inline__ internal_comm_message_nacked(void) {
	internal_comm_resend();
}

/*! \brief Will trigger a resend of the last message */
void internal_comm_resend(void) {
  if (resend_count < UC_COMM_RESEND_COUNT) {
		if (msg_not_acked == 1) {
			internal_comm_send_message(int_comm_tx_queue_get());

			#ifdef INT_COMM_DEBUG
				printf("RS[%i]: %u\n",resend_count,timer);
			#endif
			
			resend_count++;
		}
	}
	else {
		#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
			led_set_error(LED_STATE_ON);
			
			//Set the error flag
			error_handler_set(ERROR_TYPE_INT_COMM_RESEND,1,0);
		#endif

		#ifdef INT_COMM_DEBUG
			printf("RESEND ERROR\n");
      
		#endif
			
		int_comm_tx_queue_drop();
		counter_tx_timeout = 0;
		internal_comm_reset_rx();
		resend_count = 0;
		msg_not_acked = 0;
	}
}

void enable_int_comm_interrupt(void) {
  #ifdef DEVICE_TYPE_MAIN_MOTHERBOARD_UNIT
    UCSR0B |= (1<<RXCIE0);
  #endif

  //! if the device is a frontpanel we need to set the proper USARTs used
  #ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
    UCSR0B |= (1<<RXCIE0);
  #endif

  //! if the device is a frontpanel we need to set the proper USARTs used
  #ifdef DEVICE_TYPE_AMP_CTRL_BOX
    UCSR0B |= (1<<RXCIE0);
  #endif

  #ifdef DEVICE_TYPE_AMP_CTRL_BOX_MOTHERBOARD
    UCSR0B |= (1<<RXCIE0);
  #endif
}

void disable_int_comm_interrupt(void) {
  #ifdef DEVICE_TYPE_MAIN_MOTHERBOARD_UNIT
    UCSR0B &= ~(1<<RXCIE0);
  #endif

  //! if the device is a frontpanel we need to set the proper USARTs used
  #ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
    UCSR0B &= ~(1<<RXCIE0);
  #endif

  //! if the device is a frontpanel we need to set the proper USARTs used
  #ifdef DEVICE_TYPE_AMP_CTRL_BOX
    UCSR0B &= ~(1<<RXCIE0);
  #endif

  #ifdef DEVICE_TYPE_AMP_CTRL_BOX_MOTHERBOARD
    UCSR0B &= ~(1<<RXCIE0);
  #endif
}

/*! \brief Goes through the TX queue and checks if a certain command exists 
 *  \param cmd The command we wish to check if it exists in the queue *
 *  \return 1 if it exist in the queue, 0 if not */
unsigned char internal_comm_check_cmd_in_tx_queue(unsigned char cmd) {
    unsigned char ret_val = 0;
  
  for (unsigned char i=0;i<int_comm_tx_queue_size();i++)
    if (int_comm_tx_queue_get_pos(i).cmd == cmd) {
      ret_val = 1;
      break;
    }
    
  return(ret_val);
}

//! Interrupt when a byte has been received from the UART
ISR(ISR_INTERNAL_COMM_USART_RECV) {
	unsigned char data = INTERNAL_COMM_UDR;
	uc_com.char_count++;
	counter_rx_timeout = 0;
	
	if (uc_com.flags && (1<<UC_PREAMBLE_FOUND)) {
		//Check if this is a postamble
		if ((uc_com.char_count >= 4) && (data == UC_COMM_MSG_POSTAMBLE)) {
			if (uc_new_message.length == (uc_com.char_count-4)) {
				if (uc_new_message.checksum == uc_com.checksum) {
					#ifdef INT_COMM_DEBUG
						printf("AM 0x%02X\n",uc_new_message.cmd);
					#endif
					
					//If there is room in the queue the message gets added and acked, otherwise not
					if (int_comm_rx_queue_count() < INTERNAL_COMM_RX_QUEUE_SIZE) {
            if ((uc_new_message.cmd != UC_COMM_MSG_ACK) && (uc_new_message.cmd != UC_COMM_MSG_NACK)) {
              internal_comm_send_ack();
            
              //Add the message to the RX queue
              int_comm_rx_queue_add(uc_new_message);
              //Send ack to acknowledge we recieved the package
            }
            else {
              if (uc_new_message.cmd == UC_COMM_MSG_ACK)
                internal_comm_message_acked();
              else if (uc_new_message.cmd == UC_COMM_MSG_ACK)
                internal_comm_message_nacked();
            }
          }
          else {
            #ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
              led_set_error(LED_STATE_ON);
        
              //Set the error flag
              //error_handler_set(ERROR_TYPE_INT_COMM_RX_FULL,1,0);
            #endif
          }
				}
				else {
					internal_comm_send_nack();
				}

				uc_com.flags &= ~(1<<UC_PREAMBLE_FOUND);
				uc_com.char_count = 0;
				prev_data = 0;
			}
			else {
				if ((uc_com.char_count - 4) < UC_MESSAGE_DATA_SIZE) {
					uc_new_message.data[uc_com.char_count - 4] = data;
					uc_com.checksum += data;
				}
			}
		}
		else {
			switch(uc_com.char_count) {
				case 1 :
					uc_new_message.checksum = data;
					break;
				case 2 :
					uc_new_message.cmd = data;
					uc_com.checksum = data;
					break;				
				case 3 :	
					uc_new_message.length = data;
					uc_com.checksum += data;
					break;
				default:	
					if ((uc_com.char_count - 4) < UC_MESSAGE_DATA_SIZE) {
              uc_new_message.data[uc_com.char_count - 4] = data;
              uc_com.checksum += data;
            }
				break;
			}
		}
	}
	else if (data == UC_COMM_MSG_PREAMBLE) {
		//Is this a preamble or just a variable sent?
		if (prev_data == UC_COMM_MSG_PREAMBLE) {
			uc_com.flags |= (1 << UC_PREAMBLE_FOUND);
			uc_com.char_count = 0;
			uc_com.checksum = 0;
		}
	}
	
	prev_data = data;
}

//! Interrupt when data has been received from the UART
ISR(ISR_INTERNAL_COMM_USART_DATA) {
	
}

/*! \brief Function which should be called each ms */
void internal_comm_1ms_timer(void) {
	if ((uc_com.char_count > 0) && (counter_rx_timeout >= UC_COMM_RX_TIMEOUT)) {
		counter_rx_timeout = 0;
    
		internal_comm_reset_rx();
	}
		
	timer++;
		
	if ((msg_not_acked == 1) && (counter_tx_timeout >= UC_COMM_TX_TIMEOUT)) {
		counter_tx_timeout = 0;
		
		uc_com.flags |= (1<<UC_MESSAGE_RESEND);
	}
	
	if (msg_not_acked != 0)
		counter_tx_timeout++;
	
	if (uc_com.char_count != 0)
		counter_rx_timeout++;
}