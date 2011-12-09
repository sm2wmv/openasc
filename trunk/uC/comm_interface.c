/*! \file comm_interface.c
 *  \brief General communication interface
 *  \ingroup comm_interface_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-09
 *  \code #include "comm_interface.c" \endcode
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
#include "comm_interface.h"

#include "global.h"
#include "comm_interface_rx_queue.h"
#include "comm_interface_tx_queue.h"

//! RX data CHAR receieve timeout limit in ms (max 255 ms)
#define	RX_DATA_TIMEOUT	10
//! TX data transmit timeout limit in ms (max 255 ms)
#define	TX_DATA_TIMEOUT	100

#define PREAMBLE_FOUND	0
//! Was the last TX message acked
#define TX_MSG_ACKED	1
//! A resend should be executed
#define RESEND_FLAG		2

//! Communication protocol status
static unsigned char comm_status = 0;

//! The previous received character from UDR0
static unsigned char prev_data = 0;

//! Keep track of received number of characters
static unsigned char rx_char_count = 0;

static unsigned char resend_count = 0;

static unsigned char add_rx_message = 0;

//! Computer comm preamble
#define COMPUTER_PREAMBLE	  0xFE
//! Computer comm postamble
#define COMPUTER_POSTAMBLE	0xFD
//! Computer MSG Acknowledge
#define COMPUTER_ACK				0xFA
//! Computer MSG NOT Acknowledge
#define COMPUTER_NACK				0xFB

#define RESET_TX_MSG_ACKED comm_status &= ~(1<<TX_MSG_ACKED);
#define SET_TX_MSG_ACKED comm_status |= (1<<TX_MSG_ACKED);

static struct_comm_interface_msg	rx_message;

/*! \brief Function to be called when a message is recieved and should be parsed/executed */
static void (*f_ptr_rx)(struct_comm_interface_msg);
/*! \brief Function to be called when we wish to send a message */
static void (*f_ptr_tx)(char);

//! Counter with 1 ms ticks
static unsigned int counter_1ms = 0;

//! Timeout for the reception of data
static unsigned char rx_data_timeout = 0;

//! Timeout for the transmission of data
static unsigned char tx_data_timeout = 0;

//! RX message checksum
static unsigned char rx_checksum = 0;

void comm_interface_send_message(struct_comm_interface_msg message);
void comm_interface_send_ack(void);
void comm_interface_send_nack(void);

void enable_comm_interface_interrupt(void) {
  #ifdef DEVICE_TYPE_STN_CTRL_BOARD
    UCSR0B |= (1<<RXCIE0);
  #endif
}

void disable_comm_interface_interrupt(void) {
  #ifdef DEVICE_TYPE_STN_CTRL_BOARD
    UCSR0B &= ~(1<<RXCIE0);
  #endif
}

static void __inline__ reset_rx_status(void) {
	rx_data_timeout = 0;
	rx_char_count = 0;
	rx_checksum = 0;
	prev_data = 0;
	
	comm_status &= ~(1<<PREAMBLE_FOUND);
}

static void __inline__ reset_tx_status(void) {
	tx_data_timeout = 0;
}

void comm_interface_poll_tx_queue(void) {
	//Check that the last message sent was acked, if so we transmit the next one in the queue
	if ((comm_status & (1<<TX_MSG_ACKED)) && (comm_interface_tx_queue_size() > 0)) {
		comm_interface_send_message(comm_interface_tx_queue_get());

		RESET_TX_MSG_ACKED;
		tx_data_timeout = 0;
	}
	else if (comm_status & (1<<RESEND_FLAG)) {
		if (resend_count < COMM_INTERFACE_RESEND_COUNT) {
			comm_interface_send_message(comm_interface_tx_queue_get());

			tx_data_timeout = 0;
			resend_count++;
		}
		else {
			comm_interface_tx_queue_drop();
			
			PORTD |= (1<<7);
			
			SET_TX_MSG_ACKED;
			resend_count = 0;
			tx_data_timeout = 0;
		}
		
		comm_status &= ~(1<<RESEND_FLAG);
	}
}

/*! \brief Add a message to be sent
 *  \param cmd The command
 *  \param length The number of byte data
 *  \param data The data to be sent
 *  \return 0 if everything went OK, 1 if the TX queue was full */
