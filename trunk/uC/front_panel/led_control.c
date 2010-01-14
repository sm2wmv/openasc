/*! \file led_control.c \brief Front panel LED control functions
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-25
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

#include <stdio.h>
#include <avr/io.h>

#include "led_control.h"
#include "board.h"
#include "../global.h"

/*! Set the band LEDs to the proper band
 *  \param band The band we wish to turn on the LED for */
void led_set_band(unsigned char band) {
	if ((band >= BAND_160M) && (band <= BAND_12M))
		PORTA |= (1<<(7-band+1));
	else if (band == BAND_10M)
		PORTJ |= (1<<7);
}

/*! Turn off all band leds
 *  \param band Turn off all band leds */
void led_set_band_none(void) {
		PORTJ &= ~(1<<7);
		PORTA = 0;
}

/*! Set the PTT LED
 *  \param state The state of the LED */
void led_set_ptt(enum enum_led_ptt_state state) {
	if (state == LED_STATE_PTT_ACTIVE) {
		//Turn off the red led and turn on the green led
		PORTC |= (1<<LED_PTT_GREEN_BIT);
		PORTC &= ~(1<<LED_PTT_RED_BIT);
	}
	else if (state == LED_STATE_PTT_INHIBIT) {
		//Turn off the green led and turn on the red led
		PORTC |= (1<<LED_PTT_RED_BIT);
		PORTC &= ~(1<<LED_PTT_GREEN_BIT);
	}
	else if (state == LED_STATE_PTT_OK) {
		//Turn off both
		PORTC &= ~(1<<LED_PTT_GREEN_BIT);
		PORTC &= ~(1<<LED_PTT_RED_BIT);
	}
}

/*! Set the error LED status
 *  \param state The state of the LED */
void led_set_error(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTH |= (1<<LED_ERROR_BIT);
	else
		PORTH &= ~(1<<LED_ERROR_BIT);
}

void led_set_rotation_active(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTC |= (1<<LED_ROTATION_ACTIVE_BIT);
	else
		PORTC &= ~(1<<LED_ROTATION_ACTIVE_BIT);
}

/*! Set the TX Antenna LED status
 *  \param index Which LED we wish to change the status of
 *  \param state The state of the LED */
void led_set_tx_ant(unsigned char index, enum enum_led_state state) {
	switch(index) {
		case 0: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON1_BIT);
							PORTL |= (1<<LED_TX_BUTTON2_BIT);
							PORTL |= (1<<LED_TX_BUTTON3_BIT);
							PORTL |= (1<<LED_TX_BUTTON4_BIT);
						}
						else {
							PORTL &= ~(1<<LED_TX_BUTTON1_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON2_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON3_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON4_BIT);
						}
						break;
		case 1: if (state == LED_STATE_ON)
							PORTL |= (1<<LED_TX_BUTTON1_BIT);
						else
							PORTL &= ~(1<<LED_TX_BUTTON1_BIT);
						break;
		case 2: if (state == LED_STATE_ON)
							PORTL |= (1<<LED_TX_BUTTON2_BIT);
						else
							PORTL &= ~(1<<LED_TX_BUTTON2_BIT);
						break;
		case 3: if (state == LED_STATE_ON)
							PORTL |= (1<<LED_TX_BUTTON3_BIT);
						else
							PORTL &= ~(1<<LED_TX_BUTTON3_BIT);
						break;
		case 4: if (state == LED_STATE_ON)
							PORTL |= (1<<LED_TX_BUTTON4_BIT);
						else
							PORTL &= ~(1<<LED_TX_BUTTON4_BIT);
						break;
		default: break;
	}
}

/*! Set the RX Antenna LED status
 *  \param index Which LED we wish to change the status of
 *  \param state The state of the LED */
