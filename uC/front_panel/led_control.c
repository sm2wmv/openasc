/*! \file front_panel/led_control.c
 *  \brief Front panel LED control functions
 *  \ingroup front_panel_group 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/led_control.c" \endcode
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

static unsigned char flag_led_status_ptt_changed = 0;
static unsigned char flag_led_status_changed = 0;


static unsigned int led_status;
static unsigned char led_ptt_status;

/*! \brief Set the band LEDs to the proper band
 *  \param band The band we wish to turn on the LED for */
void led_set_band(unsigned char band) {
	if ((band >= BAND_160M) && (band <= BAND_12M))
		PORTA |= (1<<(7-band+1));
	else if (band == BAND_10M)
		PORTJ |= (1<<7);
}

/*! \brief  Turn off all band leds */
void led_set_band_none(void) {
		PORTJ &= ~(1<<7);
		PORTA = 0;
}

/*! \brief  Set the PTT LED
 *  \param state The state of the LED */
void led_set_ptt(enum enum_led_ptt_state state) {
	if (state == LED_STATE_PTT_ACTIVE) {
		//Turn off the red led and turn on the green led
		PORTC |= (1<<LED_PTT_GREEN_BIT);
		PORTC &= ~(1<<LED_PTT_RED_BIT);
    led_ptt_status = LED_PTT_STATUS_ACTIVE;
	}
	else if (state == LED_STATE_PTT_INHIBIT) {
		//Turn off the green led and turn on the red led
		PORTC |= (1<<LED_PTT_RED_BIT);
		PORTC &= ~(1<<LED_PTT_GREEN_BIT);
    led_ptt_status = LED_PTT_STATUS_INHIBIT;
	}
	else if (state == LED_STATE_PTT_OK) {
		//Turn off both
		PORTC &= ~(1<<LED_PTT_GREEN_BIT);
		PORTC &= ~(1<<LED_PTT_RED_BIT);
    led_ptt_status = LED_PTT_STATUS_OK;
	}
	
  flag_led_status_ptt_changed = 1;
}

/*! \brief  Set the error LED status
 *  \param state The state of the LED */
void led_set_error(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTH |= (1<<LED_ERROR_BIT);
    led_status |= (1<<LED_STATUS_ERROR);
  }
	else {
		PORTH &= ~(1<<LED_ERROR_BIT);
    led_status &= ~(1<<LED_STATUS_ERROR);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief  Set the rotating led to active state, indicates if any antenna on the current band is rotating
 *  \param state The state of the LED */
void led_set_rotation_active(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTC |= (1<<LED_ROTATION_ACTIVE_BIT);
    led_status |= (1<<LED_STATUS_ROTATING);
  }
	else {
		PORTC &= ~(1<<LED_ROTATION_ACTIVE_BIT);
    led_status &= ~(1<<LED_STATUS_ROTATING);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief  Set the TX Antenna LED status
 *  \param index Which LED we wish to change the status of
 *  \param state The state of the LED */
void led_set_tx_ant(unsigned char index, enum enum_led_state state) {
	switch(index) {
		case 0: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON1_BIT);
							PORTL |= (1<<LED_TX_BUTTON2_BIT);
							PORTL |= (1<<LED_TX_BUTTON3_BIT);
							PORTL |= (1<<LED_TX_BUTTON4_BIT);
              
              led_status |= (1<<LED_STATUS_TX_ANT1) | (1<<LED_STATUS_TX_ANT2) | (1<<LED_STATUS_TX_ANT3) | (1<<LED_STATUS_TX_ANT4);
						}
						else {
							PORTL &= ~(1<<LED_TX_BUTTON1_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON2_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON3_BIT);
							PORTL &= ~(1<<LED_TX_BUTTON4_BIT);
              
              led_status &= ~(1<<LED_STATUS_TX_ANT1);
              led_status &= ~(1<<LED_STATUS_TX_ANT2);
              led_status &= ~(1<<LED_STATUS_TX_ANT3);
              led_status &= ~(1<<LED_STATUS_TX_ANT4);
						}
						break;
		case 1: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON1_BIT);
              led_status |= (1<<LED_STATUS_TX_ANT1);
            }
						else {
							PORTL &= ~(1<<LED_TX_BUTTON1_BIT);
              led_status &= ~(1<<LED_STATUS_TX_ANT1);
            }
						break;
		case 2: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON2_BIT);
              led_status |= (1<<LED_STATUS_TX_ANT2);
            }
						else {
							PORTL &= ~(1<<LED_TX_BUTTON2_BIT);
              led_status &= ~(1<<LED_STATUS_TX_ANT2);
            }
						break;
		case 3: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON3_BIT);
              led_status |= (1<<LED_STATUS_TX_ANT3);
            }
						else {
							PORTL &= ~(1<<LED_TX_BUTTON3_BIT);
              led_status &= ~(1<<LED_STATUS_TX_ANT3);
            }
						break;
		case 4: if (state == LED_STATE_ON) {
							PORTL |= (1<<LED_TX_BUTTON4_BIT);
              led_status |= (1<<LED_STATUS_TX_ANT4);
            }
						else {
							PORTL &= ~(1<<LED_TX_BUTTON4_BIT);
              led_status &= ~(1<<LED_STATUS_TX_ANT4);
            }
						break;
		default: break;
  }

  flag_led_status_changed = 1;
}

/*! \brief Set the RX Antenna LED status
 *  \param index Which LED we wish to change the status of
 *  \param state The state of the LED */
