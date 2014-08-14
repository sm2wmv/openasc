/*! \file motherboard/usart.c 
 *  \ingroup motherboard_group 
 *  \brief Motherboard USART routines
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "motherboard/usart.c" \endcode
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
unsigned char usart1_transmit(char  data );

/*! \brief Initiliaze the USART for the communication bus 
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA128 baudrate setting.
 * \param baud The baudrate param from the ATMEGA32 datasheet.
 */
void usart0_init(unsigned int baudrate) {
	/* Set baud rate */
	UBRR0H = (unsigned char) (baudrate>>8);
	UBRR0L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR0C = (0<<UMSEL0) | (0<<USBS0) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCSZ2);
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
}

/*! \brief Send a character to the USART
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
unsigned char usart0_transmit(char  data ) {
	/* Wait for empty transmit buffer */
	while (!( UCSR0A & (1<<UDRE0)));
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}

/*! \brief Send a string of characters to the USART
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

/*! \brief Retrieve one character from the USART
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

/*! \brief The USART recieve loopback
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

/*! \brief Retrieve one character from the USART
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart0_receive(void ) {
	/* Check if data is received */
	return ((UCSR0A & (1<<RXC0)));
}

/*! \brief Initiliaze the USART for the communication bus 
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the ATMEGA128 baudrate setting.
 * \param baudrate The baudrate param from the ATMEGA32 datasheet.
 */
void usart1_init(unsigned int baudrate) {
	/* Set baud rate */
	UBRR1H = (unsigned char) (baudrate>>8);
	UBRR1L = (unsigned char) baudrate;
	/* Set frame format: 8data, no parity & 1 stop bits */
	UCSR1C = (0<<UMSEL0) | (0<<USBS0) | (1<<UCSZ1) | (1<<UCSZ0) | (0<<UCSZ2);
	/* Enable receiver and transmitter */
	UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
}

/*! \brief Send a character to the USART
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

/*! \brief Send a string of characters to the USART
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

/*! \brief Retrieve one character from the USART
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char usart1_receive(void ) {
	/* Wait for data to be received */
	while (!(UCSR1A & (1<<RXC1)));
	/* Get and return received data from buffer */
	return UDR0;
}

/*! \brief The USART recieve loopback
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
	usart1_transmit(rbuff);
	return rbuff;
}

/*! \brief Retrieve one character from the USART
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char poll_usart1_receive(void ) {
	/* Check if data is received */
	return ((UCSR1A & (1<<RXC1)));
}
