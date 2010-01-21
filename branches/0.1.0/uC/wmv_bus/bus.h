/*! \file bus.h \brief The communication bus protocol used in the openASC project.
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-13
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

#ifndef _BUS_H_
#define _BUS_H_

/* BUS protocol format
 * ----------------------------------------------------------------------------
 * |0xFE|0xFE|to-adr|fm-adr|checksum|flags|cmd|length|data|data|data|data|0xFD|
 * ----------------------------------------------------------------------------
 *
 * The length of a message is total length - 7 bytes (premble, to-adr, fm-adr,checksum,cmd,length,postamble)
 * 
 * If you wish to transmit messages to all the devices on the bus you can use the broadcast address
 * BUS_BROADCAST_ADDR (0x00). The checksum is calculated by adding flags+cmd+length+data[...]
*/
 
#ifdef DEVICE_TYPE_DRIVER_UNIT
	#include "../driver_unit/main.h"
#endif

#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
	#include "../driver_unit_v2/main.h"
#endif
 
#ifdef DEVICE_TYPE_ROTATOR_UNIT
	#include "../rotator_unit/main.h"
#endif
 
 #ifdef DEVICE_TYPE_DRIVER_UNIT
	#define BUS_DISABLE_INTERRUPTS()	TIMSK &= ~(1<<OCIE2);
	#define BUS_ENABLE_INTERRUPTS()		TIMSK |= (1<<OCIE2);
#endif
 
//! Define the proper interrupt routines depending on hardware
#ifdef DEVICE_TYPE_DRIVER_UNIT
	#define ISR_BUS_USART_DATA 	SIG_USART_DATA
	#define ISR_BUS_USART_RECV 	SIG_USART_RECV
	#define ISR_BUS_USART_TRANS	SIG_USART_TRANS
		
	#define ISR_BUS_TIMER_COMPARE		SIG_OUTPUT_COMPARE2
	#define ISR_BUS_TIMER_OVERFLOW	SIG_OVERFLOW2
#endif

#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
	#define ISR_BUS_USART_DATA 	SIG_USART1_DATA
	#define ISR_BUS_USART_RECV 	SIG_USART1_RECV
	#define ISR_BUS_USART_TRANS	SIG_USART1_TRANS
		
	#define ISR_BUS_TIMER_COMPARE		SIG_OUTPUT_COMPARE2
	#define ISR_BUS_TIMER_OVERFLOW	SIG_OVERFLOW2
#endif

#ifdef DEVICE_TYPE_ROTATOR_UNIT
	#define ISR_BUS_USART_DATA 	SIG_USART1_DATA
	#define ISR_BUS_USART_RECV 	SIG_USART1_RECV
	#define ISR_BUS_USART_TRANS	SIG_USART1_TRANS
			
	#define ISR_BUS_TIMER_COMPARE		SIG_OUTPUT_COMPARE2
	#define ISR_BUS_TIMER_OVERFLOW	SIG_OVERFLOW2
#endif

#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
	#include "../front_panel/main.h"
	#define ISR_BUS_USART_DATA 	SIG_USART2_DATA
	#define ISR_BUS_USART_RECV 	SIG_USART2_RECV
	#define ISR_BUS_USART_TRANS	SIG_USART2_TRANS
			
	#define ISR_BUS_TIMER_COMPARE		SIG_OUTPUT_COMPARE2A
	#define ISR_BUS_TIMER_OVERFLOW	SIG_OVERFLOW2
#endif
 
/******* BUS specifics ********/

/*! The preamble of the BUS message */
#define BUS_MSG_PREAMBLE		0xFE
/*! The postamble of the BUS message */
#define BUS_MSG_POSTAMBLE		0xFD
/*! The acknowledge command of the bus */
#define BUS_MSG_ACK					0xFA
/*! The NOT acknowledge command of the bus */
#define BUS_MSG_NACK				0xFB

/*! Bus broadcast address - All broadcast messages should contain an ID explaining which kind of 
    device that is sending the message */
#define BUS_BROADCAST_ADDR	0x00

//! Device ID for the mainbox
#define DEVICE_ID_MAINBOX				1
//! Device ID for the positive driver module
#define DEVICE_ID_DRIVER_POS		2
//! Device ID for the negative driver module
#define DEVICE_ID_DRIVER_NEG		3
//! Device ID for the rotator unit
#define DEVICE_ID_ROTATOR_UNIT	4
//! Device ID for a computer
#define DEVICE_ID_COMPUTER			5

/*! The number of times a message is resent before it's dropped and an error flag is set */
#define BUS_MAX_RESENDS	10

/*! The interval between each status message (time is in ms) */
#define BUS_DEVICE_STATUS_MESSAGE_INTERVAL	1500

/*! The interval which the SYNC command is sent out from the master (time in ms) 
 *  This value is not allowed to be over 5000 ms since that will make it too big for the
 *  timer controlling the SYNC timeout on all the devices. */
#define BUS_MASTER_SYNC_INTERVAL 500

/*! This limit is used to detect if it was too long ago since we receieved a SYNC message
 *  from the master. If so it will stop with all outgoing communication. */
#define BUS_SYNC_TIMEOUT_LIMIT 10000

/*! The timeout limit between a message that was sent to when it will be a resend, this is counted as 
    number of wraparounds on the bus, ie 5 would mean 5 wraparounds */
