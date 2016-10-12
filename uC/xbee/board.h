/*! \file xbee/board.h
 *  \ingroup xbee_group
 *  \brief XBEE interface board defines 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2016-10-12
 *  \code #include "xbee/board.h" \endcode
 */
//    Copyright (C) 2016 Mikael Larsmark, SM2WMV
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
 * PA0	- Input - NC
 * PA1	- Input - NC
 * PA2	- Input - TP1
 * PA3	- Input - TP2
 * PA4	- Input - TP3
 * PA5	- Input - TP4
 * PA6	- Input - NC
 * PA7	- Input - NC
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	Input - NC
 * PB1	-	Input - ISP SCK
 * PB2	-	Input - NC
 * PB3	-	Input - NC
 * PB4	-	Input - NC
 * PB5	-	Input - TP5
 * PB6	-	Input - NC
 * PB7	-	Input - NC
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	Input - NC
 * PC1	-	Input - NC
 * PC2	-	Input - NC
 * PC3	-	Input - NC
 * PC4	-	Input - NC
 * PC5	-	Input - NC
 * PC6	-	Input - NC
 * PC7	-	Input - NC
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	Input  - TP11
 * PD1	-	Input  - TP12
 * PD2	-	Input  - BUS RXD
 * PD3	-	Output - BUS TXD
 * PD4	-	Input  - NC
 * PD5	-	Input  - NC
 * PD6	-	Input  - NC
 * PD7	-	Input  - NC
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	Input  - XBEE TXD
 * PE1	-	Output - XBEE RXD
 * PE2	-	Input  - External address input bit 0
 * PE3	-	Input  - External address input bit 1
 * PE4	-	Input  - External address input bit 2
 * PE5	-	Input  - External address input bit 3
 * PE6	-	Input  - TP8
 * PE7	-	Input  - TP7
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	Input - NC
 * PF1	-	Input - NC
 * PF2	-	Input - NC
 * PF3	-	Input - NC
 * PF4	-	Input - NC
 * PF5	-	Input - NC
 * PF6	-	Input - NC
 * PF7	-	Input - NC
 * ===================================================================================== * *
 *
 *
 * PORTG
 * =====================================================================================
 * PG0	-	Input - NC
 * PG1	-	Input - NC
 * PG2	- 	Input - NC
 * PG3	-	Input - NC
 * PG4	-	Input - NC
 * ===================================================================================== * *
 */
 
#endif
