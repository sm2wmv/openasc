/*! \file board.h
 *  \brief Driver unit board defines 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2009-03-16
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
 * All the driver outputs are used as high for the output to be activated and
 * low to disable the output.
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
 * PA0	-	PTT input R4
 * PA1	-	PTT input R7
 * PA2	-	PTT input R3
 * PA3	-	PTT input R6
 * PA4	-	PTT input R2
 * PA5	-	PTT input R5
 * PA6	-	PTT input R1
 * PA7	-	Driver output #20
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	
 * PB1	-	ISP SCK
 * PB2	-	
 * PB3	-	
 * PB4	-	Driver output #1
 * PB5	-	Driver output #2
 * PB6	-	Driver output #3
 * PB7	-	Driver output #4
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	Driver output #11
 * PC1	-	Driver output #12
 * PC2	-	Driver output #13
 * PC3	-	Driver output #14
 * PC4	-	Driver output #15
 * PC5	-	Driver output #16
 * PC6	-	Driver output #17
 * PC7	-	Driver output #18
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	I2C SCL
 * PD1	-	I2C SDA
 * PD2	-	BUS RXD
 * PD3	-	BUS TXD
 * PD4	-	Driver output #7
 * PD5	-	Driver output #8
 * PD6	-	Driver output #9
 * PD7	-	Driver output #10
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	MOSI (ISP)
 * PE1	-	MISO (ISP)
 * PE2	-	External address input bit 0
 * PE3	-	External address input bit 1
 * PE4	-	External address input bit 2
 * PE5	-	External address input bit 3
 * PE6	-	Pos/neg sense input
 * PE7	-	
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	PTT LED R1
 * PF1	-	PTT LED R2
 * PF2	-	PTT LED R3
 * PF3	-	PTT LED R4
 * PF4	-	PTT LED R5
 * PF5	-	PTT LED R6
 * PF6	-	PTT LED R7
 * PF7	-	
 * ===================================================================================== * *
 *
 *
 * PORTG
 * =====================================================================================
 * PG0	-
 * PG1	-
 * PG2	- Driver output #19
 * PG3	-	Driver output #5
 * PG4	-	Driver output #6
 * ===================================================================================== * *
 */
 
#endif
