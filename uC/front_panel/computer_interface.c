/*! \file computer_interface.c \brief Interface towards the computer
 * \author Mikael Larsmark, SM2WMV
 * \date 2008-05-01
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

/* Protocol structure
 * ------------------
 * --------------------------------------------
 * |0xFE|0xFE|CMD|LENGTH|DATA[0]..DATA[n]|0xFD|
 * --------------------------------------------
 * After each message an ACK should be sent back to the computer. If no other data
 * is returned then just an ACK message containing the checksum, otherwise data that
 * should be sent back with the checksum retrieved as the DATA[0].
 */

//Debug flag for the computer communication protocol
//#define COM_DEBUG

#include <avr/wdt.h>

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <avr/wdt.h>

#include "computer_interface.h"
#include "radio_interface.h"
#include "usart.h"
#include "ds1307.h"
#include "antenna_ctrl.h"
#include "eeprom.h"
#include "led_control.h"

//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH	128

//! The length of the computer RX BUFFER
#define COMPUTER_TX_BUFFER_LENGTH	20

#define COMPUTER_INTERFACE_FIXED_SIZE	5

//! The preamble of the computer communication protocol
#define COMPUTER_COMM_PREAMBLE	0xFE
//! The postamble of the computer communication protocol
#define COMPUTER_COMM_POSTAMBLE	0xFD
//! The serial acknowledge of the computer communication protocol
#define COMPUTER_COMM_ACK				0xFA
//! The serial NOT acknowledge of the computer communication protocol
#define COMPUTER_COMM_NACK			0xFB


//! Flag to see if the setup mode is activated
#define COMPUTER_COMM_FLAG_SETUP_MODE			0
//! Flag to see if the preamble was found
#define COMPUTER_COMM_FLAG_FOUND_PREAMBLE	1
//! Flag to see that there is data in the rx buffer
#define COMPUTER_COMM_FLAG_DATA_IN_RX_BUF	2

//! Command to force the openASC box into bootloader mode
#define COMPUTER_COMM_ENTER_BOOTLOADER		0x01
#define CTRL_REBOOT												0x02
#define CTRL_GET_FIRMWARE_REV							0x03
//! This function just replies with the same command, this is so we can see when something has been finished
#define CTRL_DONE													0x04


#define CTRL_SET_TIME  										0x10
#define CTRL_SET_ANT_DATA									0x11
#define CTRL_CREATE_EEPROM_TABLE					0x12
#define CTRL_SET_RX_ANT_DATA							0x13
#define CTRL_SET_RADIO_SETTINGS						0x14
#define CTRL_SET_DEVICE_SETTINGS					0x15
#define CTRL_SET_BAND_DATA								0x16
#define CTRL_SET_EXT_INPUT								0x17

/* Defines for the radio settings */
#define CTRL_SET_RADIO_SETTINGS_SAVE	0x01
#define CTRL_SET_RADIO_SETTINGS_ALL		0x02

/* Defines for the antenna data */
#define CTRL_SET_ANT_DATA_SAVE						0x01
#define CTRL_SET_ANT_DATA_TEXT						0x02
#define CTRL_SET_ANT_DATA_SUB_MENU_TYPE		0x03
#define CTRL_SET_ANT_DATA_ANT_FLAGS				0x04
#define CTRL_SET_ANT_DATA_COMB_ALLOWED		0x05
#define CTRL_SET_ANT_DATA_ANT_OUT_STR			0x06
#define CTRL_SET_ANT_ROTATOR_DATA					0x07
#define CTRL_SET_ANT_DEFAULT_INDEX				0x08

/* Defines for the band data */
#define CTRL_SET_BAND_DATA_LIMITS					0x01
#define CTRL_SET_BAND_DATA_LOW_OUT_STR		0x02
#define CTRL_SET_BAND_DATA_HIGH_OUT_STR		0x03
#define CTRL_SET_BAND_DATA_SAVE						0x07

/* Defines for the rx antenna data */
#define CTRL_SET_RX_ANT_DATA_TEXT						0x01
#define CTRL_SET_RX_ANT_DATA_ANT_OUT_STR		0x02
#define CTRL_SET_RX_ANT_DATA_BAND_OUT_STR		0x03
#define CTRL_SET_RX_ANT_DATA_SAVE						0x07

