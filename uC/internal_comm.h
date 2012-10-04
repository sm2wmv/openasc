/*! \file internal_comm.h
 *  \brief The internal communication routines
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "internal_comm.h" \endcode
 *  \defgroup internal_comm_group Internal communication routines
 * 
 *  When using these routines for the internal communication it's important
 *  to initialize the pointers for the transmit and recieve data before any
 *  of the other functions are used. This is done by using the void internal_comm_init(void (*func_ptr_rx)(UC_MESSAGE), void (*func_ptr_tx)(char)); where func_ptr_rx and func_ptr_rx should point the functions which take the argument of UC_MESSAGE.
 *
 *  Doing it this way makes the routines adaptable do different hardware, you just change the routine for TX and RX of data.
 *
 *  When a message has been recieved it will be added to the RX queue and by polling communication by using
 *  internal_comm_poll_rx_queue(void) if there is a message in the queue it will get sent to the routine which was specified
 *  in the initialization routine.

 *  You will also need to poll the internal_comm_poll_tx_queue() at x intervals so that messages are sent when the tx queue
 *	isn't empty.
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

#ifndef _INTERNAL_COMM_H_
#define _INTERNAL_COMM_H_

#include "comm_interface.h"

//#define INT_COMM_DEBUG 1

//! if the device is a motherboard we need to set the proper USARTs used
#ifdef DEVICE_TYPE_MAIN_MOTHERBOARD_UNIT
	#define ISR_INTERNAL_COMM_USART_RECV	SIG_USART0_RECV
	#define ISR_INTERNAL_COMM_USART_DATA	SIG_USART0_DATA
	
	#define	INTERNAL_COMM_UDR							UDR0
#endif

//! if the device is a frontpanel we need to set the proper USARTs used
#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#define ISR_INTERNAL_COMM_USART_RECV	SIG_USART0_RECV
	#define ISR_INTERNAL_COMM_USART_DATA	SIG_USART0_DATA
	
	#define	INTERNAL_COMM_UDR							UDR0
#endif

//! if the device is a frontpanel we need to set the proper USARTs used
#ifdef DEVICE_TYPE_AMP_CTRL_BOX
  #define ISR_INTERNAL_COMM_USART_RECV  SIG_USART0_RECV
  #define ISR_INTERNAL_COMM_USART_DATA  SIG_USART0_DATA
    
  #define INTERNAL_COMM_UDR             UDR0
#endif

#ifdef DEVICE_TYPE_AMP_CTRL_BOX_MOTHERBOARD
  #define ISR_INTERNAL_COMM_USART_RECV  SIG_USART0_RECV
  #define ISR_INTERNAL_COMM_USART_DATA  SIG_USART0_DATA
    
  #define INTERNAL_COMM_UDR             UDR0
#endif

/*
	| PREAMBLE | PREAMBLE | CHECKSUM | CMD | LENGTH | DATA[0] | DATA[1] | .. | POSTAMBLE |
*/

/****************************************************************/
//! Preamble found for the communication between the uCs
#define UC_PREAMBLE_FOUND			0
//! Flag that a message is in the buffer
#define UC_MESSAGE_IN_BUFFER	1
//! Resend
#define UC_MESSAGE_RESEND     2

//! Size of UC MESSAGE fixed part
#define UC_SIZE_FIXED	5

/*! The preamble of the BUS message */
#define UC_COMM_MSG_PREAMBLE		0xFE
/*! The postamble of the BUS message */
#define UC_COMM_MSG_POSTAMBLE		0xFD
/*! The acknowledge command */
#define UC_COMM_MSG_ACK					0xFA
/*! The NOT acknowledge command  */
#define UC_COMM_MSG_NACK				0xFB

//! The size the data sent between the two devices can be maximum
#define UC_MESSAGE_DATA_SIZE	15

//! After this many ms it will reset the rx flags (in ms) (change back to 25?)
#define UC_COMM_RX_TIMEOUT	10

//! After this many ms a resend will occur if a message has not been acked (in ms) (change back to 25?)
#define UC_COMM_TX_TIMEOUT	100

//! Number of resends that is allowed (change back to 10?)
#define UC_COMM_RESEND_COUNT	5

/*! Variables used for the communication between the two uCs */
typedef struct {
	//! Various flags, defined in this file
	unsigned char flags;
	//! The checksum
	unsigned char checksum;
	//! Number of characters received
	unsigned char char_count;
} struct_uc_com;
/****************************************************************/

void internal_comm_init(void (*func_ptr_rx)(struct_comm_interface_msg), void (*func_ptr_tx)(char));
unsigned char internal_comm_poll_rx_queue(void);
unsigned char internal_comm_poll_tx_queue(void);
void internal_comm_add_tx_message(unsigned char command, unsigned char length,char *data);

void __inline__ internal_comm_message_acked(void);
void __inline__ internal_comm_message_nacked(void);

void internal_comm_send_ack(void);
void internal_comm_send_nack(void);
void internal_comm_send_message(struct_comm_interface_msg tx_message);
void __inline__ internal_comm_reset_rx(void);
void internal_comm_1ms_timer(void);

void internal_comm_resend(void);

unsigned char internal_comm_check_critical_cmd_state(void);

void enable_int_comm_interrupt(void);
void disable_int_comm_interrupt(void);

unsigned char internal_comm_is_tx_queue_full(void);

#endif
