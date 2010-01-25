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

/* TODO: Finish the transmission of data
 *       Timeout for TX messages (if not acked)
 *       Resend of NACKed messages or timeout
 */

struct_uc_com uc_com;
UC_MESSAGE uc_new_message;

unsigned char prev_data = 0;

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
	
	int_comm_rx_queue_dropall();
	int_comm_tx_queue_dropall();
}

/*! \brief Polls the RX queue in the internal communication and calls the function defined in internal_comm_init.
 * \return 1 if a message was in the buffer and got parsed, 0 if not
 */
unsigned char internal_comm_poll_rx_queue(void) {
	if (!int_comm_rx_queue_is_empty()) {
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
	if ((!int_comm_tx_queue_is_empty()) && (msg_not_acked == 0)) {
		//Send the first message in the queue
		internal_comm_send_message(int_comm_tx_queue_get());	

		msg_not_acked = 1;		
		return(1);	//Return 1 to show that something was done
	}
	
	return(0);
}

/*! \brief Send an ACK message to the internal communication uart */
void internal_comm_send_ack(void) {
	unsigned char checksum = UC_COMM_MSG_ACK;
	
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(checksum);
	f_ptr_tx(UC_COMM_MSG_ACK);
	f_ptr_tx(0);
	f_ptr_tx(UC_COMM_MSG_POSTAMBLE);
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
}

/*! \brief Send a message to the internal communication uart 
 *  \param tx_message The message we wish to send */
void internal_comm_send_message(UC_MESSAGE tx_message) {
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(UC_COMM_MSG_PREAMBLE);
	f_ptr_tx(tx_message.checksum);
	f_ptr_tx(tx_message.cmd);
	f_ptr_tx(tx_message.length);
	
	for (unsigned char i=0;i<tx_message.length;i++)
		f_ptr_tx(tx_message.data[i]);
	
	f_ptr_tx(UC_COMM_MSG_POSTAMBLE);
}

/*! \brief Add a message to the transmit queue 
 *  \param command The command we wish to perform
 *  \param length The length of the data field 
 *  \param data The data we wish to send */
void internal_comm_add_tx_message(unsigned char command, unsigned char length, char *data) {
	UC_MESSAGE new_mess;
	
	new_mess.checksum = command + length;
	
	new_mess.cmd = command;
	new_mess.length = length;
	
	for (unsigned char i=0;i<length;i++) {
		new_mess.checksum += data[i];
		new_mess.data[i] = data[i];
	}
	
	int_comm_tx_queue_add(new_mess);
}

ISR(ISR_INTERNAL_COMM_USART_RECV) {
	unsigned char data = INTERNAL_COMM_UDR;
	uc_com.char_count++;

	if (uc_com.flags && (1<<UC_PREAMBLE_FOUND)) {
		//Check if this is a postamble
		if ((uc_com.char_count >= 4) && (data == UC_COMM_MSG_POSTAMBLE)) {
			if (uc_new_message.length == (uc_com.char_count-4)) {
				if (uc_new_message.checksum == uc_com.checksum) {
					if (uc_new_message.cmd == UC_COMM_MSG_ACK) {
						//Last TXed message was acked, lets drop the message from the TX queue
						int_comm_tx_queue_drop();
						msg_not_acked = 0;
					}
					else {
						//Add the message to the RX queue
						int_comm_rx_queue_add(uc_new_message);
						//Send ack to acknowledge we recieved the package
						internal_comm_send_ack();
					}
				}
				else
					internal_comm_send_nack();
				
				uc_com.flags &= ~(1<<UC_PREAMBLE_FOUND);
				uc_com.char_count = 0;
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
		}
	}
	
	prev_data = data;
}

ISR(ISR_INTERNAL_COMM_USART_DATA) {
	
}