unsigned char comm_interface_add_tx_message(unsigned char cmd, unsigned char length, unsigned char *data) {
	struct_comm_interface_msg message;

	message.cmd = cmd;
	message.checksum = cmd;
	message.length = length;
	message.checksum += length;
	
	for (unsigned char i=0;i<length;i++) {
		message.checksum += data[i];
		message.data[i] = data[i];
	}
	
	return(comm_interface_tx_queue_add(message));	
}

void comm_interface_poll_rx_queue(void) {
	if (add_rx_message) {
		if (rx_message.cmd == COMPUTER_ACK) {
			comm_interface_tx_queue_drop();
			
			SET_TX_MSG_ACKED;
			resend_count = 0;
		}
		else if (rx_message.cmd == COMPUTER_NACK) {
			comm_status |= (1<<RESEND_FLAG);
		}
		else if (!comm_interface_rx_queue_add(rx_message)) {
			comm_interface_send_ack();	//Only send ACK if the message has been added to the queue
		}
				
		reset_rx_status();
		add_rx_message = 0;
	}

	if (!comm_interface_rx_queue_is_empty()) {
		f_ptr_rx(comm_interface_rx_queue_get());
		
		comm_interface_rx_queue_drop();
		
		reset_rx_status();
	}
}

void comm_interface_send_message(struct_comm_interface_msg message) {
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(message.checksum);
	f_ptr_tx(message.cmd);
	f_ptr_tx(message.length);
	
	for (unsigned char i=0;i<message.length;i++)
		f_ptr_tx(message.data[i]);
		
	f_ptr_tx(COMPUTER_POSTAMBLE);
}

void comm_interface_send_ack(void) {
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_ACK);	//Checksum
	f_ptr_tx(COMPUTER_ACK);	//Cmd
	f_ptr_tx(0);							//Date length
	f_ptr_tx(COMPUTER_POSTAMBLE);
}

void comm_interface_send_nack(void) {
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_PREAMBLE);
	f_ptr_tx(COMPUTER_NACK);	//Checksum
	f_ptr_tx(COMPUTER_NACK);	//Cmd
	f_ptr_tx(0);							//Date length
	f_ptr_tx(COMPUTER_POSTAMBLE);	
}

void comm_interface_init(void (*func_ptr_rx)(struct_comm_interface_msg), void (*func_ptr_tx)(char)) {
	reset_rx_status();
	reset_tx_status();
	
	comm_interface_rx_queue_init();
	comm_interface_tx_queue_init();
	
	f_ptr_tx = func_ptr_tx;
	f_ptr_rx = func_ptr_rx;
	
	SET_TX_MSG_ACKED;
}

void comm_interface_1ms_tick(void) {
  counter_1ms++;
	
	rx_data_timeout++;
	tx_data_timeout++;
	
	//Reset the RX status if the rx_data_timeout does time out
	if (rx_data_timeout > RX_DATA_TIMEOUT) {
		reset_rx_status();
	}
	
	if (tx_data_timeout > TX_DATA_TIMEOUT) {
		if ((comm_status & (1<<TX_MSG_ACKED)) == 0)
			comm_status |= (1<<RESEND_FLAG);
		
		tx_data_timeout = 0;
	}
}

ISR(SIG_USART0_DATA) {

}

/*! Interrupt which is called when a byte has been received */
ISR(SIG_USART0_RECV) {
  unsigned char data = UDR0;
	rx_data_timeout = 0;

	if (comm_status & (1<<PREAMBLE_FOUND)) {
		if ((data == COMPUTER_POSTAMBLE) && ((rx_char_count - 3) == rx_message.length)) {
			if (rx_message.checksum == rx_checksum) {
				add_rx_message = 1;
			}
			else {
				reset_rx_status();
				comm_interface_send_nack();
			}
		}
		else {
			if (rx_char_count < (COMM_INTERFACE_DATA_LENGTH + 3)) {
				switch(rx_char_count) {
					case 0: 
						rx_message.checksum = data;
						break;
					case 1:
						rx_message.cmd = data;
						rx_checksum += data;
						break;
					case 2:
						rx_message.length = data;
						rx_checksum += data;
						break;
					default:
						rx_message.data[rx_char_count-3] = data;
						rx_checksum += data;
						break;
				}
				
				rx_char_count++;
			}
			else {
				reset_rx_status();
			}
		}
	}
	else {
		if ((prev_data == COMPUTER_PREAMBLE) && (data == COMPUTER_PREAMBLE)) {	//Preamble found
			reset_rx_status();
			comm_status |= (1<<PREAMBLE_FOUND);
		}
	}
	
	prev_data = data;
}
