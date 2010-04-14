/*! \file rotator_unit/board.h
 *  \brief Defines for the rotator board
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2009-04-22
 */
//    Copyright (C) 2009  Mikael Larsmark, SM2WMV
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

#ifndef _BOARD_H_
#define _BOARD_H_

/* This is the processor pinout and what is connected to each pin 
 * =====================================================================================
 * The external address input has got external pull up resistor which makes the 
 * address inverted. So if one bit is zero it means that the DIP is actually
 * switched to its on position.
 * =====================================================================================
 * 
 * JP1 header pinout (ISP header)
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
 * PA0	-	AUX I/O - SV4 (10)
 * PA1	-	AUX I/O - SV4 (9)
 * PA2	- AUX I/O - SV4 (8)
 * PA3	-	AUX I/O - SV4 (7)
 * PA4	-	AUX I/O - SV4 (6)
 * PA5	-	AUX I/O - SV4 (5)
 * PA6	-	AUX I/O - SV4 (4)
 * PA7	-	AUX I/O - SV4 (3)
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	
 * PB1	-	ISP SCK
 * PB2	-	
 * PB3	-	
 * PB4	-	External address input bit 0
 * PB5	-	External address input bit 1
 * PB6	-	External address input bit 2
 * PB7	-	External address input bit 3
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	NMOS output #1
 * PC1	-	NMOS output #2
 * PC2	-	
 * PC3	-	
 * PC4	-	
 * PC5	-	
 * PC6	-	
 * PC7	-	
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	
 * PD1	-	
 * PD2	-	BUS RXD
 * PD3	-	BUS TXD
 * PD4	-	Relay #3
 * PD5	-	Relay #4
 * PD6	-	Relay #2
 * PD7	-	Relay #1
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	MOSI (ISP)
 * PE1	-	MISO (ISP)
 * PE2	-	
 * PE3	-	
 * PE4	-	
 * PE5	-	
 * PE6	-	Pulse sensor input #1
 * PE7	-	Pulse sensor input #2
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	Potentiometer input #1 (A/D)
 * PF1	-	Potentiometer input #2 (A/D)
 * PF2	-	
 * PF3	-	
 * PF4	-	
 * PF5	-	
 * PF6	-	
 * PF7	-	A/D input for sensing voltage dropoff (for saving data before shutdown)
 * ===================================================================================== * *
 *
 *
 * PORTG
 * =====================================================================================
 * PG0	-
 * PG1	-
 * PG2	- 
 * PG3	-	
 * PG4	-	
 * ===================================================================================== * *
 */
 
#define RELAY1_BIT 7
#define RELAY2_BIT 6
#define RELAY3_BIT 4
#define RELAY4_BIT 5

#define FET1_BIT	0
#define FET2_BIT	1
 
#endif
