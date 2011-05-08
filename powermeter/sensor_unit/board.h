/*! \file board.h \brief Board definitions
 * \author Mikael Larsmark, SM2WMV
 * \date 2010-03-24
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

#ifndef _BOARD_H_
#define _BOARD_H_

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
 * PA0 - Input  - Forward power voltage level (A/D)
 * PA1 - Input  - Reflected power voltage level (A/D)
 * PA2 - Output - NC
 * PA3 - Output - NC
 * PA4 - Output - NC
 * PA5 - Output - NC
 * PA6 - Output - NC
 * PA7 - Output - NC
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0 - Output - NC
 * PB1 - Output - NC
 * PB2 - Output - NC
 * PB3 - Output - NC
 * PB4 - Output - NC
 * PB5 - Input  - MOSI (ISP)
 * PB6 - Input  - MISO(ISP)
 * PB7 - Input  - SCK (ISP)
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0 - Output - NC
 * PC1 - Output - NC
 * PC2 - Input  - External address BIT0
 * PC3 - Input  - External address BIT1
 * PC4 - Input  - External address BIT2
 * PC5 - Input  - External address BIT3
 * PC6 - Input  - 32.768kHz clock
 * PC7 - Input  - 32.768kHz clock
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0 - Input  - BUS RXD (USART)
 * PD1 - Output - BUS TXD (USART)
 * PD2 - Output - NC
 * PD3 - Input  - Configuration bit 0
 * PD4 - Input  - Configuration bit 1
 * PD5 - Input  - Configuration bit 2
 * PD6 - Input  - Freq div output (freq counter input, ICP)
 * PD7 - Output - Counter clear (freq counter)
 * =====================================================================================
 */


#endif
