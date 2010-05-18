/*! \file wmv_bus/bus_usart.c \brief Driver unit USART routines.
 *  \ingroup bus_group 
 *  \brief These routines are used to communicate over the WMV bus.
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus_usart.c" \endcode
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

/*! \brief Initiliaze the USART for the communication bus 
 * This function is used to initialize the USART which a baudrate
 * that needs to be sent in as a parameter Use the baudrate settings
 * specified in the uCs datasheet.
 * \param baudrate The baudrate param from the ATMEGA32 datasheet.
 */
void bus_usart_init(unsigned int baudrate) {
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		/* Set baud rate */
		UBRRH = (unsigned char) (baudrate>>8);
		UBRRL = (unsigned char) baudrate;

		UCSRA = 0;

		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSRC = (1<< URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);
		/* Enable receiver and transmitter */
		UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) | (1<<TXCIE);
	#endif
	
	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		/* Set baud rate */
		UBRRH = (unsigned char) (baudrate>>8);
		UBRRL = (unsigned char) baudrate;

		UCSRA = 0;

		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSRC = (1<< URSEL) | (0<<UMSEL) | (0<<UPM1) | (0<<UPM0) | (0<<USBS) | (1<<UCSZ1) | (1<<UCSZ0);
		/* Enable receiver and transmitter */
		UCSRB = (1<<RXEN) | (1<<TXEN) | (1<<RXCIE) | (1<<TXCIE);
	#endif

		
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		/* Set baud rate */
		UBRR1H = (unsigned char) (baudrate>>8);
		UBRR1L = (unsigned char) baudrate;

		UCSR1A = 0;

		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCSZ12);
		/* Enable receiver and transmitter */
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<TXCIE1);
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		/* Set baud rate */
		UBRR1H = (unsigned char) (baudrate>>8);
		UBRR1L = (unsigned char) baudrate;

		UCSR1A = 0;

		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCSZ12);
		/* Enable receiver and transmitter */
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<TXCIE1);
	#endif

	#ifdef DEVICE_TYPE_ROTATOR_UNIT
		/* Set baud rate */
		UBRR1H = (unsigned char) (baudrate>>8);
		UBRR1L = (unsigned char) baudrate;

		UCSR1A = 0;

		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSR1C = (1<<UCSZ11) | (1<<UCSZ10) | (0<<UCSZ12);
		/* Enable receiver and transmitter */
		UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1) | (1<<TXCIE1);
	#endif
		
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		/* Set baud rate */
		UBRR2H = (unsigned char) (baudrate>>8);
		UBRR2L = (unsigned char) baudrate;
		/* Set frame format: 8data, no parity & 1 stop bits */
		UCSR2C = (1<<UCSZ21) | (1<<UCSZ20) | (0<<UCSZ22);
		/* Enable receiver and transmitter */
		UCSR2B = (1<<RXEN2) | (1<<TXEN2) | (1<<RXCIE2) | (1<<TXCIE2);
	#endif
}

/*! \brief Send a character to the USART
 * Send a single character to the USART used for the communication bus
 * \param data The character you want to send
 */
unsigned char bus_usart_transmit(unsigned char  data) {
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		/* Wait for empty transmit buffer */
		while (!( UCSRA & (1<<UDRE)));
		/* Put data into buffer, sends the data */
		UDR = data;
	#endif

	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		/* Wait for empty transmit buffer */
		while (!( UCSRA & (1<<UDRE)));
		/* Put data into buffer, sends the data */
		UDR = data;
	#endif
		
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		/* Wait for empty transmit buffer */
		while (!( UCSR1A & (1<<UDRE1)));
		/* Put data into buffer, sends the data */
		UDR1 = data;
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		/* Wait for empty transmit buffer */
		while (!( UCSR1A & (1<<UDRE1)));
		/* Put data into buffer, sends the data */
		UDR1 = data;
	#endif


	#ifdef DEVICE_TYPE_ROTATOR_UNIT
		/* Wait for empty transmit buffer */
		while (!( UCSR1A & (1<<UDRE1)));
		/* Put data into buffer, sends the data */
		UDR1 = data;
	#endif
		
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		/* Wait for empty transmit buffer */
		while (!( UCSR2A & (1<<UDRE2)));
		/* Put data into buffer, sends the data */
		UDR2 = data;
	#endif
	
	return 0;
}

