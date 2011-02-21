/*! \file front_panel/board.h 
 *  \ingroup front_panel_group
 *  \brief Front panel board defines
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/board.h" \endcode
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
 * PA0 - Output - LED 12M
 * PA1 - Output - LED 15M
 * PA2 - Output - LED 17M
 * PA3 - Output - LED 20M
 * PA4 - Output - LED 30M
 * PA5 - Output - LED 40M
 * PA6 - Output - LED 80M
 * PA7 - Output - LED 160M
 * =====================================================================================
 *
 *
 * PORTB
 * =====================================================================================
 * PB0 - Output/Input - AUX X11 PIN 8
 * PB1 - Output - SPI SCK (ISP)
 * PB2 - Output - SPI MOSI (ISP)
 * PB3 - Input  - SPI MISO (ISP)
 * PB4 - Output - LED Button 3 RX
 * PB5 - Input  - Button 4 RX
 * PB6 - Output - LED Button 4 RX
 * PB7 - Output - PWM backlight
 * =====================================================================================
 *
 *
 * PORTC
 * =====================================================================================
 * PC0 - Output - LED PTT Red
 * PC1 - Output - LED PTT Green
 * PC2 - Output - LED Button RX/TX Mode
 * PC3 - Input  - Button RX/TX Mode
 * PC4 - Output - LED Button Rotate
 * PC5 - Input  - Button Rotate
 * PC6 - Output - LED Rotation
 * PC7 - Output - LED AUX
 * =====================================================================================
 *
 *
 * PORTD
 * =====================================================================================
 * PD0 - I/O    - I2C SCL
 * PD1 - I/O    - I2C SDA
 * PD2 - Input  - USB RXD	(RXD 1)
 * PD3 - Output - USB TXD	(TXD 1)
 * PD4 - Output - LED Button AUX
 * PD5 - Input  - Button SUB
 * PD6 - Output - LED Button RX ANT 2
 * PD7 - Input  - Button RX ANT 2
 * =====================================================================================
 *
 *
 * PORTE
 * =====================================================================================
 * PE0 - Input  - USART RXD (Communication between uCs)	(RXD 0)
 * PE1 - Output - USART TXD (Communication between uCs)	(TXD 0)
 * PE2 - Output - RELAY #1 AMP
 * PE3 - Input  - Footswitch (Active Low)
 * PE4 - Input  - Radio sense (Active Low)
 * PE5 - Output - Radio inhibit
 * PE6 - Input  - Pulse sensor pin #2 (INT6)
 * PE7 - Input  - Pulse sensor pin #1 (INT7)
 * =====================================================================================
 *
 *
 * PORTF
 * =====================================================================================
 * PF0 - I/O    - LCD DB0
 * PF1 - I/O    - LCD DB1
 * PF2 - I/O    - LCD DB2
 * PF3 - I/O    - LCD DB3
 * PF4 - I/O    - LCD DB4
 * PF5 - I/O    - LCD DB5
 * PF6 - I/O    - LCD DB6
 * PF7 - I/O    - LCD DB7
 * =====================================================================================
 *
 *
 * PORTG
 * =====================================================================================
 * PG0 - Input  - AUX Button 1
 * PG1 - Input  - AUX Button 2
 * PG2 - Input  - Rotary button
 * PG3 - Input  - 32.768kHz crystal
 * PG4 - Input  - 32.768kHz crystal
 * PG5 - Output - RELAY PTT RADIO
 * =====================================================================================
 *
 *
 * PORTH
 * =====================================================================================
 * PH0 - Input  - BUS RXD (RXD 2)
 * PH1 - Output - BUS TXD	(TXD 2)
 * PH2 - Input  - Button 1 RX
 * PH3 - Output - LED 1 RX
 * PH4 - Input  - Button 2 RX
 * PH5 - Output - LED 2 RX
 * PH6 - Input  - Button 3 RX
 * PH7 - Output - LED 12
 * =====================================================================================
 *
 *
 * PORTJ
 * =====================================================================================
 * PJ0 - Input  - Radio RXD (RXD 3)
 * PJ1 - Output - Radio TXD	(TXD 3)
 * PJ2 - Input  - TX radio sense (upper floor)
 * PJ3 - Output - PTT radio
 * PJ4 - Input  - USB1 DTR
 * PJ5 - Input  - USB2 DTR
 * PJ6 - Input  - USB2 RTS
 * PJ7 - Output - LED 10M
 * =====================================================================================
 *
 *
 * PORTK
 * =====================================================================================
 * PK0 - Output - LCD CS1
 * PK1 - Output - LCD CS2
 * PK2 - Output - LCD RESET
 * PK3 - Output - LCD RW
 * PK4 - Output - LCD DI
 * PK5 - Output - LCD ENABLE
 * PK6 - Input  - Button Menu
 * PK7 - Output - LED Button Menu
 * =====================================================================================
 *
 *
 * PORTL
 * =====================================================================================
 * PL0 - Input  - Button 4 TX
 * PL1 - Output - LED Button 4 TX
 * PL2 - Input  - Button 3 TX
 * PL3 - Output - LED Button 3 TX
 * PL4 - Input  - Button 2 TX
 * PL5 - Output - LED Button 2 TX
 * PL6 - Input  - Button 1 TX
 * PL7 - Output - LED Button 1 TX
 * =====================================================================================
 *
 *
 */

