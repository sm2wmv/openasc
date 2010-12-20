/*! \file comm_interface.h
 *  \brief A generic simple communication interface without queues
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-12-18
 *  \code #include "comm_interface.h" \endcode
 *  \defgroup comm_interface_group Generic communication interface
 * 
 *  When using these routines for the generic communication interface it's important
 *  to initialize the pointers for the transmit and recieve data before any
 *  of the other functions are used. This is done by using the 
 *  void comm_interface_init(void (*func_ptr_rx)(comm_message), void (*func_ptr_tx)(char)); 
 *  where func_ptr_rx and func_ptr_rx should point the functions which take the argument of comm_message.
 *
 *  Doing it this way makes the routines adaptable do different hardware, you just change the routine for TX and RX of data.
 *
 *  When a message has been recieved it will be added to the RX queue and by polling communication by using
 *  comm_interface_poll_rx_queue(void) if there is a message in the queue it will get sent to the routine which was specified
 *  in the initialization routine.

 *  You will also need to poll the comm_interface_poll_tx_queue() at x intervals so that messages are sent when the tx queue
 *	isn't empty.
 *  
 *  Each time a character is received which should be handled by the comm interface, the function comm_interface_char_rxed(unsigned char data)
 *  should be called.
 * 
 *  The function which is called comm_interface_1_ms_tick() should be called each ms
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

//! Number of bytes in the data buffer inside a comm message
#define COMM_MESSAGE_DATA_LENGTH	20

/*! The preamble of the comm message */
#define COMM_INTERFACE_MSG_PREAMBLE		0xFE
/*! The postamble of the comm message */
#define COMM_INTERFACE_MSG_POSTAMBLE	0xFD
/*! The acknowledge command */
#define COMM_INTERFACE_MSG_ACK				0xFA
/*! The NOT acknowledge command  */
#define COMM_INTERFACE_MSG_NACK				0xFB

typedef struct {
	//! The checksum of the message
	unsigned char checksum;
	//! The command of the comm interface message
	unsigned char command;
	//! The length of the data field in this message
	unsigned char length;
	//! The data
	unsigned char data[COMM_MESSAGE_DATA_LENGTH];
} comm_message_struct;

/*! \brief Initialize the generic communication interface
 *  \param func_ptr_rx The function you wish to call when a new message has been recieved and should be parsed
 *  \param func_ptr_tx The function used to send data to the hardware handeling the data transmission */
void comm_interface_init(void (*func_ptr_rx)(comm_message_struct), void (*func_ptr_tx)(char));

/*! \brief Function which should be called each ms, keeps track of timeouts etc */
void comm_interface_1_ms_tick(void);

/*! \brief Function which checks if there is any message to be sent, if so it transmits it */
void comm_interface_poll_tx_message(void);

/*! \brief Checks if there is a message to be parsed, if so then func_rx_ptr is called 
 *  \return 1 if there was a message to be parsed, 0 if not */
unsigned char comm_interface_poll_rx_message(void);

/*! \brief Function which is called each time a character has been received from the usart
 *  \param data The character which was received */
void comm_interface_char_rxed(unsigned char data);