/*! \brief Send a string of characters to the USART
 * Send a string of characters to the USART used for the communication bus
 * \param data The string of characters you wish to send
 * \param length The length of the string you wish to send
 */
unsigned char bus_usart_sendstring(char *data,unsigned char length) {
	int i;
	for (i=0;i<length;i++)
		bus_usart_transmit(*(data++));
	
	return 0;
}

/*! \brief Retrieve one character from the USART
 * Retrieve one character from the USART. This function will wait until
 * there is a character in the USART RX buffer
 * \return The character from the RX USART buffer
 */
unsigned char bus_usart_receive(void ) {
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		/* Get and return received data from buffer */
		return UDR;
	#endif

	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		/* Get and return received data from buffer */
		return UDR;
	#endif
		
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		return UDR1;
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		return UDR1;
	#endif
		
	#ifdef DEVICE_TYPE_ROTATOR_UNIT
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		return UDR1;
	#endif		

	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		/* Wait for data to be received */
		while (!(UCSR2A & (1<<RXC2)));
		/* Get and return received data from buffer */
		return UDR2;
	#endif
		
	return(0);
}

/*! \brief The USART recieve loopback
 * This function does wait for a character in the RX buffer and returns
 * it to the transmit buffer.
 * \return The character from the RX USART buffer
 */
unsigned char bus_usart_receive_loopback(void ) {
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		/* Get and return received data from buffer */
		rbuff = UDR;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif

	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSRA & (1<<RXC)));
		/* Get and return received data from buffer */
		rbuff = UDR;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif
		
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		rbuff = UDR1;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		rbuff = UDR1;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif

	#ifdef DEVICE_TYPE_ROTATOR_UNIT
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSR1A & (1<<RXC1)));
		/* Get and return received data from buffer */
		rbuff = UDR1;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif
		
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		uint8_t rbuff;
		/* Wait for data to be received */
		while (!(UCSR2A & (1<<RXC2)));
		/* Get and return received data from buffer */
		rbuff = UDR2;
		bus_usart_transmit(rbuff);
		return rbuff;
	#endif

	return(0);
}

/*! \brief Retrieve one character from the USART
 * Retrieve one character from the USART. With this function you will
 * need to poll the USART, it does NOT wait until a character is in the buffer.
 * \return The character from the RX USART buffer
 */
unsigned char bus_poll_usart_receive(void ) {
	#ifdef DEVICE_TYPE_DRIVER_UNIT
		/* Check if data is received */
		return ((UCSRA & (1<<RXC)));
	#endif

	#ifdef DEVICE_TYPE_POWERMETER_PICKUP
		/* Check if data is received */
		return ((UCSRA & (1<<RXC)));
	#endif
		
	#ifdef DEVICE_TYPE_DRIVER_UNIT_V2
		/* Check if data is received */
		return ((UCSR1A & (1<<RXC1)));
	#endif

	#ifdef DEVICE_TYPE_GENERAL_IO
		/* Check if data is received */
		return ((UCSR1A & (1<<RXC1)));
	#endif

	#ifdef DEVICE_TYPE_ROTATOR_UNIT
		/* Check if data is received */
		return ((UCSR1A & (1<<RXC1)));
	#endif		
		
	#ifdef DEVICE_TYPE_MAIN_FRONT_UNIT
		/* Check if data is received */
		return ((UCSR2A & (1<<RXC2)));
	#endif
		
	return (0);
}