//LEDs
//! Bit offset of TX button 1 LED
#define LED_TX_BUTTON1_BIT	7
//! Bit offset of TX button 2 LED
#define LED_TX_BUTTON2_BIT	5
//! Bit offset of TX button 3 LED
#define LED_TX_BUTTON3_BIT	3
//! Bit offset of TX button 4 LED
#define LED_TX_BUTTON4_BIT	1

//! Bit offset of Error LED
#define LED_ERROR_BIT				7
//! Bit offset of PTT Green LED
#define LED_PTT_GREEN_BIT		0
//! Bit offset of PTT Red LED
#define LED_PTT_RED_BIT			1
//! Bit offset of rotation active LED
#define LED_ROTATION_ACTIVE_BIT		6

//! Bit offset of RX button 1 LED
#define LED_RX_BUTTON1_BIT	3
//! Bit offset of RX button 2 LED
#define LED_RX_BUTTON2_BIT	5
//! Bit offset of RX button 3 LED
#define LED_RX_BUTTON3_BIT	4
//! Bit offset of RX button 4 LED
#define LED_RX_BUTTON4_BIT	6

//! Bit offset of LED rotate
#define LED_ROTATE_BIT			4
//! Bit offset of LED TX/RX mode
#define LED_TXRX_BIT				2
//! Bit offset of LED RX ANTENNA
#define LED_RXANT_BIT				6
//! Bit offset of LED SUB MENU
#define LED_SUBMENU_BIT			4
//! bit offset of LED MENU
#define LED_MENU_BIT				7
//! Bit offset of LED for MENU system
#define LED_AUX_BIT					7

//Button and knobs
//! Bit offset of the pulse sensor
#define PULSE_SENSOR_BIT1		6
//! Bit offset of the pulse sensor
#define PULSE_SENSOR_BIT2		7

//! Bit offset of TX button 1
#define BUTTON1_TX_BIT			6
//! Bit offset of TX button 2
#define BUTTON2_TX_BIT			4
//! Bit offset of TX button 3
#define BUTTON3_TX_BIT			2
//! Bit offset of TX button 4
#define BUTTON4_TX_BIT			0

//! Bit offset of RX button 1
#define BUTTON1_RX_BIT			2
//! Bit offset of RX button 2
#define BUTTON2_RX_BIT			4
//! Bit offset of RX button 3
#define BUTTON3_RX_BIT			6
//! Bit offset of RX button 4
#define BUTTON4_RX_BIT			5

