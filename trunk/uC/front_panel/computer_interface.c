/*! \file front_panel/computer_interface.c
 *  \ingroup front_panel_group
 *  \brief Interface towards the computer
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/computer_interface.c" \endcode
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
#include "sequencer.h"

//! The length of the computer RX BUFFER
#define COMPUTER_RX_BUFFER_LENGTH	128

//! The length of the computer RX BUFFER
#define COMPUTER_TX_BUFFER_LENGTH	20

//! The fixed size of the computer interface structure (PREAMBLE, POSTAMBLE etc)
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
//! CTRL command: Reboot the device
#define CTRL_REBOOT												0x02
//! CTRL command: Retrieve the firmware revision
#define CTRL_GET_FIRMWARE_REV							0x03
//! This function just replies with the same command, this is so we can see when something has been finished
#define CTRL_DONE													0x04

//! CTRL section: Set the time of the realtime clock
#define CTRL_SET_TIME  										0x10
//! CTRL section: Set the TX antenna settings
#define CTRL_SET_ANT_DATA									0x11
//! CTRL section: Create an EEPROM table
#define CTRL_CREATE_EEPROM_TABLE					0x12
//! CTRL section: Set the RX antenna settings
#define CTRL_SET_RX_ANT_DATA							0x13
//! CTRL section: Set the radio settings
#define CTRL_SET_RADIO_SETTINGS						0x14
//! CTRL section: Set the device settings
#define CTRL_SET_DEVICE_SETTINGS					0x15
//! CTRL section: Set the band data settings
#define CTRL_SET_BAND_DATA								0x16
//! CTRL section: Set the external input settings
#define CTRL_SET_EXT_INPUT								0x17
//! CTRL section: Set the sequencer settings
#define CTRL_SET_SEQUENCER_SETTINGS				0x18

//! CTRL command: Save the radio settings
#define CTRL_SET_RADIO_SETTINGS_SAVE	0x01
//! CTRL command: Set all antenna settings
#define CTRL_SET_RADIO_SETTINGS_ALL		0x02

//! CTRL command: Save the antenna information data to the EEPROM
#define CTRL_SET_ANT_DATA_SAVE						0x01
//! CTRL command: Set the antenna text
#define CTRL_SET_ANT_DATA_TEXT						0x02
//! CTRL command: Set the antenna sub menu type
#define CTRL_SET_ANT_DATA_SUB_MENU_TYPE		0x03
//! CTRL command: Set the antenna flags
#define CTRL_SET_ANT_DATA_ANT_FLAGS				0x04
//! CTRL command: Set the output combination allowed
#define CTRL_SET_ANT_DATA_COMB_ALLOWED		0x05
//! CTRL command: Set the antenna output str
#define CTRL_SET_ANT_DATA_ANT_OUT_STR			0x06
//! CTRL command: Set the rotator information
#define CTRL_SET_ANT_ROTATOR_DATA					0x07
//! CTRL command: Set the default antenna index
#define CTRL_SET_ANT_DEFAULT_INDEX				0x08
//! CTRL command: Set the sub menu data
#define CTRL_SET_ANT_SUB_MENU_DATA				0x09
//! CTRL command: Set the sub menu data, text
#define CTRL_SET_ANT_SUB_MENU_TEXT				0x0A
//! CTRL command: Set the sub menu data, output str
#define CTRL_SET_ANT_SUB_MENU_OUTPUT_STR  0x0B

//! CTRL command: Set the band data limits
#define CTRL_SET_BAND_DATA_LIMITS					0x01
//! CTRL command: Set the band low portion output str
#define CTRL_SET_BAND_DATA_LOW_OUT_STR		0x02
//! CTRL command: Set the band high portion output str
#define CTRL_SET_BAND_DATA_HIGH_OUT_STR		0x03
//! CTRL command: Save the band data settings
#define CTRL_SET_BAND_DATA_SAVE						0x07

//! CTRL command: Set the RX antenna text
#define CTRL_SET_RX_ANT_DATA_TEXT						0x01
//! CTRL command: Set the RX antenna output str
#define CTRL_SET_RX_ANT_DATA_ANT_OUT_STR		0x02
//! CTRL command: Set the RX antenna band output str
#define CTRL_SET_RX_ANT_DATA_BAND_OUT_STR		0x03
//! CTRL command: Save the RX antenna settings
#define CTRL_SET_RX_ANT_DATA_SAVE						0x07

//! CTRL command: Network settings
#define CTRL_SET_DEVICE_SETTINGS_NETWORK		0x01
//! CTRL command: Powermeter settings
#define CTRL_SET_POWERMETER_SETTINGS				0x02
//! CTRL command: Various settings
#define CTRL_SET_DEVICE_SETTINGS_OTHER			0x03
//! CTRL command: External input settings
#define CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS	0x04
//! CTRL command: Save data to eeprom
#define CTRL_SET_DEVICE_SETTINGS_SAVE				0x07

//! CTRL command: Save the sequencer settings
#define CTRL_SET_SEQUENCER_SAVE					0x01
//! CTRL command: Set the sequencer footswitch input values
#define CTRL_SET_SEQUENCER_FOOTSWITCH		0x02
//! CTRL command: Set the sequencer computer input values
#define CTRL_SET_SEQUENCER_COMPUTER			0x03
//! CTRL command: Set the sequencer radio sense input values
#define CTRL_SET_SEQUENCER_RADIO_SENSE	0x04

//! Computer interface communication struct
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
	//! Flags for computer comm, defined in this file
	unsigned char flags;
	//! The current command
	unsigned char command;
	//! Length of the data field
	unsigned char length;
	//! Current byte count
	unsigned int count;
} computer_comm_struct;

//! Computer communication structure
computer_comm_struct computer_comm;

//! Pointer to an area which we create space when configuring the antenna data
struct_antenna *antenna_ptr;

//! Pointer to an area which we create space when configuring the rx antenna data
struct_rx_antennas *rx_antenna_ptr;

//! Pointer to an area which we create space when configuring the band data
struct_band *band_ptr;

//! Pointer to an area which we create space when configuring the settings
struct_setting *settings_ptr;

//! Pointer to an area which we create space when configuring the ptt_sequencer
struct_ptt *ptt_sequencer_ptr;

//! Pointer to an area which we crate space when configuring the radio settings
struct_radio_settings *radio_settings_ptr;

//! Pointer to an area which we crate space when configuring the sub menu (array)
struct_sub_menu_array *sub_menu_array_ptr[4];
	
struct_sub_menu_stack *sub_menu_stack_ptr[4];

//! Address which we call when we wish to reboot the device (jumps to the bootloader area)
void (*bootloader_start)(void) = (void *)0x1FE00;

//! \brief Initialize the communication interface towards the computer. Will initialize buffers etc.
void computer_interface_init(void) {
	// Setup the RX buffer
	computer_comm.rx_buffer = (char *)malloc(COMPUTER_RX_BUFFER_LENGTH);
	computer_comm.rx_buffer_start = computer_comm.rx_buffer;
	
	computer_comm.tx_buffer = (char *)malloc(COMPUTER_TX_BUFFER_LENGTH);
	computer_comm.tx_buffer_start = computer_comm.tx_buffer;
	
	computer_comm.data_in_tx_buffer = 0;
}

//! \brief Function which will send data from the tx_buffer to the uart
void computer_interface_send_data(void) {
	if (computer_comm.data_in_tx_buffer == 1) {
		for (int i=0;i<computer_comm.tx_buffer_length;i++)
			usart1_transmit(computer_comm.tx_buffer_start[i]);
		
		computer_comm.tx_buffer = computer_comm.tx_buffer_start;
		computer_comm.tx_buffer_length = 0;

		computer_comm.data_in_tx_buffer = 0;
	}
}

/*! \brief Function which will add data to the tx_buffer. Function also sets the flag indicating that the data should be sent.
 *  \param command The command we wish to sendchar(
 *  \param length Number of bytes of data to be sent (only size of the data variable)
 *  \param data The data we wish to send */
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

