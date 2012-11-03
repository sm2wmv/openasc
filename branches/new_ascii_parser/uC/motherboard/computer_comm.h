/*! \file computer_comm.h
 *  \brief The computer communication routines
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-22
 *  \code #include "computer_comm.h" \endcode
 *  \defgroup computer_comm_group Computer communication routines
 * 
 *  When using these routines for the computer communication it's important
 *  to initialize the pointers for the transmit and recieve data before any
 *  of the other functions are used. This is done by using the 
 *  void computer_comm_init(void (*func_ptr_rx)(COMM_MESSAGE), void (*func_ptr_tx)(char)); 
 *  where func_ptr_rx and func_ptr_rx should point the functions which take the argument of COMM_MESSAGE.
 *
 *  Doing it this way makes the routines adaptable do different hardware, you just change the routine for TX and RX of data.
 *
 *  When a message has been recieved it will be added to the RX queue and by polling communication by using
 *  computer_comm_poll_rx_queue(void) if there is a message in the queue it will get sent to the routine which was specified
 *  in the initialization routine.

 *  You will also need to poll the computer_comm_poll_tx_queue() at x intervals so that messages are sent when the tx queue
 *	isn't empty.
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

#ifndef _COMPUTER_COMM_H_
#define _COMPUTER_COMM_H_

#define ISR_COMPUTER_COMM_USART_RECV	SIG_USART1_RECV
#define ISR_COMPUTER_COMM_USART_DATA	SIG_USART1_DATA
	
#define	COMPUTER_COMM_UDR							UDR1

/*
	| PREAMBLE | PREAMBLE | CHECKSUM | CMD | LENGTH | DATA[0] | DATA[1] | .. | POSTAMBLE |
*/

/****************************************************************/
//! Preamble found for the communication between the uCs
#define COMM_PREAMBLE_FOUND			0
//! Flag that a message is in the buffer
#define COMM_MESSAGE_IN_BUFFER	1
//! Resend
#define COMM_MESSAGE_RESEND     2

//! Size of UC MESSAGE fixed part
#define COMPUTER_SIZE_FIXED	5

/*! The preamble of the BUS message */
#define COMPUTER_COMM_MSG_PREAMBLE		0xFE
/*! The postamble of the BUS message */
#define COMPUTER_COMM_MSG_POSTAMBLE		0xFD
/*! The acknowledge command */
#define COMPUTER_COMM_MSG_ACK					0xFA
/*! The NOT acknowledge command  */
#define COMPUTER_COMM_MSG_NACK				0xFB

//! The length of the serial rx buffer used for communication between the uCs
#define COMM_SERIAL_RX_BUFFER_LENGTH	25

//! The size the data sent between the two devices can be maximum
#define COMM_MESSAGE_DATA_SIZE	15

//! After this many ms it will reset the rx flags (in ms) (change back to 25?)
#define COMPUTER_COMM_RX_TIMEOUT	5

//! After this many ms a resend will occur if a message has not been acked (in ms) (change back to 25?)
#define COMPUTER_COMM_TX_TIMEOUT	10

//! Number of resends that is allowed (change back to 10?)
#define COMPUTER_COMM_RESEND_COUNT	15


/*! uC message structure, used for communication between the uCs */
typedef struct {
	//! The checksum of the message
	unsigned char checksum;
	//! The command of the message
	unsigned char cmd;
	//! The length of the data sent in the message
	unsigned char length;
	//! The variables wanted to be sent
	unsigned char data[COMM_MESSAGE_DATA_SIZE];
} COMM_MESSAGE;

/*! Variables used for the communication between the two uCs */
typedef struct {
	//! Various flags, defined in this file
	unsigned char flags;
	//! The checksum
	unsigned char checksum;
	//! Number of characters received
	unsigned char char_count;
} struct_computer_com;
/****************************************************************/

void computer_comm_init(void (*func_ptr_rx)(COMM_MESSAGE), void (*func_ptr_tx)(char));
unsigned char computer_comm_poll_rx_queue(void);
unsigned char computer_comm_poll_tx_queue(void);
void computer_comm_add_tx_message(unsigned char command, unsigned char length,char *data);

void __inline__ computer_comm_message_acked(void);
void __inline__ computer_comm_message_nacked(void);

void computer_comm_send_ack(void);
void computer_comm_send_nack(void);
void computer_comm_send_message(COMM_MESSAGE tx_message);
void __inline__ computer_comm_reset_rx(void);
void computer_comm_1ms_timer(void);

void computer_comm_resend(void);

void computer_interface_send_data(void);
void computer_interface_parse_data(void);
void computer_interface_1ms_tick(void);
void computer_interface_tx_message(unsigned char length, char *data);

#endif