/* Defines for the device settings */
#define CTRL_SET_DEVICE_SETTINGS_ADDRESS						0x01
#define CTRL_SET_DEVICE_SETTINGS_NR_NODES						0x02
#define CTRL_SET_DEVICE_SETTINGS_DEVICE_IS_MASTER		0x03

/* bit 0 = PTTInputFootswitch
 * bit 1 = PTTInputRadioSense
 * bit 2 = PTTInputComputerRTS
 * bit 3 = PTTInputRadioSenseInverted
 * bit 4 = PTTInputComputerRTSInverted
 * bit 5 = InhibitPolarityActiveLow */
#define CTRL_SET_DEVICE_SETTINGS_PTT_INPUTS		0x04
#define CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS		0x05
#define CTRL_SET_DEVICE_SETTINGS_SAVE					0x07

/* Defines for the ext input commands */
#define CTRL_SET_EXT_KEYPAD_FUNCTIONS			0x01

typedef struct {
	//! The serial tx buffer
	char *tx_buffer;
	//! The start of the serial tx buffer
	char *tx_buffer_start;
	//! The length of the data in the buffer
	unsigned int tx_buffer_length;
	//! The serial rx buffer
	char *rx_buffer;
	//! The start of the serial rx buffer
	char *rx_buffer_start;
	//! Different flags
	unsigned char data_in_tx_buffer;
	unsigned char flags;
	unsigned char command;
	unsigned char length;
	unsigned int count;
	unsigned int eeprom_ptr;
} computer_comm_struct;

computer_comm_struct computer_comm;

struct_antenna *antenna_ptr;

struct_rx_antennas *rx_antenna_ptr;

struct_band *band_ptr;

struct_setting *settings_ptr;

void (*bootloader_start)(void) = (void *)0x1FE00;

//! Initialize the communication interface towards the computer
void computer_interface_init(void) {
	// Setup the RX buffer
	computer_comm.rx_buffer = (char *)malloc(COMPUTER_RX_BUFFER_LENGTH);
	computer_comm.rx_buffer_start = computer_comm.rx_buffer;
	
	computer_comm.tx_buffer = (char *)malloc(COMPUTER_TX_BUFFER_LENGTH);
	computer_comm.tx_buffer_start = computer_comm.tx_buffer;
	
	computer_comm.data_in_tx_buffer = 0;
	computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_SETUP_MODE);
}

void computer_interface_send_data(void) {
	if (computer_comm.data_in_tx_buffer == 1) {
		for (int i=0;i<computer_comm.tx_buffer_length;i++)
			usart1_transmit(computer_comm.tx_buffer_start[i]);
		
		computer_comm.tx_buffer = computer_comm.tx_buffer_start;
		computer_comm.tx_buffer_length = 0;

		computer_comm.data_in_tx_buffer = 0;
	}
}

