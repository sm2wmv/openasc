/*! \file board.h \brief Board definitions
 * \author Mikael Larsmark, SM2WMV
 * \date 2009-06-07
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
 * PA0 - Output - LCD DATA PIN DB7
 * PA1 - Output - LCD DATA PIN DB6
 * PA2 - Output - LCD DATA PIN DB5
 * PA3 - Output - LCD DATA PIN DB4
 * PA4 - Output - LCD DATA PIN DB3
 * PA5 - Output - LCD DATA PIN DB2
 * PA6 - Output - LCD DATA PIN DB1
 * PA7 - Output - LCD DATA PIN DB0
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0 - Output - POWER LED #2
 * PB1 - Output - POWER LED #3
 * PB2 - Output - POWER LED #4
 * PB3 - Output - POWER LED #5
 * PB4 - Output - POWER LED #6
 * PB5 - Output - POWER LED #7
 * PB6 - Output - POWER LED #8
 * PB7 - Output - POWER LED #9
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0 - Output - Reflected POWER LED #2
 * PC1 - Output - Reflected POWER LED #3
 * PC2 - Output - Reflected POWER LED #4
 * PC3 - Output - Reflected POWER LED #5
 * PC4 - Output - Reflected POWER LED #6
 * PC5 - Output - LCD PIN RS
 * PC6 - Output - LCD PIN RW
 * PC7 - Output - LCD PIN ENABLE
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0 - I/O    - I2C SCL
 * PD1 - I/O    - I2C SDA
 * PD2 - Input  - USB RXD	(RXD 1)
 * PD3 - Output - USB TXD	(TXD 1)
 * PD4 - Input  - Divided frequency input
 * PD5 - Output - POWER LED #12
 * PD6 - Output - POWER LED #13
 * PD7 - Output - POWER LED #14
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0 - Input  - PDI
 * PE1 - Output - PDO
 * PE2 - Output - Reflected POWER LED #7
 * PE3 - Output - PWM output for display backlight (OC3A)
 * PE4 - Output - PWM output for display contrast  (OC3B)
 * PE5 - Input  - Pulse sensor pin #1
 * PE6 - Input  - Pulse sensor pin #2
 * PE7 - Output - POWER LED #1
 * =====================================================================================
 *
 *
 * PORTF
 * =====================================================================================
 * PF0 - Input  - A/D input, forward voltage (power)
 * PF1 - Input  - A/D input, reflected voltage
 * PF2 - Output - RELAY #1
 * PF3 - Output - Counter clear
 * PF4 - Output - RELAY #2
 * PF5 - Input  - Menu button
 * PF6 - 
 * PF7 - 
 * =====================================================================================
 *
 *
 * PORTG
 * =====================================================================================
 * PG0 - Output - POWER LED #15
 * PG1 - Output - Reflected POWER LED #1
 * PG2 - Output - VSWR warning LED
 * PG3 - Output - POWER LED #10
 * PG4 - Output - POWER LED #11
 * =====================================================================================
 *
 */

/* PORTE */
#define LED_FWD_POWER_BIT0	7
/* PORTB */
#define LED_FWD_POWER_BIT1	0
#define LED_FWD_POWER_BIT2	1
#define LED_FWD_POWER_BIT3	2
#define LED_FWD_POWER_BIT4	3
#define LED_FWD_POWER_BIT5	4
#define LED_FWD_POWER_BIT6	5
#define LED_FWD_POWER_BIT7	6
#define LED_FWD_POWER_BIT8	7
/* PORTG */
#define LED_FWD_POWER_BIT9	3
#define LED_FWD_POWER_BIT10	4
/* PORTD */
#define LED_FWD_POWER_BIT11	5
#define LED_FWD_POWER_BIT12	6
#define LED_FWD_POWER_BIT13	7
/* PORTG */
#define LED_FWD_POWER_BIT14	0

/* PORTG */
#define LED_REF_POWER_BIT0	1
/* PORTC */
#define LED_REF_POWER_BIT1	0
#define LED_REF_POWER_BIT2	1
#define LED_REF_POWER_BIT3	2
#define LED_REF_POWER_BIT4	3
#define LED_REF_POWER_BIT5	4
/* PORTE */
#define LED_REF_POWER_BIT6	2

/* PORTG */
#define LED_VSWR_ALARM_BIT	2

/* PORTF */
#define RELAY1_BIT	2
#define RELAY2_BIT	4



#endif
