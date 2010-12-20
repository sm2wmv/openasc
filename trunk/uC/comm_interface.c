/*! \file comm_interface.c
 *  \brief A generic simple communication interface without queues
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-12-18
 *  \code #include "comm_interface.c" \endcode
 */
//    Copyright (C) 2010  Mikael Larsmark, SM2WMV
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

#include "comm_interface.h"

//! Indicates that there is data to be sent
#define COMM_FLAG_TX_DATA	0
//! Indicates that there is data to be parsed
#define COMM_FLAG_RX_DATA 1
//! Preamble found in the message 
#define COMM_FLAG_PREAMBLE_FOUND 2
//! Flag which is set if the last TX message was acked
#define COMM_TX_MSG_ACKED	3

comm_message_struct rx_message;
comm_message_struct tx_message;

unsigned char tx_timeout = 0;
unsigned char rx_timeout = 0;

unsigned char comm_flags = 0;

unsigned char char_count = 0;
unsigned char prev_char = 0;
unsigned char curr_checksum = 0;

/*! \brief Function to be called when a message is recieved and should be parsed/executed */
void (*f_ptr_rx)(comm_message_struct);
/*! \brief Function to be called when we wish to send a message */
void (*f_ptr_tx)(char);

/*! \brief Initialize the generic communication interface
 *  \param func_ptr_rx The function you wish to call when a new message has been recieved and should be parsed
 *  \param func_ptr_tx The function used to send data to the hardware handeling the data transmission */
void comm_interface_init(void (*func_ptr_rx)(comm_message_struct), void (*func_ptr_tx)(char)) {
	comm_message.length = 0;
	comm_message.command = 0;
	comm_flags = (1<<COMM_TX_MSG_ACKED);
	
	f_ptr_rx = func_ptr_rx;
	f_ptr_tx = func_ptr_tx;
}

/*! \brief Function which checks if there is any message to be sent, if so it transmits it */
void comm_interface_poll_tx_message(void) {
	if (comm_flags & (1<<COMM_FLAG_TX_DATA)) {
		comm_flags &= ~(1<<COMM_FLAG_TX_DATA);
		comm_flags &= ~(1<<COMM_TX_MSG_ACKED);
		
		f_ptr_tx(COMM_INTERFACE_MSG_PREAMBLE);
		f_ptr_tx(COMM_INTERFACE_MSG_PREAMBLE);
		f_ptr_tx(tx_message.checksum);
		f_ptr_tx(tx_message.command);
		f_ptr_tx(tx_message.length);
		
		for (unsigned char i=0;i<tx_message.length;i++)
			f_ptr_tx(tx_message.data[i]);
		
		f_ptr_tx(COMM_INTERFACE_MSG_POSTAMBLE);
	}
}

/*! \brief Checks if there is a message to be parsed, if so then func_rx_ptr is called 
 *  \return 1 if there was a message to be parsed, 0 if not */
unsigned char comm_interface_poll_rx_message(void) {
	if (comm_flags & (1<<COMM_FLAG_RX_DATA)) {
		comm_flags &= ~(1<<COMM_FLAG_RX_DATA);
		
		f_ptr_rx(rx_message);
		
		return(1);
	}
	
	return(0);
}

/*! \brief Function which should be called each ms, keeps track of timeouts etc */
void comm_interface_1_ms_tick(void) {
	tx_timeout++;
	rx_timeout++;
	
	if (rx_timeout > COMM_INTERFACE_RX_TIMEOUT) {
		char_count = 0;
		prev_data = 0;
		comm_flags &= ~(1<<COMM_FLAG_PREAMBLE_FOUND);
	}
}

/*! \brief Function which is called each time a character has been received from the usart
 *  \param data The character which was received */
void comm_interface_char_rxed(unsigned char data) {
	rx_timeout = 0;
	char_count++;
	
	if (comm_flags & (1<<COMM_FLAG_PREAMBLE_FOUND)) {
		//Check if the postamble is found
		if ((char_count > 5) & (data == COMM_FLAG_POSTAMBLE_FOUND) & (rx_message.length == char_count-6)) {
			if (rx_message.command == COMM_INTERFACE_MSG_ACK) {
				comm_flags |= (1<<COMM_TX_MSG_ACKED);
			}
			else if (rx_message.command == COMM_INTERFACE_MSG_NACK) {
				comm_flags &= ~(1<<COMM_TX_MSG_ACKED);
				//TODO: initiate resend etc
			}
			else {
				//There is a message to be parsed
				comm_flags |= (1<<COMM_FLAG_RX_DATA);
				comm_flags &= ~(1<<COMM_FLAG_PREAMBLE_FOUND);
				
				char_count = 0;
			}
		}
		else {
			switch (char_count) {
				case 3:
					rx_message.checksum = data;
					curr_checksum = data;
					break;
				case 4:
					rx_message.command = data;
					curr_checksum += data;
					break;
				case 5:
					rx_message.length = data;
					curr_checksum += data;
					break;
				default:
					rx_message.data[char_count-6] = data;
					curr_checksum += data;
					break;
			}
		}
	}
	else if ((data == COMM_INTERFACE_MSG_PREAMBLE) & (prev_char == COMM_INTERFACE_MSG_PREAMBLE))  {
		comm_flags |= (1<<COMM_FLAG_PREAMBLE_FOUND);
	}
	else {
		char_count = 0;
		prev_char = 0;
	}
	
	prev_char = data;
}