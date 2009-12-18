/*! \file computer_interface.c \brief Interface towards the computer
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-12-16
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
#define COMPUTER_TX_BUFFER_LENGTH	20

#define COMPUTER_INTERFACE_FIXED_SIZE	5

//! The preamble of the computer communication protocol
#define COMPUTER_COMM_PREAMBLE	0xFE
//! The postamble of the computer communication protocol
#define COMPUTER_COMM_POSTAMBLE	0xFD
//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB

#define COMPUTER_COMM_REDIRECT_DATA					0x10

//! Flag to see if the preamble was found
#define COMPUTER_COMM_FLAG_FOUND_PREAMBLE	1
//! Flag to see that there is data in the rx buffer
#define COMPUTER_COMM_FLAG_DATA_IN_RX_BUF	2

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
	unsigned char flags;
	unsigned char command;
	unsigned char length;
	unsigned int count;
} computer_comm_struct;

computer_comm_struct computer_comm;

void (*bootloader_start)(void) = (void *)0x1FE00;

//! Initialize the communication interface towards the computer
void computer_interface_init(void) {
	// Setup the RX buffer
	computer_comm.rx_buffer = (char *)malloc(COMPUTER_RX_BUFFER_LENGTH);
	computer_comm.rx_buffer_start = computer_comm.rx_buffer;
	
	computer_comm.tx_buffer = (char *)malloc(COMPUTER_TX_BUFFER_LENGTH);
	computer_comm.tx_buffer_start = computer_comm.tx_buffer;
	
	computer_comm.data_in_tx_buffer = 0;
}

void computer_interface_send_data(void) {
	if (computer_comm.data_in_tx_buffer == 1) {
		for (int i=0;i<computer_comm.tx_buffer_length;i++)
			usart1_transmit(computer_comm.tx_buffer_start[i]);
		
		computer_comm.tx_buffer = computer_comm.tx_buffer_start;
		computer_comm.tx_buffer_length = 0;

		computer_comm.data_in_tx_buffer = 0;
	}
}

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

void computer_interface_send_ack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_ACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_send_nack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_NACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_send_flag(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = 0xFC;
	computer_comm.tx_buffer[3] = 0x01;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_parse_data(void) {
	if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF)) {
		
		if (computer_comm.command == INT_COMM_REDIRECT_DATA) {
			internal_comm_add_tx_message(INT_COMM_PC_CTRL, computer_comm.length, (void *)computer_comm.rx_buffer_start);
			
			computer_interface_send_ack();
		}
		else
			computer_interface_send_nack();
		
		//Clear the RX flag
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
		computer_comm.rx_buffer = computer_comm.rx_buffer_start;
	}
}

ISR(SIG_USART1_DATA) {
	
}

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
	else if ((computer_comm.rx_buffer - computer_comm.rx_buffer_start > 0) && (data == COMPUTER_COMM_PREAMBLE)
							&& (*(computer_comm.rx_buffer-1) == COMPUTER_COMM_PREAMBLE)) {
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
}