void led_set_rx_ant(unsigned char index, enum enum_led_state state) {
	switch(index) {
		case 0: if (state == LED_STATE_ON) {
							PORTH |= (1<<LED_RX_BUTTON1_BIT);
							PORTH |= (1<<LED_RX_BUTTON2_BIT);
							PORTB |= (1<<LED_RX_BUTTON3_BIT);
							PORTB |= (1<<LED_RX_BUTTON4_BIT);
              
              led_status |= (1<<LED_STATUS_RX_ANT1);
              led_status |= (1<<LED_STATUS_RX_ANT2);
              led_status |= (1<<LED_STATUS_RX_ANT3);
              led_status |= (1<<LED_STATUS_RX_ANT4);
						}
						else {
							PORTH &= ~(1<<LED_RX_BUTTON1_BIT);
							PORTH &= ~(1<<LED_RX_BUTTON2_BIT);
							PORTB &= ~(1<<LED_RX_BUTTON3_BIT);
							PORTB &= ~(1<<LED_RX_BUTTON4_BIT);

              led_status &= ~(1<<LED_STATUS_RX_ANT1);
              led_status &= ~(1<<LED_STATUS_RX_ANT2);
              led_status &= ~(1<<LED_STATUS_RX_ANT3);
              led_status &= ~(1<<LED_STATUS_RX_ANT4);
						}
						break;
		case 1: if (state == LED_STATE_ON) {
							PORTH |= (1<<LED_RX_BUTTON1_BIT);
              led_status |= (1<<LED_STATUS_RX_ANT1);
            }
						else {
							PORTH &= ~(1<<LED_RX_BUTTON1_BIT);
              led_status &= ~(1<<LED_STATUS_RX_ANT1);
            }
						break;
		case 2: if (state == LED_STATE_ON) {
							PORTH |= (1<<LED_RX_BUTTON2_BIT);
              led_status |= (1<<LED_STATUS_RX_ANT2);
            }
						else {
							PORTH &= ~(1<<LED_RX_BUTTON2_BIT);
              led_status &= ~(1<<LED_STATUS_RX_ANT2);
            }
						break;
		case 3: if (state == LED_STATE_ON) {
							PORTB |= (1<<LED_RX_BUTTON3_BIT);
              led_status |= (1<<LED_STATUS_RX_ANT3);
            }
						else {
							PORTB &= ~(1<<LED_RX_BUTTON3_BIT);
              led_status &= ~(1<<LED_STATUS_RX_ANT3);
            }
						break;
		case 4: if (state == LED_STATE_ON) {
							PORTB |= (1<<LED_RX_BUTTON4_BIT);
              led_status |= (1<<LED_STATUS_RX_ANT4);
            }
						else {
							PORTB &= ~(1<<LED_RX_BUTTON4_BIT);
              led_status &= ~(1<<LED_STATUS_RX_ANT4);
            }
						break;
		default: 
      break;
	}

  flag_led_status_changed = 1;
}

/*! \brief Set the Rotate LED status
 *  \param state The state of the LED */
void led_set_rotate(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTC |= (1<<LED_ROTATE_BIT);
    led_status |= (1<<LED_STATUS_ROTATE);
  }
	else {
		PORTC &= ~(1<<LED_ROTATE_BIT);
    led_status &= ~(1<<LED_STATUS_ROTATE);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief Set the TX/RX mode LED status
 *  \param state The state of the LED */
void led_set_txrx(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTC |= (1<<LED_TXRX_BIT);
    led_status |= (1<<LED_STATUS_TXRX);
  }
	else {
		PORTC &= ~(1<<LED_TXRX_BIT);
    led_status &= ~(1<<LED_STATUS_TXRX);
  }
  
  flag_led_status_changed = 1;
} 

/*! \brief Set the RX antenna LED status
 *  \param state The state of the LED */
void led_set_rxant(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTD |= (1<<LED_RXANT_BIT);
    led_status |= (1<<LED_STATUS_RXANT);
	}
	else {
		PORTD &= ~(1<<LED_RXANT_BIT);
    led_status &= ~(1<<LED_STATUS_RXANT);
	}
	
	flag_led_status_changed = 1;
}

/*! \brief Set the AUX LED status
 *  \param state The state of the LED */
void led_set_aux(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTC |= (1<<LED_AUX_BIT);
    led_status |= (1<<LED_STATUS_AUX);
  }
	else {
		PORTC &= ~(1<<LED_AUX_BIT);
    led_status &= ~(1<<LED_STATUS_AUX);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief Set the AUX LED status
 *  \param state The state of the LED */
void led_set_submenu(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTD |= (1<<LED_SUBMENU_BIT);
    led_status |= (1<<LED_STATUS_SUB);
  }
	else {
		PORTD &= ~(1<<LED_SUBMENU_BIT);
    led_status &= ~(1<<LED_STATUS_SUB);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief Set the menu LED status
 *  \param state The state of the LED */
void led_set_menu(enum enum_led_state state) {
	if (state == LED_STATE_ON) {
		PORTK |= (1<<LED_MENU_BIT);
    led_status |= (1<<LED_STATUS_MENU);
  }
	else {
		PORTK &= ~(1<<LED_MENU_BIT);
    led_status &= ~(1<<LED_STATUS_MENU);
  }
  
  flag_led_status_changed = 1;
}

/*! \brief Set all the LEDs
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
		led_set_band(BAND_UNDEFINED);
	}
	
	flag_led_status_changed = 1;
}

unsigned char led_status_changed(void) {
  if (flag_led_status_changed)
    return(1);
  if(flag_led_status_ptt_changed)
    return(1);
  
  return(0);
}

unsigned int led_get_status(void) {
  return(led_status);
}

unsigned char led_get_ptt_status(void) {
  return(led_ptt_status);
}

void led_status_clear(void) {
  flag_led_status_changed = 0;
  flag_led_status_ptt_changed = 0;
}