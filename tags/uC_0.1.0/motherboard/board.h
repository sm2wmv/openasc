/*! \file board.h \brief Motherboard defines 
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-06
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

/* This is the processor pinout and what is connected to each pin 
 * =====================================================================================
 * 
 * JP2 header pinout (ISP header)
 * =====================================================================================
 * This header can be used either to program the device (pin compatible with AVR ISP)
 * or you can use it to communicate with other hardware devices via SPI. I would suggest
 * that you use PB4 in that case as the chip select pin.
 *
 *  -----
 * o|1 2|
 *  |3 4|
 *  |5 6|
 *  -----
 *
 *  Pin 1 = SPI MISO
 *  Pin 2 = NC
 *  Pin 3 = SPI SCK
 *  Pin 4 = SPI MOSI
 *  Pin 5 = uC RESET
 *  Pin 6 = GND
 * =====================================================================================
 *
 * 
 * PORTA
 * =====================================================================================
 * PA0 - I/O    - X11 conn, pin 3 - AUX I/O
 * PA1 - I/O    - X11 conn, pin 8 - AUX I/O
 * PA2 - I/O    - X11 conn, pin 4 - AUX I/O
 * PA3 - Input  - PS/2 keyboard DATA
 * PA4 - Output - Driver 12
 * PA5 - Output - Driver 11
 * PA6 - Output - Driver 10
 * PA7 - Output - Driver 9
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0 - 
 * PB1 - Output - SPI SCK (ISP)
 * PB2 - Output - SPI MOSI (ISP)
 * PB3 - Input  - SPI MISO (ISP)
 * PB4 - Output - SPI CS
 * PB5 - 
 * PB6 - Input  - ON/OFF button
 * PB7 - Output - Relay for the power input
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0 - Output - Driver 1
 * PC1 - Output - Driver 2
 * PC2 - Output - Driver 3
 * PC3 - Output - Driver 4
 * PC4 - Output - Driver 5
 * PC5 - Output - Driver 6
 * PC6 - Output - Driver 7
 * PC7 - Output - Driver 8
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0 - I/O    - I2C SCL
 * PD1 - I/O    - I2C SDA
 * PD2 - Input  - USB RXD	(RXD 1)
 * PD3 - Output - USB TXD	(TXD 1)
 * PD4 - 
 * PD5 - I/O    - JP8 #3 - AUX
 * PD6 - I/O    - JP8 #2 - AUX
 * PD7 - I/O    - JP8 #1 - AUX
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0 - Input  - USART RXD (Communication between uCs)	(RXD 0)
 * PE1 - Output - USART TXD (Communication between uCs)	(TXD 0)
 * PE2 - 
 * PE3 - Output - JP10 conn, NMOS output
 * PE4 - I/O    - X11 conn, pin 2 - AUX input, INT4
 * PE5 - I/O    - X11 conn, pin 7	- AUX input, INT5
 * PE6 - Input  - PS/2 keyboard clock
 * PE7 - Input  - AUX input
 * =====================================================================================
 *
 *
 * PORTF
 * =====================================================================================
 * PF0 - Input  - BCD input Bit 2
 * PF1 - Input  - BCD input Bit 3
 * PF2 - Input  - BCD input Bit 0
 * PF3 - Input  - BCD input Bit 1
 * PF4 - Output - AUX relay #2 - X11 pin #9
 * PF5 - Output - AUX relay #1 - X11 pin #5
 * PF6 - 
 * PF7 - Input  - Upper floor button, bootloader mode
 * =====================================================================================
 *
 *
 * PORTG
 * =====================================================================================
 * PG0 - 
 * PG1 - 
 * PG2 - 
 * PG3 - Input  - 32.768kHz crystal
 * PG4 - Input  - 32.768kHz crystal
 * =====================================================================================
*/

#ifndef _BOARD_H_
#define _BOARD_H_

//! Driver output 1 port offset
#define DRIVER_OUTPUT_1		0
//! Driver output 2 port offset
#define DRIVER_OUTPUT_2		1
//! Driver output 3 port offset
#define DRIVER_OUTPUT_3		2
//! Driver output 4 port offset
#define DRIVER_OUTPUT_4		3
//! Driver output 5 port offset
#define DRIVER_OUTPUT_5		4
//! Driver output 6 port offset
#define DRIVER_OUTPUT_6		5
//! Driver output 7 port offset
#define DRIVER_OUTPUT_7		6
//! Driver output 8 port offset
#define DRIVER_OUTPUT_8		7
//! Driver output 9 port offset
#define DRIVER_OUTPUT_9		7
//! Driver output 10 port offset
#define DRIVER_OUTPUT_10	6
//! Driver output 11 port offset
#define DRIVER_OUTPUT_11	5
//! Driver output 12 port offset
#define DRIVER_OUTPUT_12	4

//! AUX pin #3 on the X11 connector
#define AUX_X11_PIN3			0
//! AUX pin #8 on the X11 connector
#define AUX_X11_PIN8			1
//! AUX pin #4 on the X11 connector
#define AUX_X11_PIN4			2
//! AUX pin #5 on the X11 connector, relay output (draws to either +12V or GND)
#define AUX_X11_PIN5			3
//! AUX pin #9 on the X11 connector, relay output (draws to either +12V or GND)
#define AUX_X11_PIN9			4

#endif