//! Bit offset of rotate button
#define BUTTON_ROTATE_BIT		5
//! Bit offset of TX/RX mode button
#define BUTTON_TXRX_BIT			3
//! Bit offset of RX Antenna button
#define BUTTON_RXANT_BIT		7
//! Bit offset of SUB MENU button
#define BUTTON_SUBMENU_BIT	5
//! Bit offset of MENU button
#define BUTTON_MENU_BIT			6
//! Bit offset of PULSE SENSOR button
#define BUTTON_PULSE_BIT		2
//! Bit offset of AUX 1 button
#define BUTTON_AUX1_BIT			0
//! Bit offset of AUX 2 button
#define BUTTON_AUX2_BIT			1

//Ext devices
//! Bit offset of the EXT Radio sense 1
#define	EXT_RADIO_SENSE1_BIT	4
//! Bit offset of the EXT Radio sense 2
#define	EXT_RADIO_SENSE2_BIT	2
//! Bit offset of the footswitch
#define	EXT_FOOTSWITCH_BIT		3
//! Bit offset of the USB 1 DTR
#define EXT_USB1_DTR_BIT			4
//! Bit offset of the USB 2 DTR
#define EXT_USB2_DTR_BIT			5
//! Bit offset of the USB 2 RTS
#define EXT_USB2_RTS_BIT			6

//! Bit offset of the amplifier output
#define AMPLIFIER_OUTPUT_BIT	2

//! Bit offset of the tx active output
#define TX_ACTIVE_OUTPUT_BIT	5

//! Bit offset of the inhibit output
#define RADIO_INHIBIT_OUTPUT_BIT 5

//! Flag is set if the TX ANTENNA #1 button is pressed
#define FLAG_BUTTON1_TX_BIT			0
//! Flag is set if the TX ANTENNA #2 button is pressed
#define FLAG_BUTTON2_TX_BIT			1
//! Flag is set if the TX ANTENNA #3 button is pressed
#define FLAG_BUTTON3_TX_BIT			2
//! Flag is set if the TX ANTENNA #4 button is pressed
#define FLAG_BUTTON4_TX_BIT			3
//! Flag is set if the RX ANTENNA #1 button is pressed
#define FLAG_BUTTON1_RX_BIT			4
//! Flag is set if the RX ANTENNA #2 button is pressed
#define FLAG_BUTTON2_RX_BIT			5
//! Flag is set if the RX ANTENNA #3 button is pressed
#define FLAG_BUTTON3_RX_BIT			6
//! Flag is set if the RX ANTENNA #4 button is pressed
#define FLAG_BUTTON4_RX_BIT			7
//! Flag is set if the menu button is pressed
#define FLAG_BUTTON_MENU_BIT		8
//! Flag is set if the Rotate button is pressed
#define FLAG_BUTTON_ROTATE_BIT	9
//! Flag is set if the TX/RX mode button is pressed
#define FLAG_BUTTON_TXRX_BIT		10
//! Flag is set if the RX ANTENNA button is pressed
#define FLAG_BUTTON_RXANT_BIT		11
//! Flag is set if the AUX button is pressed
#define FLAG_BUTTON_SUBMENU_BIT			12
//! Flag is set if the pulse sensor button is pressed
#define FLAG_BUTTON_PULSE_BIT		13
//! Flag is set if the AUX button 1 is pressed
#define FLAG_BUTTON_AUX1_BIT			14
//! Flag is set if the AUX button 2 is pressed
#define FLAG_BUTTON_AUX2_BIT			15


//! This bit shows the status of the radio sense input on floor 1
#define STATUS_RADIO_SENSE1_BIT		0
//! This bit shows the status of the footswitch input
#define STATUS_FOOTSWITCH_BIT			1
//! This bit shows the status of the radio sense input on floor 2
#define STATUS_RADIO_SENSE2_BIT		2
//! This bit shows the status of the USB DTR on USB port 1
#define STATUS_USB1_DTR_BIT				3
//! This bit shows the status of the USB DTR on USB port 2
#define STATUS_USB2_DTR_BIT				4
//! This bit shows the status of the USB RTS on USB port 2
#define STATUS_USB2_RTS_BIT				5

#endif
