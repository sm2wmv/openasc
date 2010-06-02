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
 * PA0	-	Input - AUX I/O - SV4 (10)
 * PA1	-	Input - AUX I/O - SV4 (9)
 * PA2	- Input - AUX I/O - SV4 (8)
 * PA3	-	Input - AUX I/O - SV4 (7)
 * PA4	-	Input - AUX I/O - SV4 (6)
 * PA5	-	Input - AUX I/O - SV4 (5)
 * PA6	-	Input - AUX I/O - SV4 (4)
 * PA7	-	Input - AUX I/O - SV4 (3)
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-
 * PB1	-	ISP SCK
 * PB2	-
 * PB3	-
 * PB4	-
 * PB5	-
 * PB6	-
 * PB7	-
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	Output - NMOS output #3
 * PC1	-	Output - NMOS output #4
 * PC2	-	Output - NMOS output #1
 * PC3	-	Output - NMOS output #2
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
 * PD2	-	Input  - BUS RXD
 * PD3	-	Output - BUS TXD
 * PD4	-	Output - Relay #3
 * PD5	-	Output - Relay #4
 * PD6	-	Output - Relay #2
 * PD7	-	Output - Relay #1
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	MOSI (ISP)
 * PE1	-	MISO (ISP)
 * PE2	- Input -	External address bit 0
 * PE3	-	Input - External address bit 1
 * PE4	-	Input - External address bit 2
 * PE5	-	Input - External address bit 3
 * PE6	-	Input - Pulse sensor input #1
 * PE7	-	Input - Pulse sensor input #2
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	Input - Potentiometer input #1 (A/D)
 * PF1	-	Input - Potentiometer input #2 (A/D)
 * PF2	-	Input - DC Voltage sense (Vin = 13.8V -> PF2 voltage = 3.872V)
 * PF3	-	
 * PF4	-	
 * PF5	-	
 * PF6	-	
 * PF7	-
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

#define FET1_BIT	2
#define FET2_BIT	3
#define FET3_BIT  0
#define FET4_BIT  1

#endif
