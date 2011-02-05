/*! \file 4cx1500b/ctrl_board/board.h  
 *  \ingroup ctrl_board_group
 *  \brief CTRL board board defines
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/init.h" \endcode
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
 * PA0 - I/O - Digital I/O 8
 * PA1 - I/O - Digital I/O 7
 * PA2 - I/O - Digital I/O 6
 * PA3 - I/O - Digital I/O 5
 * PA4 - I/O - Digital I/O 4
 * PA5 - I/O - Digital I/O 3
 * PA6 - I/O - Digital I/O 2
 * PA7 - I/O - Digital I/O 1
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0 - Output - Driver 6 PHASE
 * PB1 - Output - SPI SCK (ISP)
 * PB2 - Output - SPI MOSI (ISP)
 * PB3 - Input  - SPI MISO (ISP)
 * PB4 - Output - Driver 6 D2
 * PB5 - Output - Driver 3 D2
 * PB6 - Output - Driver 3 Phase
 * PB7 - Output - Driver 3 D1
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0 - Output - 
 * PC1 - Output - 
 * PC2 - Output - Driver output #14
 * PC3 - Output - Driver output #13
 * PC4 - Output - Driver output #12
 * PC5 - Output - Driver output #11
 * PC6 - Output - Driver output #10
 * PC7 - Output - Driver output #9
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0 - I/O    - I2C SCL
 * PD1 - I/O    - I2C SDA
 * PD2 - 
 * PD3 - 
 * PD4 - Output - Driver 2 Phase
 * PD5 - Output - Driver 2 D2
 * PD6 - I/O    - Digital I/O
 * PD7 - I/O    - Digital I/O
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0 - Input  - USB RXD
 * PE1 - Output - USB TXD
 * PE2 - I/O    - Digital I/O
 * PE3 - I/O    - Digital I/O
 * PE4 - I/O    - Digital I/O
 * PE5 - I/O    - Digital I/O
 * PE6 - I/O    - Digital I/O
 * PE7 - Output - Driver 5 D2
 * =====================================================================================
 *
 *
 * PORTF
 * =====================================================================================
 * PF0 - Input  - Analog input #1
 * PF1 - Input  - Analog input #2
 * PF2 - Input  - Analog input #3
 * PF3 - Input  - Analog input #4
 * PF4 - Input  - Analog input #5
 * PF5 - Input  - Analog input #6
 * PF6 - Input  - Analog input #7
 * PF7 - Input  - Analog input #8
 * =====================================================================================
 *
 *
 * PORTG
 * =====================================================================================
 * PG0 - I/O    - Digital I/O
 * PG1 - I/O    - Digital I/O
 * PG2 - Output - Driver output #1
 * PG3 - Output - Driver 3 D0
 * PG4 - Output - Driver 4 D1
 * PG5 - I/O    - Digital I/O
 * =====================================================================================
 *
 *
 * PORTH
 * =====================================================================================
 * PH0 - Input  - BUS RXD (RXD 2)
 * PH1 - Output - BUS TXD	(TXD 2)
 * PH2 - Output - Driver 5 Phase
 * PH3 - Output - Driver 5 D1
 * PH4 - Output - Driver 6 D0
 * PH5 - Output - Driver 5 D0
 * PH6 - Output - Driver 6 D1
 * PH7 - Output - Driver 4 D0
 * =====================================================================================
 *
 *
 * PORTJ
 * =====================================================================================
 * PJ0 - Output - Driver output #8
 * PJ1 - Output - Driver output #7
 * PJ2 - Output - Driver output #6
 * PJ3 - Output - Driver output #5
 * PJ4 - Output - Driver output #4
 * PJ5 - Output - Driver output #3
 * PJ6 - Output - Driver output #2
 * PJ7 - 
 * =====================================================================================
 *
 *
 * PORTK
 * =====================================================================================
 * PK0 - Input  - Analog input #9
 * PK1 - Input  - Analog input #10
 * PK2 - Input  - Analog input #11
 * PK3 - Input  - Analog input #12
 * PK4 - Input  - Analog input #13
 * PK5 - Input  - Analog input #14
 * PK6 - Input  - Analog input #15
 * PK7 - Input  - Analog input #16
 * =====================================================================================
 *
 *
 * PORTL
 * =====================================================================================
 * PL0 - Output - Driver 4 Phase
 * PL1 - Output - Driver 4 D2
 * PL2 - Output - Driver 1 D2
 * PL3 - Output - Driver 1 Phase
 * PL4 - Output - Driver 1 D1
 * PL5 - Output - Driver 2 D0
 * PL6 - Output - Driver 1 D0
 * PL7 - Output - Driver 2 D1
 * =====================================================================================
 *
 *
 */

#endif