void led_set_rx_ant(unsigned char index, enum enum_led_state state) {
	switch(index) {
		case 0: if (state == LED_STATE_ON) {
							PORTH |= (1<<LED_RX_BUTTON1_BIT);
							PORTH |= (1<<LED_RX_BUTTON2_BIT);
							PORTB |= (1<<LED_RX_BUTTON3_BIT);
							PORTB |= (1<<LED_RX_BUTTON4_BIT);
						}
						else {
							PORTH &= ~(1<<LED_RX_BUTTON1_BIT);
							PORTH &= ~(1<<LED_RX_BUTTON2_BIT);
							PORTB &= ~(1<<LED_RX_BUTTON3_BIT);
							PORTB &= ~(1<<LED_RX_BUTTON4_BIT);
						}
						break;
		case 1: if (state == LED_STATE_ON)
							PORTH |= (1<<LED_RX_BUTTON1_BIT);
						else
							PORTH &= ~(1<<LED_RX_BUTTON1_BIT);
						break;
		case 2: if (state == LED_STATE_ON)
							PORTH |= (1<<LED_RX_BUTTON2_BIT);
						else
							PORTH &= ~(1<<LED_RX_BUTTON2_BIT);
						break;
		case 3: if (state == LED_STATE_ON)
							PORTB |= (1<<LED_RX_BUTTON3_BIT);
						else
							PORTB &= ~(1<<LED_RX_BUTTON3_BIT);
						break;
		case 4: if (state == LED_STATE_ON)
							PORTB |= (1<<LED_RX_BUTTON4_BIT);
						else
							PORTB &= ~(1<<LED_RX_BUTTON4_BIT);
						break;
		default: break;
	}
}

/*! Set the Rotate LED status
 *  \param state The state of the LED */
void led_set_rotate(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTC |= (1<<LED_ROTATE_BIT);
	else
		PORTC &= ~(1<<LED_ROTATE_BIT);
}

/*! Set the TX/RX mode LED status
 *  \param state The state of the LED */
void led_set_txrx(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTC |= (1<<LED_TXRX_BIT);
	else
		PORTC &= ~(1<<LED_TXRX_BIT);
} 

/*! Set the RX antenna LED status
 *  \param state The state of the LED */
void led_set_rxant(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTD |= (1<<LED_RXANT_BIT);
	}
	else {
		PORTD &= ~(1<<LED_RXANT_BIT);
	}
}

/*! Set the AUX LED status
 *  \param state The state of the LED */
void led_set_aux(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTD |= (1<<LED_AUX_BIT);
	else
		PORTD &= ~(1<<LED_AUX_BIT);
}

/*! Set the menu LED status
 *  \param state The state of the LED */
void led_set_menu(enum enum_led_state state) {
	if (state == LED_STATE_ON)
		PORTK |= (1<<LED_MENU_BIT);
	else
		PORTK &= ~(1<<LED_MENU_BIT);
}

/*! Set all the LEDs
 *  \param state The state of the LED */
void led_set_all(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		led_set_rx_ant(0,LED_STATE_ON);
		led_set_tx_ant(0,LED_STATE_ON);
		led_set_error(LED_STATE_ON);
		led_set_ptt(LED_STATE_PTT_ACTIVE);
		led_set_rxant(LED_STATE_ON);
		led_set_txrx(LED_STATE_ON);
		led_set_rotate(LED_STATE_ON);
		led_set_aux(LED_STATE_ON);
		led_set_menu(LED_STATE_ON);
	}
	else {
		led_set_rx_ant(0,LED_STATE_OFF);
		led_set_tx_ant(0,LED_STATE_OFF);
		led_set_error(LED_STATE_OFF);
		led_set_ptt(LED_STATE_PTT_OK);
		led_set_rxant(LED_STATE_OFF);
		led_set_txrx(LED_STATE_OFF);
		led_set_rotate(LED_STATE_OFF);
		led_set_aux(LED_STATE_OFF);
		led_set_menu(LED_STATE_OFF);
		PORTG &= ~(1<<0);
		PORTG &= ~(1<<1);
		led_set_band(BAND_UNDEFINED);
	}
}
