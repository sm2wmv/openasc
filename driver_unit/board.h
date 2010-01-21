/*! \file board.h
 *  \brief Driver unit board defines 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2008-04-06
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
 * PA0	-	Driver output #16
 * PA1	-	Driver output #15
 * PA2	-	Driver output #14
 * PA3	-	Driver output #13
 * PA4	-	Driver output #12
 * PA5	-	Driver output #11
 * PA6	-	Driver output #10
 * PA7	-	Driver output #9
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0	-	Driver output #17
 * PB1	-	Driver output #18
 * PB2	-	Driver output #19
 * PB3	-	Driver output #20
 * PB4	-	NC
 * PB5	-	SPI MOSI (ISP)
 * PB6	-	SPI MISO (ISP)
 * PB7	-	SPI SCK (ISP)
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0	-	SCL (I2C) for temp sensor, pulled up with 4k7 to VCC
 * PC1	-	SDA (I2C) for temp sensor, pulled up with 4k7 to VCC
 * PC2	-	Driver output #3
 * PC3	-	Driver output #4
 * PC4	-	Driver output #5
 * PC5	-	Driver output #6
 * PC6	-	Driver output #7
 * PC7	-	Driver output #8
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0	-	USART RXD input, used for the communication bus
 * PD1	-	USART TXD output, used for the communication bus
 * PD2	-	Driver output #1
 * PD3	-	Driver output #2
 * PD4	-	External address input bit 0
 * PD5	-	External address input bit 1
 * PD6	-	External address input bit 2
 * PD7	-	External address input bit 3
 * =====================================================================================
 *
 *
 */
 
 /*! Flag to indicate if the TX/RX mode is enabled */
 #define FLAG_TXRX_MODE_ENABLED	0
 
/*!Structure of the driver output status. It contains information of which address
 * an output was activated/deactivated from last and also it's status */
typedef struct {
	//! The address of the device that last changed the status of the output */
	unsigned char driver_output_owner[20];	//The address of the last device to change the status
	//! The type of message that activated the output
	unsigned char driver_output_type[20];	
	//! The state of the driver output if it's high or low
	unsigned long driver_output_state;
	//! Flags
	unsigned char flags;
} driver_status_struct;

//! Address input port */
#define ADDRESS_PORT	PORTD
//! Address input BIT 0 port offset */
#define ADDRESS_BIT0	4
//! Address input BIT 1 port offset */
#define ADDRESS_BIT1	5
//! Address input BIT 2 port offset */
#define ADDRESS_BIT2	6
//! Address input BIT 3 port offset */
#define ADDRESS_BIT3	7

//! Driver output 1 port offset
#define DRIVER_OUTPUT_1		2
//! Driver output 2 port offset
#define DRIVER_OUTPUT_2		3
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
//! Driver output 13 port offset
#define DRIVER_OUTPUT_13	3
//! Driver output 14 port offset
#define DRIVER_OUTPUT_14	2
//! Driver output 15 port offset
#define DRIVER_OUTPUT_15	1
//! Driver output 16 port offset
#define DRIVER_OUTPUT_16	0
//! Driver output 17 port offset
#define DRIVER_OUTPUT_17	0
//! Driver output 18 port offset
#define DRIVER_OUTPUT_18	1
//! Driver output 19 port offset
#define DRIVER_OUTPUT_19	2
//! Driver output 20 port offset
#define DRIVER_OUTPUT_20	3

//! Driver status for output OFF
#define DRIVER_STATUS_OFF	0
//! Driver status for output ON
#define DRIVER_STATUS_ON	1

#endif