void computer_interface_send(unsigned char command, unsigned int length, char *data) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = command;
	computer_comm.tx_buffer[3] = length;
	int i;
	for (i=0;i<length;i++)
		computer_comm.tx_buffer[4+i] = data[i];
		
	computer_comm.tx_buffer[4+i] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = COMPUTER_INTERFACE_FIXED_SIZE+length;
			
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_send_ack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_ACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_send_nack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_NACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_send_flag(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = 0xFC;
	computer_comm.tx_buffer[3] = 0x01;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

void computer_interface_parse_data(void) {
	unsigned char ptr_pos=2;
	
	if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF)) {
		if (computer_comm.command == CTRL_SET_TIME) {
			ds1307_set_time(computer_comm.rx_buffer_start);
			
			computer_interface_send_ack();
		}
		else if (computer_comm.command == COMPUTER_COMM_ENTER_BOOTLOADER) {
			computer_interface_send_ack();
			
			//Jump to the bootloader application section
			bootloader_start();
		}
		else if (computer_comm.command == CTRL_REBOOT) {
			computer_interface_send_ack();
			computer_interface_deactivate_setup();
			
			do {
				wdt_enable(WDTO_15MS);
				for(;;){}
			} 
			while(0);
		}
		else if (computer_comm.command == CTRL_GET_FIRMWARE_REV) {
			computer_interface_send(CTRL_GET_FIRMWARE_REV, strlen(FIRMWARE_REV), FIRMWARE_REV);
		}
		else if (computer_comm.command == CTRL_DONE) {
			computer_interface_send(CTRL_DONE, 0, 0);
		}
		else if (computer_comm.command == CTRL_SET_ANT_DATA) {
			//TODO: Sub menu support
			
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_ANT_DATA_TEXT:
					antenna_ptr->antenna_text_length[(int)computer_comm.rx_buffer_start[2]] = computer_comm.rx_buffer_start[3];
					
					if (computer_comm.rx_buffer_start[3] == 0)
						antenna_ptr->antenna_text[(int)computer_comm.rx_buffer_start[2]][0] = 0;
					else
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							antenna_ptr->antenna_text[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DATA_SUB_MENU_TYPE: 
					//antenna_ptr->sub_menu_type[(int)*(computer_comm.rx_buffer_start+1)] = *(computer_comm.rx_buffer_start+3);
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DATA_ANT_FLAGS:
					antenna_ptr->antenna_flag[0] = computer_comm.rx_buffer_start[2];
					antenna_ptr->antenna_flag[1] = computer_comm.rx_buffer_start[3];
					antenna_ptr->antenna_flag[2] = computer_comm.rx_buffer_start[4];
					antenna_ptr->antenna_flag[3] = computer_comm.rx_buffer_start[5];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DATA_COMB_ALLOWED:
					antenna_ptr->antenna_comb_allowed = (computer_comm.rx_buffer_start[1]<<8) | computer_comm.rx_buffer_start[2];
									
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DATA_ANT_OUT_STR:
					//Antenna index bit is use for which combination that should be chosen
					antenna_ptr->antenna_output_length[(int)computer_comm.rx_buffer_start[1]] = computer_comm.rx_buffer_start[2];
					
					for (int i=0;i<computer_comm.rx_buffer_start[2];i++) {
						antenna_ptr->antenna_comb_output_str[(int)computer_comm.rx_buffer_start[1]][i] = computer_comm.rx_buffer_start[3+i];
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_ROTATOR_DATA:
					for (unsigned char i=0;i<4;i++) {
						antenna_ptr->rotator_addr[i] = computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_max_rotation[i] = computer_comm.rx_buffer[ptr_pos++] << 8;
						antenna_ptr->rotator_max_rotation[i] += computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_min_heading[i] = computer_comm.rx_buffer[ptr_pos++] << 8;
						antenna_ptr->rotator_min_heading[i] += computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_delay[i] = computer_comm.rx_buffer[ptr_pos++];
					}
					
					antenna_ptr->rotator_view_360_deg = computer_comm.rx_buffer[ptr_pos++];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DEFAULT_INDEX:
					antenna_ptr->default_antenna = computer_comm.rx_buffer[2];
					computer_interface_send_ack();
					break;	
				case CTRL_SET_ANT_DATA_SAVE:
					//Save the antenna structure to the eeprom
					eeprom_save_ant_structure(computer_comm.rx_buffer_start[1], antenna_ptr);

					//Reset the content of the antenna_ptr
					memset(antenna_ptr,0,sizeof(struct_antenna));
					computer_interface_send_ack();
					break;
				default:
					computer_interface_send_nack();
					break;
			}
		}
		else if (computer_comm.command == CTRL_SET_RX_ANT_DATA) {
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_RX_ANT_DATA_TEXT:
					rx_antenna_ptr->name_length[(int)computer_comm.rx_buffer_start[2]] = computer_comm.rx_buffer_start[3];
					
					if (computer_comm.rx_buffer_start[3] == 0)
						rx_antenna_ptr->name[(int)computer_comm.rx_buffer_start[2]][0] = 0;
					else
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							rx_antenna_ptr->name[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_RX_ANT_DATA_ANT_OUT_STR: 
					//Antenna index bit is use for which combination that should be chosen
					rx_antenna_ptr->output_length[(int)computer_comm.rx_buffer_start[1]] = computer_comm.rx_buffer_start[2];
					
					for (int i=0;i<computer_comm.rx_buffer_start[2];i++) {
						rx_antenna_ptr->output_str[(int)computer_comm.rx_buffer_start[1]][i] = computer_comm.rx_buffer_start[3+i];
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_RX_ANT_DATA_BAND_OUT_STR:
					rx_antenna_ptr->band_output_length[(int)computer_comm.rx_buffer_start[1]] = computer_comm.rx_buffer_start[2];
					
					for (int i=0;i<computer_comm.rx_buffer_start[2];i++) {
						rx_antenna_ptr->band_output_str[(int)computer_comm.rx_buffer_start[1]][i] = computer_comm.rx_buffer_start[3+i];
					}

					computer_interface_send_ack();
					break;
				case CTRL_SET_RX_ANT_DATA_SAVE:
					eeprom_save_rx_ant_structure(rx_antenna_ptr);
					
					computer_interface_send_ack();
					break;
				default:
					computer_interface_send_nack();
					break;					
			}
		}
		else if (computer_comm.command == CTRL_CREATE_EEPROM_TABLE) {
			eeprom_create_table();
			
			computer_interface_send_ack();
		}
		else if (computer_comm.command == CTRL_SET_RADIO_SETTINGS) {
			/* Data structure for changing settings in the radio interface
			   byte 0   = sub command, see CTRL_SET_RADIO_SETTINGS_X defines
				 byte 1..x = data
			*/
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_RADIO_SETTINGS_ALL:
					radio_interface_set_interface(computer_comm.rx_buffer_start[1]);
					radio_interface_set_serial_mode(computer_comm.rx_buffer_start[2]);
					radio_interface_set_baudrate(computer_comm.rx_buffer_start[3]);
					radio_interface_set_stopbits(computer_comm.rx_buffer_start[4]);
					radio_interface_set_civ_addr(computer_comm.rx_buffer_start[5]);
					radio_interface_set_poll_interval(computer_comm.rx_buffer_start[6]);
					radio_interface_set_ptt_mode(computer_comm.rx_buffer_start[7]);
					radio_interface_set_ptt_input(computer_comm.rx_buffer_start[8]);
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_RADIO_SETTINGS_SAVE:
					radio_interface_save_eeprom();
					computer_interface_send_ack();
				default:	//If sub command not found
					computer_interface_send_nack();
					break;
			}
		}
		else if (computer_comm.command == CTRL_SET_DEVICE_SETTINGS) {
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_DEVICE_SETTINGS_ADDRESS:
					settings_ptr->network_address = computer_comm.rx_buffer_start[1];
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_NR_NODES:
					settings_ptr->network_device_count = computer_comm.rx_buffer_start[1];
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_DEVICE_IS_MASTER:
					settings_ptr->network_device_is_master = computer_comm.rx_buffer_start[1];
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_PTT_INPUTS:
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS:
					for (unsigned char i=0;i<17;i++) {
						settings_ptr->ext_key_assignments[i] = computer_comm.rx_buffer_start[i+1];
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_SAVE:
					eeprom_save_settings_structure(settings_ptr);
					
					computer_interface_send_ack();
					break;
				default:	//If sub command not found, send NACK back to the computer
					computer_interface_send_nack();
					break;
			}
		}
		else if (computer_comm.command == CTRL_SET_BAND_DATA) {
			/* 1st byte = What kind of data
				 2nd byte = Band index
			*/
			
			/*	Band limits (in data)
			   	---------------------
					byte[0..1] = low_portion_low_limit
					byte[2..3] = low_portion_high_limit
					byte[4..5] = high_portion_low_limit
					byte[6..7] = high_portion_high_limit
			*/
			
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_BAND_DATA_LIMITS:
					band_ptr->low_portion_low_limit = (int)(((int)(computer_comm.rx_buffer_start[2])<<8) + (int)computer_comm.rx_buffer_start[3]);
					band_ptr->low_portion_high_limit = (int)(((int)(computer_comm.rx_buffer_start[4])<<8) + (int)computer_comm.rx_buffer_start[5]);
					band_ptr->high_portion_low_limit = (int)(((int)(computer_comm.rx_buffer_start[6])<<8) + (int)computer_comm.rx_buffer_start[7]);
					band_ptr->high_portion_high_limit = (int)(((int)(computer_comm.rx_buffer_start[8])<<8) + (int)computer_comm.rx_buffer_start[9]);
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_BAND_DATA_LOW_OUT_STR:
					band_ptr->band_low_output_str_length = computer_comm.rx_buffer_start[2];

					for (int i=0;i<computer_comm.rx_buffer_start[2];i++)
						band_ptr->band_low_output_str[i] = computer_comm.rx_buffer_start[3+i];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_BAND_DATA_HIGH_OUT_STR:
					band_ptr->band_high_output_str_length = computer_comm.rx_buffer_start[2];
					
					for (int i=0;i<computer_comm.rx_buffer_start[2];i++)
						band_ptr->band_high_output_str[i] = computer_comm.rx_buffer_start[3+i];

					computer_interface_send_ack();
					break;
				case CTRL_SET_BAND_DATA_SAVE:
					eeprom_save_band_data(computer_comm.rx_buffer_start[1],band_ptr);
					
					//Reset the content of the band_ptr
					memset(band_ptr,0,sizeof(struct_band));
					
					computer_interface_send_ack();
					break;
				default:
					computer_interface_send_nack();
					break;
			}
		}
		else
			computer_interface_send_nack();
	
		//Clear the RX flag
		computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
		computer_comm.rx_buffer = computer_comm.rx_buffer_start;
	}
}

