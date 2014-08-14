/*! \file amplifiers/finnpa/ctrl_board/usart.c
 *  \brief USART routines
 *  \ingroup ctrl_board_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "amplifiers/finnpa/ctrl_board/usart.h" \endcode
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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

/*! \brief Initiliaze the USART0 for the communication bus 
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baudrate The baudrate param from the ATMEGA2560 datasheet.
 */
void usart0_init(unsigned int baudrate) {
	/* Set baud rate */
	UBRR0H = (unsigned char) (baudrate>>8);
	UBRR0L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

/*! \brief Send a character to the USART0
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
void usart0_transmit(char data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

/*! \brief Send a string of characters to the USART0
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart0_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		usart0_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART0
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart0_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	return UDR0;
}

/*! \brief The USART0 recieve loopback
 * This function does wait for a character in the RX buffer and returns
 * it to the transmit buffer.
 * \return The character from the RX USART buffer
 */
unsigned char usart0_receive_loopback(void ) {
	uint8_t rbuff;
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	/* Get and return received data from buffer */
	rbuff = UDR0;
	usart0_transmit(rbuff);
	return rbuff;
}

/*! \brief Retrieve one character from the USART0
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart0_receive(void ) {
	/* Check if data is received */
	return ((UCSR0A & (1<<RXC0)));
}

/*! \brief Initiliaze the USART1 for the interface towards the computer
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baudrate The baudrate param from the ATMEGA2560 datasheet.
 * \param stopbits The number of stopbits.
 */
void usart1_init(unsigned int baudrate, unsigned char stopbits) {
	/* Set baud rate */
	UBRR1H = (unsigned char) (baudrate>>8);
	UBRR1L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCSZ12);
	
	if (stopbits == 1)
		UCSR1C |= (0<<USBS1);
	else if (stopbits == 2)
		UCSR1C |= (1<<USBS1);
	
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);// | (1<<RXCIE1);
}

/*! \brief Send a character to the USART1
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
unsigned char usart1_transmit(char  data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR1A & (1<<UDRE1)));
	/* Put data into buffer, sends the data */
	UDR1 = data;
	return 0;
}

/*! \brief Send a string of characters to the USART1
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart1_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		usart1_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART1
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart1_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR1A & (1<<RXC1)));
	/* Get and return received data from buffer */
	return UDR1;
}

/*! \brief The USART1 recieve loopback
 * This function does wait for a character in the RX buffer and returns
 * it to the transmit buffer.
 * \return The character from the RX USART buffer
 */
unsigned char usart1_receive_loopback(void ) {
	uint8_t rbuff;
	/* Wait for data to be received */
	while (!(UCSR1A & (1<<RXC1)));
	/* Get and return received data from buffer */
	rbuff = UDR1;
	usart0_transmit(rbuff);
	return rbuff;
}

/*! \brief Retrieve one character from the USART1
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart1_receive(void ) {
	/* Check if data is received */
	return ((UCSR1A & (1<<RXC1)));
}

/*! \brief Initiliaze the USART3 for the radio interface
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA2560 baudrate setting.
 * \param baudrate The baudrate param from the ATMEGA2560 datasheet.
 * \param stopbits The number of stopbits.
 */
void usart3_init(unsigned int baudrate, unsigned char stopbits) {
	/* Set baud rate */
	UBRR3H = (unsigned char) (baudrate>>8);
	UBRR3L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity */
	UCSR3C = (1<<UCSZ31) | (1<<UCSZ30) | (0<<UCSZ32);
	/* Set the number of stopbits */
	if (stopbits == 1)
		UCSR3C |= (0<<USBS3);
	else if (stopbits == 2)
		UCSR3C |= (1<<USBS3);
	
	/* Enable receiver, transmitter and RX interrupt */
	UCSR3B = (1<<RXEN3) | (1<<TXEN3) | (1<<RXCIE3);
}

/*! \brief Send a character to the USART3
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
unsigned char usart3_transmit(char  data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR3A & (1<<UDRE3)));
	/* Put data into buffer, sends the data */
	UDR3 = data;
	return 0;
}

/*! \brief Send a string of characters to the USART3
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char usart3_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		usart3_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART3
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart3_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR3A & (1<<RXC3)));
	/* Get and return received data from buffer */
	return UDR3;
}

/*! \brief The USART3 recieve loopback
 * This function does wait for a character in the RX buffer and returns
 * it to the transmit buffer.
 * \return The character from the RX USART buffer
 */
unsigned char usart3_receive_loopback(void ) {
	uint8_t rbuff;
	/* Wait for data to be received */
	while (!(UCSR3A & (1<<RXC3)));
	/* Get and return received data from buffer */
	rbuff = UDR3;
	usart0_transmit(rbuff);
	return rbuff;
}

/*! \brief Retrieve one character from the USART3
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart3_receive(void ) {
	/* Check if data is received */
	return ((UCSR3A & (1<<RXC3)));
}