/*! \brief Function which will add an ACK message to the tx_buffer. Also sets a flag that indicates data ready to be sent */
void computer_interface_send_ack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_ACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;
	
	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

/*! \brief Function which will add an NACK message to the tx_buffer. Also sets a flag that indicates data ready to be sent */
void computer_interface_send_nack(void) {
	computer_comm.tx_buffer[0] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[1] = COMPUTER_COMM_PREAMBLE;
	computer_comm.tx_buffer[2] = COMPUTER_COMM_NACK;
	computer_comm.tx_buffer[3] = 0x00;
	computer_comm.tx_buffer[4] = COMPUTER_COMM_POSTAMBLE;

	computer_comm.tx_buffer_length = 5;
	computer_comm.data_in_tx_buffer = 1;
}

/*! \brief Function which will parse the data in the rx_buffer and process the command */
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
			
			bootloader_start();
			
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
					antenna_ptr->sub_menu_type[0] = computer_comm.rx_buffer_start[1];
					antenna_ptr->sub_menu_type[1] = computer_comm.rx_buffer_start[2];
					antenna_ptr->sub_menu_type[2] = computer_comm.rx_buffer_start[3];
					antenna_ptr->sub_menu_type[3] = computer_comm.rx_buffer_start[4];
					
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
					ptr_pos = 2;
					
					for (unsigned char i=0;i<4;i++) {
						antenna_ptr->rotator_addr[i] = computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_sub_addr[i] = computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_max_rotation[i] = computer_comm.rx_buffer[ptr_pos++] << 8;
						antenna_ptr->rotator_max_rotation[i] += computer_comm.rx_buffer[ptr_pos++];
						antenna_ptr->rotator_min_heading[i] = computer_comm.rx_buffer[ptr_pos++] << 8;
						antenna_ptr->rotator_min_heading[i] += computer_comm.rx_buffer[ptr_pos++];
					}

					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DEFAULT_INDEX:
					antenna_ptr->default_antenna = computer_comm.rx_buffer[2];
					computer_interface_send_ack();
					break;	
				case CTRL_SET_ANT_SUB_MENU_DATA:
					if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer[1]] == SUBMENU_VERT_ARRAY) {
						sub_menu_array_ptr[(int)computer_comm.rx_buffer[1]]->direction_count = computer_comm.rx_buffer[2];
					}
					else if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer[1]] == SUBMENU_STACK) {
						sub_menu_stack_ptr[(int)computer_comm.rx_buffer[1]]->comb_count = computer_comm.rx_buffer[2];
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_SUB_MENU_TEXT:
					if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer_start[1]] == SUBMENU_VERT_ARRAY) {
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							sub_menu_array_ptr[(int)computer_comm.rx_buffer_start[1]]->direction_name[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
						
						sub_menu_array_ptr[(int)computer_comm.rx_buffer_start[1]]->direction_name[(int)computer_comm.rx_buffer_start[2]][(int)computer_comm.rx_buffer_start[3]] = 0;
					}
					else if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer_start[1]] == SUBMENU_STACK) {
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							sub_menu_stack_ptr[(int)computer_comm.rx_buffer_start[1]]->comb_name[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
						
						sub_menu_stack_ptr[(int)computer_comm.rx_buffer_start[1]]->comb_name[(int)computer_comm.rx_buffer_start[2]][(int)computer_comm.rx_buffer_start[3]] = 0;	
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_SUB_MENU_OUTPUT_STR:
					if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer_start[1]] == SUBMENU_VERT_ARRAY) {
						sub_menu_array_ptr[(int)computer_comm.rx_buffer_start[1]]->output_str_dir_length[(int)computer_comm.rx_buffer_start[2]] = computer_comm.rx_buffer_start[3];
						
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							sub_menu_array_ptr[(int)computer_comm.rx_buffer_start[1]]->output_str_dir[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
					}
					else if (antenna_ptr->sub_menu_type[(int)computer_comm.rx_buffer_start[1]] == SUBMENU_STACK) {
						sub_menu_stack_ptr[(int)computer_comm.rx_buffer_start[1]]->output_str_comb_length[(int)computer_comm.rx_buffer_start[2]] = computer_comm.rx_buffer_start[3];
						
						for (unsigned char i=0;i<computer_comm.rx_buffer_start[3];i++)
							sub_menu_stack_ptr[(int)computer_comm.rx_buffer_start[1]]->output_str_comb[(int)computer_comm.rx_buffer_start[2]][i] = computer_comm.rx_buffer_start[4+i];
					}
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_ANT_DATA_SAVE:
					//Save the antenna structure to the eeprom
					eeprom_save_ant_structure(computer_comm.rx_buffer_start[1]+1, antenna_ptr);
					
					//Routines to save the SUB MENU data to the EEPROM
					for (unsigned char ant_index=0;ant_index<4;ant_index++) {
						if (antenna_ptr->sub_menu_type[ant_index] == SUBMENU_VERT_ARRAY) {
							eeprom_save_ant_sub_menu_array_structure(computer_comm.rx_buffer_start[1]+1, ant_index, sub_menu_array_ptr[ant_index]);
						}
						else if (antenna_ptr->sub_menu_type[ant_index] == SUBMENU_STACK) {
							eeprom_save_ant_sub_menu_stack_structure(computer_comm.rx_buffer_start[1]+1, ant_index, sub_menu_stack_ptr[ant_index]);
						}
					}

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
					
					rx_antenna_ptr->name[(int)computer_comm.rx_buffer_start[2]][(int)computer_comm.rx_buffer_start[3]] = 0;
					
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
			// Data structure for changing settings in the radio interface
			// byte 0   = sub command, see CTRL_SET_RADIO_SETTINGS_X defines
			// byte 1..x = data

			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_RADIO_SETTINGS_ALL:
					radio_settings_ptr->radio_model = computer_comm.rx_buffer_start[1];
					radio_settings_ptr->interface_type = computer_comm.rx_buffer_start[2];
					radio_settings_ptr->cat_enabled = computer_comm.rx_buffer_start[3];
					radio_settings_ptr->baudrate = computer_comm.rx_buffer_start[4];
					radio_settings_ptr->stopbits = computer_comm.rx_buffer_start[5];
					radio_settings_ptr->civ_addr = computer_comm.rx_buffer_start[6];
					radio_settings_ptr->poll_interval = computer_comm.rx_buffer_start[7];
					radio_settings_ptr->ptt_input = computer_comm.rx_buffer_start[8];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_RADIO_SETTINGS_SAVE:
					eeprom_save_radio_settings_structure(radio_settings_ptr);
					
					computer_interface_send_ack();
					break;
				default:	//If sub command not found
					computer_interface_send_nack();
					break;
			}
		}
		else if (computer_comm.command == CTRL_SET_SEQUENCER_SETTINGS) {
				// Bit 0 = Footswitch
				// Bit 1 = Radio sense lower floor
				// Bit 2 = Radio sense upper floor
				//	Bit 3 = Computer RTS
				//	Bit 4 = Inverted radio sense
				//	Bit 5 = Inverted Computer RTS 
				//	Bit 6 = Inhibit polarity (0=active low, 1=active high)
						
				//	unsigned char ptt_input;
				
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_SEQUENCER_FOOTSWITCH:
					ptt_sequencer_ptr->footswitch.radio_pre_delay = computer_comm.rx_buffer_start[1];
					ptt_sequencer_ptr->footswitch.radio_post_delay = computer_comm.rx_buffer_start[2];
					ptt_sequencer_ptr->footswitch.amp_pre_delay = computer_comm.rx_buffer_start[3];
					ptt_sequencer_ptr->footswitch.amp_post_delay = computer_comm.rx_buffer_start[4];
					ptt_sequencer_ptr->footswitch.inhibit_pre_delay = computer_comm.rx_buffer_start[5];
					ptt_sequencer_ptr->footswitch.inhibit_post_delay = computer_comm.rx_buffer_start[6];
					ptt_sequencer_ptr->footswitch.antennas_post_delay = computer_comm.rx_buffer_start[7];
					ptt_sequencer_ptr->footswitch.active = computer_comm.rx_buffer_start[8];
					ptt_sequencer_ptr->ptt_input |= computer_comm.rx_buffer_start[9];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_SEQUENCER_COMPUTER:
					ptt_sequencer_ptr->computer.radio_pre_delay = computer_comm.rx_buffer_start[1];
					ptt_sequencer_ptr->computer.radio_post_delay = computer_comm.rx_buffer_start[2];
					ptt_sequencer_ptr->computer.amp_pre_delay = computer_comm.rx_buffer_start[3];
					ptt_sequencer_ptr->computer.amp_post_delay = computer_comm.rx_buffer_start[4];
					ptt_sequencer_ptr->computer.inhibit_pre_delay = computer_comm.rx_buffer_start[5];
					ptt_sequencer_ptr->computer.inhibit_post_delay = computer_comm.rx_buffer_start[6];
					ptt_sequencer_ptr->computer.antennas_post_delay = computer_comm.rx_buffer_start[7];
					ptt_sequencer_ptr->computer.active = computer_comm.rx_buffer_start[8];
					ptt_sequencer_ptr->ptt_input |= computer_comm.rx_buffer_start[9];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_SEQUENCER_RADIO_SENSE:
					ptt_sequencer_ptr->radio_sense.radio_pre_delay = computer_comm.rx_buffer_start[1];
					ptt_sequencer_ptr->radio_sense.radio_post_delay = computer_comm.rx_buffer_start[2];
					ptt_sequencer_ptr->radio_sense.amp_pre_delay = computer_comm.rx_buffer_start[3];
					ptt_sequencer_ptr->radio_sense.amp_post_delay = computer_comm.rx_buffer_start[4];
					ptt_sequencer_ptr->radio_sense.antennas_post_delay = computer_comm.rx_buffer_start[7];
					ptt_sequencer_ptr->radio_sense.active = computer_comm.rx_buffer_start[8];
					ptt_sequencer_ptr->ptt_input |= computer_comm.rx_buffer_start[9];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_SEQUENCER_SAVE:
					eeprom_save_ptt_data(ptt_sequencer_ptr);
					
					computer_interface_send_ack();
					break;
				default:
					computer_interface_send_nack();
					break;
			}
		}			
		else if (computer_comm.command == CTRL_SET_DEVICE_SETTINGS) {
			switch(computer_comm.rx_buffer_start[0]) {
				case CTRL_SET_DEVICE_SETTINGS_NETWORK:
					settings_ptr->network_address = computer_comm.rx_buffer_start[1];
					settings_ptr->network_device_count = computer_comm.rx_buffer_start[2];
					settings_ptr->network_device_is_master = computer_comm.rx_buffer_start[3];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_EXT_INPUTS:
					for (unsigned char i=0;i<17;i++) {
						settings_ptr->ext_key_assignments[i] = computer_comm.rx_buffer_start[i+1];
					}
					
					//Set the aux button functions
					settings_ptr->aux1_button_func = computer_comm.rx_buffer_start[18];
					settings_ptr->aux2_button_func = computer_comm.rx_buffer_start[19];
					
					computer_interface_send_ack();
					break;
				case CTRL_SET_DEVICE_SETTINGS_OTHER:
					settings_ptr->ptt_interlock_input = computer_comm.rx_buffer_start[1];
					
					computer_interface_send_ack();
					break;					
				case CTRL_SET_POWERMETER_SETTINGS:
					for (unsigned char i=0;i<9;i++)
						settings_ptr->powermeter_address[i] = computer_comm.rx_buffer_start[1+i];
					
					settings_ptr->powermeter_vswr_limit = (computer_comm.rx_buffer_start[10] << 8);
					settings_ptr->powermeter_vswr_limit += computer_comm.rx_buffer_start[11];
					settings_ptr->powermeter_update_rate_text = (computer_comm.rx_buffer_start[12] << 8);
					settings_ptr->powermeter_update_rate_text += computer_comm.rx_buffer_start[13];
					settings_ptr->powermeter_update_rate_bargraph = (computer_comm.rx_buffer_start[14] << 8);
					settings_ptr->powermeter_update_rate_bargraph += computer_comm.rx_buffer_start[15];
					
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
			// 1st byte = What kind of data
			//	 2nd byte = Band index
			//
			
			//	Band limits (in data)
			//   	---------------------
			//	byte[0..1] = low_portion_low_limit
      //  byte[2..3] = low_portion_high_limit
			//	byte[4..5] = high_portion_low_limit
			//	byte[6..7] = high_portion_high_limit
			
			
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
					eeprom_save_band_data(computer_comm.rx_buffer_start[1]+1,band_ptr);
					
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

/*! \brief Retrieve the status if the computer interface is active
 *  \return 1 if it is active, 0 otherwise */
unsigned char computer_interface_is_active(void) {
	return((computer_comm.flags & (1<<COMPUTER_COMM_FLAG_SETUP_MODE))>>COMPUTER_COMM_FLAG_SETUP_MODE);
}

/*! \brief Activate the setup mode of the device. Will mainly just create various buffers needed to store settings */
void computer_interface_activate_setup(void) {
	computer_comm.flags = (1<<COMPUTER_COMM_FLAG_SETUP_MODE);
	
	//Create dummy structures
	antenna_ptr = (struct_antenna *)malloc(sizeof(struct_antenna));
  //antenna_ptr = antenna_ctrl_get_antenna_ptr();
	rx_antenna_ptr = (struct_rx_antennas *)malloc(sizeof(struct_rx_antennas));
	band_ptr = (struct_band *)malloc(sizeof(struct_band));
	settings_ptr = (struct_setting *)malloc(sizeof(struct_setting));
	ptt_sequencer_ptr = (struct_ptt *)malloc(sizeof(struct_ptt));
	radio_settings_ptr = (struct_radio_settings *)malloc(sizeof(struct_radio_settings));
	
	for (unsigned char i=0;i<4;i++)
		sub_menu_array_ptr[i] = (struct_sub_menu_array *)malloc(sizeof(struct_sub_menu_array));
	
	for (unsigned char i=0;i<4;i++)
		sub_menu_stack_ptr[i] = (struct_sub_menu_stack *)malloc(sizeof(struct_sub_menu_stack));
	
	ptt_sequencer_ptr->ptt_input = 0;
}

/*! \brief Function which will deactivate the computer setup mode, this will clear up memory space of the allocated buffers in the computer_interface_activate_setup() function */
void computer_interface_deactivate_setup(void) {
	computer_comm.flags &= ~(1<<COMPUTER_COMM_FLAG_SETUP_MODE);
	
	free(ptt_sequencer_ptr);
	free(antenna_ptr);
	free(rx_antenna_ptr);
	free(band_ptr);
	free(radio_settings_ptr);
	free(settings_ptr);
	
	for (unsigned char i=0;i<4;i++)
		free(sub_menu_array_ptr[i]);
	
	for (unsigned char i=0;i<4;i++)
		free(sub_menu_stack_ptr[i]);
	
}

ISR(SIG_USART1_DATA) {
	
}

/*! \brief Interrupt when a character is received over the UART. If computer setup mode is active it will parse the incoming data, otherwise it is used for CAT control */
ISR(SIG_USART1_RECV) {
	char data = UDR1;

	//Check if the openASC is in setup mode
	if (computer_interface_is_active()) {
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
	else {
		usart3_transmit(data);
	}
}
