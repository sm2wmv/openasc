/*! \file general_io/board.h
 *  \ingroup general_io_group
 *  \brief General I/O board defines 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-05-18
 *  \code #include "general_io/board.h" \endcode
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
 * PA0	-	Output - LED 6 (Status LED, Green)
 * PA1	-	Output - Driver 2
 * PA2	-	Output - Driver 3
 * PA3	- Output - Driver 4
 * PA4	-	Output - Driver 5
 * PA5	-	Output - Driver 6
 * PA6	-	Output - Driver 7
 * PA7	-	
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	Output - SPI (SS)
 * PB1	-	Output - SPI SCK (also ISP)
 * PB2	-	Output - SPI MOSI
 * PB3	-	Input  - SPI MISO
 * PB4	-	I/O - PB4
 * PB5	-	I/O - PB5
 * PB6	-	I/O - PB6
 * PB7	- I/O - PB7
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	Output - Relay K6
 * PC1	-	Output - Relay K5
 * PC2	-	Output - Relay K4
 * PC3	-	Output - Relay K3
 * PC4	-	Output - Relay K2
 * PC5	-	Output - Relay K1
 * PC6	-	Output - LED 5 (Status LED, Green)
 * PC7	-	Output - LED 4 (Status LED, Red)
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	I2C SCL
 * PD1	-	I2C SDA
 * PD2	-	BUS RXD
 * PD3	-	BUS TXD
 * PD4	-	I/O - PD4
 * PD5	-	I/O - PD5
 * PD6	-	I/O - PD6
 * PD7	-	I/O - PD7
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0	-	Input - MOSI (ISP)
 * PE1	-	Input -	MISO (ISP)
 * PE2	-	Input - Input External address input bit 0
 * PE3	-	Input - External address input bit 1
 * PE4	-	Input - External address input bit 2
 * PE5	-	Input - External address input bit 3
 * PE6	- Input - INT 6, interrupt input
 * PE7	-	Input - INT 7, interrupt input
 * ===================================================================================== *
 *
 *
 * PORTF
 * =====================================================================================
 * PF0	-	Input - ADC0
 * PF1	-	Input - ADC1
 * PF2	-	Input - ADC2
 * PF3	- Input - ADC3
 * PF4	-	Input - ADC4
 * PF5	-	Input - ADC5
 * PF6	-	Input - ADC6
 * PF7	-	Input - ADC7
 * ===================================================================================== * *
 *
 *
 * PORTG
 * =====================================================================================
 * PG0	- I/O - PG0
 * PG1	- I/O - PG1
 * PG2	- I/O - PG2
 * PG3	-	Input - 32 kHz external oscillator
 * PG4	- Input - 32 kHz external oscillator
 * ==================================================================================== * *
 */
 
#endif