unsigned char computer_interface_is_active(void) {
	return((computer_comm.flags & (1<<COMPUTER_COMM_FLAG_SETUP_MODE))>>COMPUTER_COMM_FLAG_SETUP_MODE);
}

void computer_interface_activate_setup(void) {
	computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_SETUP_MODE);
	
	//Create dummy structures
	antenna_ptr = (struct_antenna *)malloc(sizeof(struct_antenna));
	rx_antenna_ptr = (struct_rx_antennas *)malloc(sizeof(struct_rx_antennas));
	band_ptr = (struct_band *)malloc(sizeof(struct_band));
	settings_ptr = (struct_setting *)malloc(sizeof(struct_setting));
}

void computer_interface_deactivate_setup(void) {
	computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_SETUP_MODE);
	
	free(antenna_ptr);
	free(rx_antenna_ptr);
	free(band_ptr);
}

ISR(SIG_USART1_DATA) {
	
}

ISR(SIG_USART1_RECV) {
	unsigned char data = UDR1;

	//Check if the openASC is in setup mode
	if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_SETUP_MODE)) {
	
		//Has a preamble been found?
		if (computer_comm.flags & (1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE)) {
			//Check if the data is a postamble
			if ((data == COMPUTER_COMM_POSTAMBLE) && ((computer_comm.count-2) == computer_comm.length)) {
				computer_comm.rx_buffer = computer_comm.rx_buffer_start;
				
				computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
				computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE);
			}
			else {
				switch(computer_comm.count) {
					case 0:	
						computer_comm.command = data;
						break;
					case 1: 
						computer_comm.length += data;
						break;
					default:
						*(computer_comm.rx_buffer++) = data;
						break;
				}

				computer_comm.count++;
			}
		}
		else if ((computer_comm.rx_buffer - computer_comm.rx_buffer_start > 0) && (data == COMPUTER_COMM_PREAMBLE)
						&& (*(computer_comm.rx_buffer-1) == COMPUTER_COMM_PREAMBLE)) {
			computer_comm.rx_buffer = computer_comm.rx_buffer_start;
			computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_DATA_IN_RX_BUF);
			computer_comm.flags |= (1<<COMPUTER_COMM_FLAG_FOUND_PREAMBLE);
			
			computer_comm.count = 0;
			computer_comm.length = 0;
		}
		else {
			if ((computer_comm.rx_buffer - computer_comm.rx_buffer_start) >= COMPUTER_RX_BUFFER_LENGTH) {
				computer_comm.rx_buffer = computer_comm.rx_buffer_start;
			}
			else {
				*(computer_comm.rx_buffer++) = data;
			}
		}
	}
}