#define BUS_ACK_WRAPAROUND_LIMIT 10

/*! Timeout limit for how long it can take without receiving a message before the buffer is cleared, this is counted as
    time, 5 would mean 5 * 130 us */
#define BUS_TIMEOUT_LIMIT 5
/******************************/

/*! This bit is to set if the device is MASTER in the bus_status.flags variable */
#define BUS_STATUS_DEVICE_IS_MASTER_BIT	0
/*! This bit shows if it's allowed to send messages on the bus, ie first sync has been received */
#define BUS_STATUS_ALLOWED_TO_SEND_BIT	1
/*! This bit is set if the preamble is found in the bus_status.flags variable */
#define BUS_STATUS_PREAMBLE_FOUND_BIT		2
/*! This bit is to set if the MASTER has sent a SYNC message so we are allowed to start sending */
#define BUS_STATUS_MASTER_SENT_SYNC_BIT	3
/*! This bit is to set if the device own time slot is currently active, ie it is possibly allowed to send */
#define BUS_STATUS_TIME_SLOT_ACTIVE			4
/*! This bit is to indicate that we should try to send the message currently in the TX queue */
#define BUS_STATUS_SEND_MESSAGE					5
/*! This bit is to indicate that we are CURRENTLY sending a message */
#define BUS_STATUS_SEND_ACTIVE					6
/*! This bit is to indicate that we are CURRENTLY receiving a message */
#define BUS_STATUS_RECEIVE_ON						7
/*! This bit is to force a SYNC message to be sent */
#define BUS_STATUS_FORCE_SYNC						8
/*! This bit is to see if a message should be acked or not, used for the timeout of an acknowledge */
#define BUS_STATUS_MESSAGE_ACK_TIMEOUT	9

/*! This is the multiplier for the send window
    BUS_TIME_INTERRUPT_INTERVAL * BUS_TIME_MULTIPLIER = BUS_TIME_FRAME_LIMIT */
#define BUS_TIME_MULTIPLIER 8
/*! The interval of the timer interrupts, in us */
#define BUS_TIME_INTERRUPT_INTERVAL 130
/*! The time frame size of the bus time slots, in us */
#define BUS_TIME_FRAME_LIMIT	1040
/*! The dead time of the time slot, in us. This should be set pretty high to accept rather high clock drift */
#define BUS_SLOT_DEAD_TIME		260

//! Length of the RX buffer
#define SERIAL_RX_BUF_LENGTH 20

//! Define the length of the bus message data field length
#define BUS_MESSAGE_DATA_SIZE 15

//! BUS MESSAGE flags
//! The message should be ACKED
#define BUS_MESSAGE_FLAGS_NEED_ACK			0

/*! BUS message structure */
typedef struct {
	//! From which address the bus message was sent
	unsigned char from_addr;
	//! To which address the bus message should be sent
	unsigned char to_addr;
	//! The checksum of the message
	unsigned char checksum;
	//! Different flags
	unsigned char flags;
	//! The command of the message
	unsigned char cmd;
	//! The length of the data sent in the message
	unsigned char length;
	//! The variables wanted to be sent
	unsigned char data[BUS_MESSAGE_DATA_SIZE];
} BUS_MESSAGE;

//! The structure of the RX circular buffer
typedef struct rx_linked_list {
	BUS_MESSAGE message[BUS_RX_QUEUE_SIZE];
	unsigned char first;
	unsigned char last;
} rx_queue_struct;

//! The structure of the TX circular buffer
typedef struct tx_linked_list {
	BUS_MESSAGE message[BUS_TX_QUEUE_SIZE];
	unsigned char first;
	unsigned char last;
} tx_queue_struct;

/*! The bus status structure */
typedef struct {
	//!Counter which keeps track of which frame (slot) we are in
	unsigned char frame_counter;			
	//!The address of this device
	unsigned char ext_addr;						
	//!The lower limit of the time slot
	unsigned int lower_limit;				
	//!The upper limit of the time slot
	unsigned int upper_limit;				
	//!Nr of devices on the bus
	unsigned char device_count;				
	//!Nr of devices are x 4 so we don't need to do the division
	unsigned int device_count_mult;		
	/*! See details in defines */
	unsigned int flags;
	//!The char count of the received message
	unsigned char char_count;					
	//!The number of times the last message was sent
	unsigned char send_count;
	//! The previous character received
	unsigned char prev_char;
	//! The number of wrap arounds
	unsigned char wraparounds;
} bus_status_struct;

void bus_add_tx_message(unsigned char from_addr, unsigned char to_addr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]);
void bus_add_rx_message(unsigned char from_addr, unsigned char to_addr, unsigned char flags, unsigned char cmd, unsigned char length, unsigned char data[]);
void bus_set_address(unsigned char addr);
unsigned char bus_get_address(void);
void bus_init(void);
void  bus_resend_message(void);

void bus_send_ack(unsigned char to_addr);
void bus_send_nack(unsigned char to_addr);

void bus_message_acked(void);
void bus_message_nacked(void);

void __inline__ bus_reset_tx_status(void);
void __inline__ bus_reset_rx_status(void);

unsigned char bus_is_master(void);
void bus_set_is_master(unsigned char state, unsigned char count);

unsigned char bus_get_device_count(void);
void bus_set_device_count(unsigned char device_count);
unsigned char bus_allowed_to_send(void);
void bus_check_tx_status(void);

#endif
