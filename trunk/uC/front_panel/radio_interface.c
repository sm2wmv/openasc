/*! \file radio_interface.c \brief Handles all the interfacing towards the radio
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-04-08
 */
//    Copyright (C) 2008  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHSIG_USART1_RECVOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "radio_interface.h"
#include "led_control.h"
#include "band_ctrl.h"
#include "main.h"
#include "usart.h"
#include "board.h"
#include "eeprom.h"
#include "display.h"
#include "../global.h"

unsigned char *radio_serial_rx_buffer;
unsigned char *radio_serial_rx_buffer_start;

struct_radio_status radio_status;
struct_radio_settings radio_settings;

unsigned char radio_flags;

unsigned char radio_sent_request = 0;

//! External variable of the radio rx data counter used for a timeout
extern unsigned char radio_rx_data_counter;

/*! Initialize the radio interface */
void radio_interface_init(void) {
	//Initialize the serial rx buffer used for the communication with the radio
	radio_serial_rx_buffer = (unsigned char *)malloc(RADIO_SERIAL_RX_BUFFER_LENGTH);
	radio_serial_rx_buffer_start = radio_serial_rx_buffer;
	
	if ((radio_settings.interface_type == RADIO_INTERFACE_CAT_POLL) | (radio_settings.interface_type == RADIO_INTERFACE_CAT_MON)) {
		switch (radio_settings.baudrate) {
			case RADIO_SERIAL_BAUDRATE_1200 : usart3_init(766, radio_settings.stopbits);
																				usart1_init(766, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_2400 : usart3_init(383, radio_settings.stopbits);
																				usart1_init(383, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_4800 : usart3_init(191, radio_settings.stopbits);
																				usart1_init(191, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_9600 :	usart3_init(95, radio_settings.stopbits);
																				usart1_init(95, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_14400 :	usart3_init(63, radio_settings.stopbits);
																					usart1_init(63, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_19200 :	usart3_init(47, radio_settings.stopbits);
																					usart1_init(47, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_28800 :	usart3_init(31, radio_settings.stopbits);
																					usart1_init(31, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_38400 :	usart3_init(23, radio_settings.stopbits);
																					usart1_init(23, radio_settings.stopbits);
																				break;
			case RADIO_SERIAL_BAUDRATE_57600 : 	usart3_init(15, radio_settings.stopbits);
																					usart1_init(15, radio_settings.stopbits);
																				break;
		}
	}
}

/*! This function is called each lap in the main loop and we can use this to process certain tasks */
void radio_process_tasks(void) {
	if (radio_flags & (1<<RADIO_FLAG_FREQ_CHANGED)) {
		radio_status.current_band = radio_freq_to_band(radio_status.current_freq);	
	}
}

/*! Retrieve the frequency from the radio. If it's configured for BCD it just retrieves the freq band
 *  The frequency is returned as an integer so for example 21350 means 21 MHz and 350 kHz.
 *  \return The frequency as an integer, max freq 65536 */
unsigned int radio_get_current_freq(void) {
	return(radio_status.current_freq);
}


/*! Retrieve the current band from the radio
 *  \return The radios band */
unsigned char radio_get_current_band(void) {
	return(radio_status.current_band);
}

/*! Activate the radio PTT */
void radio_ptt_active(void) {
	if (runtime_settings.radio_ptt_output) {
		if (main_get_inhibit_state() == INHIBIT_OK_TO_SEND)
			led_set_ptt(LED_STATE_PTT_ACTIVE);
		else if(main_get_inhibit_state() == INHIBIT_NOT_OK_TO_SEND)
			led_set_ptt(LED_STATE_PTT_INHIBIT);
		
		/* Activate the PTT to the radio */
		PORTG |= (1<<RADIO_PTT_OUTPUT_BIT);
		PORTJ |= (1<<3);
	}
}

/*! Deactivate the radio PTT */
void radio_ptt_deactive(void) {
	if (main_get_inhibit_state() == INHIBIT_OK_TO_SEND)
		led_set_ptt(LED_STATE_PTT_OK);
	else if(main_get_inhibit_state() == INHIBIT_NOT_OK_TO_SEND)
		led_set_ptt(LED_STATE_PTT_INHIBIT);
		
	/* Deactivate the PTT to the radio */
	PORTG &= ~(1<<RADIO_PTT_OUTPUT_BIT);
	PORTJ &= ~(1<<3);
}

/*! Set the inhibit signal to high */
void radio_inhibit_high(void) {
	//This signal is inverted in hardware, because of a transistor output with pullup
	PORTE &= ~(1<<RADIO_INHIBIT_OUTPUT_BIT);
}

/*! Set the inhibit signal to low */
void radio_inhibit_low(void) {
	//This signal is inverted in hardware, because of a transistor output with pullup
	PORTE |= (1<<RADIO_INHIBIT_OUTPUT_BIT);
}

/*! Get the portion of the band the radio is on.
 *  \return Return BAND_HIGH if it's in the higher portion of the band, BAND_LOW if it's the lower portion. If neither
 *  then it returns BAND_UNDEFINED */
unsigned char radio_get_band_portion(void) {
	if ((radio_status.current_freq >= band_ctrl_get_low_portion_low(radio_status.current_band)) && (radio_status.current_freq < band_ctrl_get_low_portion_high(radio_status.current_band)))
		return (BAND_LOW);
	else if ((radio_status.current_freq >= band_ctrl_get_high_portion_low(radio_status.current_band)) && (radio_status.current_freq <= band_ctrl_get_high_portion_high(radio_status.current_band)))
		return (BAND_HIGH);
	
	return(BAND_UNDEFINED);
}

/*! Polls the status of the PTT input
 *  \return Return RADIO_PTT_ACTIVATE if the radio is PTT and RADIO_PTT_DEACTIVATE if it doesn't */
unsigned char radio_poll_ptt(void) {
	//Senses the radio PTT from the top floor
	if ((radio_settings.ptt_input << RADIO_SENSE_UPPER_FLOOR)) {
		if ((radio_settings.ptt_input << RADIO_SENSE_INVERTED))
			if (PINJ << 2)
				return(RADIO_PTT_DEACTIVE);
			else
				return(RADIO_PTT_ACTIVE);
		else {
			if (PINJ << 2)
				return(RADIO_PTT_DEACTIVE);
			else
				return(RADIO_PTT_ACTIVE);
		}
	}
	//Senses the radio PTT from the lower floor
	else if ((radio_settings.ptt_input << RADIO_SENSE_LOWER_FLOOR)) {
		if ((radio_settings.ptt_input << RADIO_SENSE_INVERTED))
			if (PINE << 5)
				return(RADIO_PTT_DEACTIVE);
		else
			return(RADIO_PTT_ACTIVE);
		else {
			if (PINE << 5)
				return(RADIO_PTT_ACTIVE);
			else
				return(RADIO_PTT_DEACTIVE);
		}
	}

	return(RADIO_PTT_DEACTIVE);
}

/*! Polls the status of the radio and saves it into the radio_status structure.
 *  \return 0 if the poll went OK and 1 if it didn't  */
unsigned char radio_poll_status(void) {
	//Ask radio for freq etc
	if (radio_settings.interface_type == RADIO_INTERFACE_BCD) {
		unsigned char bcd;
		
		bcd = (PINF & (1<<2))>>2;
		bcd += (PINF & (1<<3))>>2;
		bcd += (PINF & (1<<0))<<2;
		bcd += (PINF & (1<<1))<<2;
		
		radio_status.current_band = bcd;
	}
	else if (radio_settings.interface_type == RADIO_INTERFACE_CAT_POLL) {
		
	}
	else if (radio_settings.interface_type == RADIO_INTERFACE_CAT_MON) {
		display_update_radio_freq();
	}
	
	return(0);
}

/*! Parse the radios frequency
 *  \param freq_data The frequency data sent in as an array of characters
 *  \param length The length of the frequency data
 *  \param radio_type The type of radio that the freq should be parsed for
 *  \return The radios frequency in integer format. So for example 21305 is 21 MHz and 305 kHz. */
unsigned int radio_parse_freq(unsigned char *freq_data, unsigned char length, unsigned char radio_model) {
	unsigned int freq=0;
	
	if (radio_model == RADIO_MODEL_KENWOOD) {
		//TODO: Not tested

		for (unsigned char i=3;i<8;i++) {
			if (i == 3)
				freq = *(freq_data - 48) * 10000;
			else if (i == 4)
				freq += *(freq_data - 48) * 1000;
			else if (i == 5)
				freq += *(freq_data - 48) * 100;
			else if (i == 6)
				freq += *(freq_data - 48) * 10;
			else
				freq += *(freq_data - 48);
		}
	}
	else if (radio_model == RADIO_MODEL_ICOM) {
		//TODO: Not tested

		freq =  ((0xF0 & *(freq_data+8))>>4) * 10000;
		freq += ( 0x0F & *(freq_data+8)) * 1000;
		freq += ((0xF0 & *(freq_data+7))>>4) * 100;
		freq += ( 0x0F & *(freq_data+7)) * 10;
		freq += ((0xF0 & *(freq_data+6))>>4);
	}
	else if (radio_model == RADIO_MODEL_FT1000MKV) {
		//TODO: Not tested

		if (length != 16)
			return(0);
		
		/* Operating frequency for FT1000MKV is 4 bytes located on positions 1, 2, 3, 4 in the received data.
		 * Operating mode is in position 7. */
		freq =  ( 0x0F & *(freq_data+4)) * 10000;
		freq += ((0xF0 & *(freq_data+3))>>4) * 1000;
		freq += ( 0x0F & *(freq_data+3)) * 100;
		freq += ((0xF0 & *(freq_data+2))>>4) * 10;
		freq += ( 0x0F & *(freq_data+2));
	}
	else if (radio_model == RADIO_MODEL_FT1000) {
		/*TODO: This is very tricky to implement without the FT1000 firmware upgrade.
						But hopefully we will be able to implement this */
	}
	
	return(freq);
}

//! Activate PTT amp
void radio_amp_ptt_active(void) {
	if (runtime_settings.amplifier_ptt_output)
		PORTE |= (1<<AMPLIFIER_OUTPUT_BIT);
}

//! Deactivate PTT amp
void radio_amp_ptt_deactive(void) {
	PORTE &= ~(1<<AMPLIFIER_OUTPUT_BIT);
}


/*! Convert a radio frequency (integer) to band data
 *  \param freq The frequency as integer
 *  \return The band of the frequency sent in as parameter. If band not found then it returns BAND_UNDEFINED */
unsigned char radio_freq_to_band(unsigned int freq) {
	for (unsigned char i=0;i<9;i++)
		if ((freq >= band_ctrl_get_low_portion_low(i+1)) && (freq <= band_ctrl_get_high_portion_high(i+1)))
			return(i+1);
	
	//If the freq isn't in the band array then we just return BAND_UNDEFINED
	return(BAND_UNDEFINED);
}

/*! Set which radio model is used, saves it in the radio_settings struct
 *  \param model The radio model */
void radio_interface_set_model(unsigned char model) {
	radio_settings.radio_model = model;
}

/*! Set which radio interface is used, saves it in the radio_settings struct
 *  \param interface_type The interface type */
void radio_interface_set_interface(unsigned char interface_type) {
	radio_settings.interface_type = interface_type;
}

/*! Set which baudrate setting is used, saves it in the radio_settings struct
 *  \param baudrate Which baudrate setting to use */
void radio_interface_set_baudrate(unsigned char baudrate) {
	radio_settings.baudrate = baudrate;
}

/*! Set which number of stopbits should be used, saves it in the radio_settings struct
 *  \param stopbits The number of stopbits that are used to interface the radio */
void radio_interface_set_stopbits(unsigned char stopbits) {
	radio_settings.stopbits = stopbits;
}

/*! Set which CI-V address the radio has got, saves it in the radio_settings struct
 *  \param civ The CI-V address */
void radio_interface_set_civ_addr(unsigned char civ) {
	radio_settings.civ_addr = civ;	
}

/*! Set which PTT mode is used, saves it in the radio_settings struct
 *  \param ptt_mode Set which PTT mode to be used */
void radio_interface_set_ptt_mode(unsigned char ptt_mode) {
	radio_settings.ptt_mode = ptt_mode;
}

/*! Set which PTT input that is used, saves it in the radio_settings struct
 *  \param ptt_input Which PTT input that is used */
void radio_interface_set_ptt_input(unsigned char ptt_input) {
	radio_settings.ptt_input = ptt_input;
}

/*! Set the poll intervall for the radio band decoding, saves it in the radio_settings struct
 *  \param poll_interval The poll interval in ms/10 */
void radio_interface_set_poll_interval(unsigned char poll_interval) {
	radio_settings.poll_interval = poll_interval;
}

/*! Get which radio model is used
 *  \return The radio model */
unsigned char radio_interface_get_model(void) {
	return(radio_settings.radio_model);
}

/*! Get which radio interface is used
 *  \return The interface type */
unsigned char radio_interface_get_interface(void) {
	return(radio_settings.interface_type);
}

/*! Get which baudrate setting is used
 *  \return Which baudrate setting is used */
unsigned char radio_interface_get_baudrate(void) {
	return(radio_settings.baudrate);
}

/*! Get which number of stopbits should be used
 *  \return The number of stopbits that are used to interface the radio */
unsigned char radio_interface_get_stopbits(void) {
	return(radio_settings.stopbits);
}

/*! Get which CI-V address the radio has got
 *  \return The CI-V address */
unsigned char radio_interface_get_civ_addr(void) {
	return(radio_settings.civ_addr);
}

/*! Get which PTT mode is used
 *  \return Get which PTT mode to be used */
unsigned char radio_interface_get_ptt_mode(void) {
	return(radio_settings.ptt_mode);
}

/*! Get which PTT input that is used
 *  \return Which PTT input that is used */
unsigned char radio_interface_get_ptt_input(void) {
	return(radio_settings.ptt_input);
}

/*! Get the poll intervall for the radio band decoding
 *  \return The poll interval in ms/10 */
unsigned char radio_interface_get_poll_interval(void){
	return(radio_settings.poll_interval);
}


/*! This function will save the current data in the radio_settings struct into the EEPROM
    at it's proper location */
void radio_interface_save_eeprom(void) {
	eeprom_save_radio_settings_structure(&radio_settings);
}

/*! This function will load data from the eeprom to the radio_settings struct */
void radio_interface_load_eeprom(void) {
	eeprom_get_radio_settings_structure(&radio_settings);
}

ISR(SIG_USART3_DATA) {
}

ISR(SIG_USART3_RECV) {
	unsigned char data = UDR3;
	
	radio_rx_data_counter = 0;
	
	if ((radio_settings.interface_type == RADIO_INTERFACE_CAT_POLL) | (radio_settings.interface_type == RADIO_INTERFACE_CAT_MON)) {
		/*if (radio_settings.radio_model == RADIO_MODEL_KENWOOD) {
			if (data == ';') {
				if (strncmp((char*)radio_serial_rx_buffer_start,"IF",2)) {
					radio_status.current_freq = radio_parse_freq(radio_serial_rx_buffer_start,radio_serial_rx_buffer_start-radio_serial_rx_buffer,RADIO_MODEL_KENWOOD);
					radio_status.current_band = radio_freq_to_band(radio_status.current_freq);
				}
			}
			else {
				if ((radio_serial_rx_buffer - radio_serial_rx_buffer_start) >= RADIO_SERIAL_RX_BUFFER_LENGTH)
					radio_serial_rx_buffer = radio_serial_rx_buffer_start;
				else
					*(radio_serial_rx_buffer++) = data;
			}
		}*/
		if (radio_settings.radio_model == RADIO_MODEL_ICOM) {
			if (data == 0xFD) {
				if ((radio_serial_rx_buffer_start[0] == 0xFE) && (radio_serial_rx_buffer_start[1] == 0xFE)) {
					//TODO: Consider moving the parsing etc outside of the interrupt, to make the interrupt take as little time as possible
					//TODO: Implement a timeout which resets the buffer after XX ms
					radio_status.current_freq = radio_parse_freq(radio_serial_rx_buffer_start,radio_serial_rx_buffer_start-radio_serial_rx_buffer,RADIO_MODEL_ICOM);
					
					radio_flags |= (1<<RADIO_FLAG_FREQ_CHANGED);
					radio_serial_rx_buffer = radio_serial_rx_buffer_start;
				}
			}
			else {
				if ((radio_serial_rx_buffer - radio_serial_rx_buffer_start) >= RADIO_SERIAL_RX_BUFFER_LENGTH)
					radio_serial_rx_buffer = radio_serial_rx_buffer_start;
				else
					*(radio_serial_rx_buffer++) = data;
			}
		}
	}
	
	usart1_transmit(data);
}
