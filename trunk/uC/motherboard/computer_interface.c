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

/* Protocol structure
 * ------------------
 * --------------------------------------------
 * |0xFE|0xFE|CMD|LENGTH|DATA[0]..DATA[n]|0xFD|
 * --------------------------------------------
 * After each message an ACK should be sent back to the computer. If no other data
 * is returned then just an ACK message containing the checksum, otherwise data that
 * should be sent back with the checksum retrieved as the DATA[0].
 */

//Debug flag for the computer communication protocol
//#define COM_DEBUG
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <avr/wdt.h>

#include "computer_interface.h"
#include "usart.h"
#include "main.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH	128

//! The length of the computer RX BUFFER
#define COMPUTER_TX_BUFFER_LENGTH	25

//! The number of bytes the fixed part of the data structure takes up
#define COMPUTER_INTERFACE_FIXED_SIZE	5

//! The preamble of the computer communication protocol
#define COMPUTER_COMM_PREAMBLE	0xFE
//! The postamble of the computer communication protocol
#define COMPUTER_COMM_POSTAMBLE	0xFD
//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

//! Flag to see if the preamble was found
#define COMPUTER_COMM_FLAG_FOUND_PREAMBLE	1
//! Flag to see that there is data in the rx buffer
#define COMPUTER_COMM_FLAG_DATA_IN_RX_BUF	2

//! Computer interface communication struct
typedef struct {
	//! The serial tx buffer
	char *tx_buffer;
	//! The start of the serial tx buffer
	char *tx_buffer_start;
	//! The length of the data in the buffer
	unsigned int tx_buffer_length;
	//! The serial rx buffer
	char *rx_buffer;
	//! The start of the serial rx buffer
	char *rx_buffer_start;
	//! Different flags
	unsigned char data_in_tx_buffer;
	//! Various flags
	unsigned char flags;
	//! The command
	unsigned char command;
	//! Data length
	unsigned char length;
	//! Number of bytes received
	unsigned int count;
} computer_comm_struct;

//! Computer communication data
computer_comm_struct computer_comm;

//! The bootloader start address
void (*bootloader_start)(void) = (void *)0x1FE00;

unsigned int counter_1ms = 0;
unsigned int counter_computer_interface_rx_timeout = 0;

unsigned char computer_comm_prev_data = 0;

//! \brief Initialize the communication interface towards the computer
void computer_interface_init(void) {
	// Setup the RX buffer
	computer_comm.rx_buffer = (char *)malloc(COMPUTER_RX_BUFFER_LENGTH);
	computer_comm.rx_buffer_start = computer_comm.rx_buffer;
	
	computer_comm.tx_buffer = (char *)malloc(COMPUTER_TX_BUFFER_LENGTH);
	computer_comm.tx_buffer_start = computer_comm.tx_buffer;
	
	computer_comm.data_in_tx_buffer = 0;
}

//! \brief Send data to the computer
void computer_interface_send_data(void) {
	if (computer_comm.data_in_tx_buffer == 1) {
		for (int i=0;i<computer_comm.tx_buffer_length;i++)
			usart1_transmit(computer_comm.tx_buffer_start[i]);
		
		computer_comm.tx_buffer = computer_comm.tx_buffer_start;
		computer_comm.tx_buffer_length = 0;

		computer_comm.data_in_tx_buffer = 0;
	}
}

/*! \brief Send data to the computer
 *  \param command The command we wish to send 
 *  \param length The length of the data 
 *  \param data The data we wish to send */
void computer_interface_send(unsigned char command, unsigned int length, char *data) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = command;
	computer_comm.tx_buffer[3] = length;
	int i;
	for (i=0;i<length;i++)
		computer_comm.tx_buffer[4+i] = data[i];
		
	computer_comm.tx_buffer[4+i] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = COMPUTER_INTERFACE_FIXED_SIZE+length;
			
	computer_comm.data_in_tx_buffer = 1;
}

/*! \brief Send an ACK message */
void computer_interface_send_ack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_ACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

/*! \brief Send a NACK message */
void computer_interface_send_nack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_NACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

/*! \brief Parse the data in the rx_buffer and execute the proper functions */
void computer_interface_parse_data(void) {
	if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF)) {
		if (computer_comm.command == INT_COMM_PC_CTRL) {
			internal_comm_add_tx_message(INT_COMM_PC_CTRL, computer_comm.length, (void *)computer_comm.rx_buffer_start);
		}
		else
			computer_interface_send_nack();
		
		//Clear the RX flag
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
		computer_comm.rx_buffer = computer_comm.rx_buffer_start;
	}
}

void computer_interface_1ms_tick(void) {
	if (counter_computer_interface_rx_timeout > COMPUTER_INTERFACE_RX_TIMEOUT) {
		computer_comm_prev_data = 0;
		computer_comm.rx_buffer = computer_comm.rx_buffer_start;
		
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE);
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
		
		computer_comm.count = 0;
		computer_comm.length = 0;
		
		counter_computer_interface_rx_timeout = 0;
	}
	
	counter_1ms++;
	counter_computer_interface_rx_timeout++;
}

ISR(SIG_USART1_DATA) {
	
}

/*! Interrupt which is called when a byte has been received */
ISR(SIG_USART1_RECV) {
	unsigned char data = UDR1;
	
	//Has a preamble been found?
	if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE)) {
		//Check if the data is a postamble
		if ((data == COMPUTER_COMM_POSTAMBLE) && ((computer_comm.count-2) == computer_comm.length)) {
			computer_comm.rx_buffer = computer_comm.rx_buffer_start;
			
			computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
			computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE);
		}
		else {
			switch(computer_comm.count) {
				case 0:
					computer_comm.command = data;
					break;
				case 1:
					computer_comm.length += data;
					break;
				default:
					*(computer_comm.rx_buffer++) = data;
					break;
			}

			computer_comm.count++;
		}
	}
	else if ((computer_comm_prev_data == COMPUTER_COMM_PREAMBLE) && (data == COMPUTER_COMM_PREAMBLE)) {
		computer_comm.rx_buffer = computer_comm.rx_buffer_start;
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
		computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE);
		
		computer_comm.count = 0;
		computer_comm.length = 0;
	}
	else {
		if ((computer_comm.rx_buffer - computer_comm.rx_buffer_start) >= COMPUTER_RX_BUFFER_LENGTH) {
			computer_comm.rx_buffer = computer_comm.rx_buffer_start;
		}
		else {
			*(computer_comm.rx_buffer++) = data;
		}
	}
	
	computer_comm_prev_data = data;
//	usart3_transmit(data);
}
