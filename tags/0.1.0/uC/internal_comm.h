/*! \file internal_comm.h \brief Internal communication between various devices
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-11-22
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

/*! When using these routines for the internal communication it's important
    to initialize the pointers for the transmit and recieve data before any
    of the other functions are used. This is done by using the void internal_comm_init(void (*func_ptr_rx)(UC_MESSAGE), void (*func_ptr_tx)(char)); where func_ptr_rx and func_ptr_rx should point the functions which take the argument of UC_MESSAGE.

	  Doing it this way makes the routines adaptable do different hardware, you just change the routine for TX and RX of data.
 
    When a message has been recieved it will be added to the RX queue and by polling communication by using
    internal_comm_poll_rx_queue(void) if there is a message in the queue it will get sent to the routine which was specified
    in the initialization routine.

		You will also need to poll the internal_comm_poll_tx_queue() at x intervals so that messages are sent when the tx queue
		isn't empty.
*/

#ifndef _INTERNAL_COMM_H_
#define _INTERNAL_COMM_H_

//#include "main.h"

#ifdef DEVICE_TYPE_MAIN_MOTHERBOARD_UNIT
	#define ISR_INTERNAL_COMM_USART_RECV	SIG_USART0_RECV
	#define ISR_INTERNAL_COMM_USART_DATA	SIG_USART0_DATA
	
	#define	INTERNAL_COMM_UDR							UDR0
#endif

#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
#define ISR_INTERNAL_COMM_USART_RECV	SIG_USART0_RECV
#define ISR_INTERNAL_COMM_USART_DATA	SIG_USART0_DATA
	
#define	INTERNAL_COMM_UDR							UDR0
#endif

/*
	| PREAMBLE | PREAMBLE | CHECKSUM | CMD | LENGTH | DATA[0] | DATA[1] | .. | POSTAMBLE |
*/

/****************************************************************/
//! Preamble found for the communication between the uCs
#define UC_PREAMBLE_FOUND			0
#define UC_MESSAGE_IN_BUFFER	1

//! Size of UC MESSAGE fixed part
#define UC_SIZE_FIXED	5

/*! The preamble of the BUS message */
#define UC_COMM_MSG_PREAMBLE		0xFE
/*! The postamble of the BUS message */
#define UC_COMM_MSG_POSTAMBLE		0xFD
/*! The acknowledge command of the bus */
#define UC_COMM_MSG_ACK					0xFB
/*! The NOT acknowledge command of the bus */
#define UC_COMM_MSG_NACK				0xFA

//! The length of the serial rx buffer used for communication between the uCs
#define UC_SERIAL_RX_BUFFER_LENGTH	20

//! The size the data sent between the two devices can be maximum
#define UC_MESSAGE_DATA_SIZE	15

/*! uC message structure, used for communication between the uCs */
typedef struct {
	//! The checksum of the message
	unsigned char checksum;
	//! The command of the message
	unsigned char cmd;
	//! The length of the data sent in the message
	unsigned char length;
	//! The variables wanted to be sent
	unsigned char data[UC_MESSAGE_DATA_SIZE];
} UC_MESSAGE;

/* Variables used for the communication between the two uCs */
typedef struct {
	unsigned char flags;
	unsigned char checksum;
	unsigned char char_count;
} struct_uc_com;
/****************************************************************/

void internal_comm_init(void (*func_ptr_rx)(UC_MESSAGE), void (*func_ptr_tx)(char));
unsigned char internal_comm_poll_rx_queue(void);
unsigned char internal_comm_poll_tx_queue(void);
void internal_comm_add_tx_message(unsigned char command, unsigned char length,char *data);

void internal_comm_send_ack(void);
void internal_comm_send_nack(void);
void internal_comm_send_message(UC_MESSAGE tx_message);

#endif
