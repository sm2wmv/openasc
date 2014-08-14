/*! \file front_panel/eeprom.c
 *  \brief EEPROM functions
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/eeprom.c" \endcode
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
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/eeprom.h>

#include "eeprom_m24.h"
#include "eeprom.h"
#include "../global.h"
#include "antenna_ctrl.h"
#include "band_ctrl.h"
#include "main.h"
#include "radio_interface.h"
#include "../global.h"

//TODO: Fix the read block command so that reading settings from the eeprom goes faster

//! EEPROM table which is a description of the location of different structures in the eeprom
struct_eeprom_table	eeprom_table;

/*! Will read the startup byte from the EEPROM, which does indicate if the unit has been started
    before or not
	\return The status of the startup byte */
unsigned char eeprom_read_startup_byte(void) {
	return(eeprom_read_byte((unsigned char *)EEPROM_STARTUP_BYTE_ADDR));
}

/*! \brief This function will write a byte in the EEPROM so we can keep track of if the unit has ever been started 
 *  \param val What value we wish to write to the EEPROM */
void eeprom_write_startup_byte(unsigned char val) {
	eeprom_write_byte((unsigned char *)EEPROM_STARTUP_BYTE_ADDR,val);
}

//! \brief Temporary crap for debug */
void eeprom_print(void) {
	printf("ANT 1: %i\n",eeprom_table.antenna[0]);
	printf("ANT 2: %i\n",eeprom_table.antenna[1]);
	printf("ANT 3: %i\n",eeprom_table.antenna[2]);
	printf("ANT 4: %i\n",eeprom_table.antenna[3]);
	printf("ANT 5: %i\n",eeprom_table.antenna[4]);
}

/*! \brief Read the map of the EEPROM */
void eeprom_read_table(void) {	
	unsigned char *eeprom_table_ptr = (unsigned char *)&eeprom_table;
	
	for (unsigned int i=0;i<sizeof(eeprom_table);i++)
		*(eeprom_table_ptr+i) = eeprom_m24_read_byte(i);
	
	//eeprom_m24_read_block(0,sizeof(eeprom_table), (unsigned char *)&eeprom_table);
}

/*! \brief Returns the antenna struct for a specific band
 *  \param data Where the data should be saved
 *  \param band Which band you wish to get the pointer */
void eeprom_get_antenna_data(struct_antenna *data, unsigned char band) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *ant_data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_antenna);i++)
		*(ant_data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna[band-1]+i);
}

/*! \brief Returns the rx antenna data
 *  \param data Where the data should be saved */
void eeprom_get_rx_antenna_data(struct_rx_antennas *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *ant_data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_rx_antennas);i++)
		*(ant_data_ptr+i) = eeprom_m24_read_byte(eeprom_table.rx_antennas+i);
}

/*! \brief Returns the band data
 *  \param band Which band we wish to retrieve the data from
 *  \param data Where the data should be saved */
void eeprom_get_band_data(unsigned char band, struct_band *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_band);i++)
		*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.band[band-1]+i);
}

/*! \brief get the radio settings from the eeprom
 *  \param data Where the data should be saved */
void eeprom_get_radio_settings_structure(struct_radio_settings *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_radio_settings);i++)
		*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.radio_settings+i);
}

/*! \brief get the settings from the eeprom
 *  \param data Where the data should be saved */
void eeprom_get_settings_structure(struct_setting *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_setting);i++)
		*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.settings+i);
}

/*! \brief Creates the eeprom table which is a map over the eeprom data */	
void eeprom_create_table(void) {
	eeprom_table.struct_size = sizeof(struct_eeprom_table);
	
	eeprom_table.antenna[0] = 256;
	eeprom_table.antenna[1] = 768;
	eeprom_table.antenna[2] = 1280;
	eeprom_table.antenna[3] = 1792;
	eeprom_table.antenna[4] = 2304;
	eeprom_table.antenna[5] = 2816;
	eeprom_table.antenna[6] = 3328;
	eeprom_table.antenna[7] = 3840;
	eeprom_table.antenna[8] = 4352;
		
	eeprom_table.rx_antennas = 4864;
	
	eeprom_table.radio_settings = 8192;
	eeprom_table.struct_ptt = 8320;

	eeprom_table.band[0] =  8448;
	eeprom_table.band[1] =  8576;
	eeprom_table.band[2] =  8704;
	eeprom_table.band[3] =  8832;
	eeprom_table.band[4] =  8960;
	eeprom_table.band[5] =  9088;
	eeprom_table.band[6] =  9216;
	eeprom_table.band[7] =  9344;
	eeprom_table.band[8] =  9472;

	eeprom_table.settings = 9600;
	eeprom_table.runtime_settings = 9728;
	
	unsigned int curr_addr = 9856;
	
	for (unsigned char i=0;i<9;i++) {
		eeprom_table.antenna1_sub_menu[i] = curr_addr;
		curr_addr += 256;
		eeprom_table.antenna2_sub_menu[i] = curr_addr;
		curr_addr += 256;
		eeprom_table.antenna3_sub_menu[i] = curr_addr;
		curr_addr += 256;
		eeprom_table.antenna4_sub_menu[i] = curr_addr;
		curr_addr += 256;
	}

	//Save the table to the eeprom, starting from address 0
	eeprom_m24_write_block(0, sizeof(eeprom_table), (unsigned char *)&eeprom_table);
}

/*! \brief Save the runtime_settings structure to the eeprom
 *  \param content The data to be saved */
void eeprom_save_runtime_settings(struct_runtime_settings *content) {
	eeprom_m24_write_block(eeprom_table.runtime_settings, sizeof(struct_runtime_settings), (unsigned char *)content);
}

/*! \brief Get the ptt structure from the EEPROM
 *  \param data A pointer where to store the data*/
void eeprom_get_ptt_data(struct_ptt *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_ptt);i++)
		*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.struct_ptt+i);
}

/*! \brief Get the runtime_settings structure from the EEPROM
 *  \param data A pointer where to store the data*/
void eeprom_get_runtime_settings(struct_runtime_settings *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	for (unsigned int i=0;i<sizeof(struct_runtime_settings);i++)
		*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.runtime_settings+i);
}

/*! \brief Get the struct_sub_menu_array structure from the EEPROM
 *  \param band_index Which band we wish to retrieve the sub menu from 
 *  \param ant_index Which antenna we wish to get the sub menu from
 *  \param data Pointer to where we wish to store the data */
void eeprom_get_ant_sub_menu_array_structure(unsigned char band_index, unsigned char ant_index, struct_sub_menu_array *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	if (ant_index == 0) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_array);i++) {
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna1_sub_menu[band_index-1]+i);
		}
	}
	else if (ant_index == 1) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_array);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna2_sub_menu[band_index-1]+i);
	}
	else if (ant_index == 2) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_array);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna3_sub_menu[band_index-1]+i);
	}
	else	if (ant_index == 3) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_array);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna4_sub_menu[band_index-1]+i);
	}
}

/*! \brief Get the struct_sub_menu_stack structure from the EEPROM
 *  \param band_index Which band we wish to retrieve the sub menu from 
 *  \param ant_index Which antenna we wish to get the sub menu from
 *  \param data Pointer to where we wish to store the data */
void eeprom_get_ant_sub_menu_stack_structure(unsigned char band_index, unsigned char ant_index, struct_sub_menu_stack *data) {
	//Create a pointer which we can handle, and point it to where the data should be saved
	unsigned char *data_ptr = (unsigned char *)data;
	
	if (ant_index == 0) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_stack);i++) {
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna1_sub_menu[band_index-1]+i);
		}
	}
	else if (ant_index == 1) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_stack);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna2_sub_menu[band_index-1]+i);
	}
	else if (ant_index == 2) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_stack);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna3_sub_menu[band_index-1]+i);
	}
	else	if (ant_index == 3) {
		for (unsigned int i=0;i<sizeof(struct_sub_menu_stack);i++)
			*(data_ptr+i) = eeprom_m24_read_byte(eeprom_table.antenna4_sub_menu[band_index-1]+i);
	}
}

/*! \brief Save the antenna structure to the eeprom
 *  \param band_index Which band it is
 *  \param content The data to be saved */
void eeprom_save_ant_structure(unsigned char band_index, struct_antenna *content) {
	eeprom_m24_write_block(eeprom_table.antenna[band_index-1], sizeof(struct_antenna), (unsigned char *)content);
}

/*! \brief Save the rx antenna structure to the eeprom
 *  \param data The data to save to the EEPROM */
void eeprom_save_rx_ant_structure(struct_rx_antennas *data) {
	eeprom_m24_write_block(eeprom_table.rx_antennas, sizeof(struct_rx_antennas), (unsigned char *)data);
}

/*! \brief Save the device settings to the eeprom
 * \param data The data to save to the EEPROM */
void eeprom_save_settings_structure(struct_setting *data) {
	eeprom_m24_write_block(eeprom_table.settings, sizeof(struct_setting), (unsigned char *)data);
}

/*! \brief Save the radio settings to the eeprom
 * \param data The data to save to the EEPROM */
void eeprom_save_radio_settings_structure(struct_radio_settings *data) {
	eeprom_m24_write_block(eeprom_table.radio_settings, sizeof(struct_radio_settings), (unsigned char *)data);
}

/*! \brief Save the band data to the eeprom
 * \param band Which band we wish to save the data to
 * \param data The data to save to the EEPROM */
void eeprom_save_band_data(unsigned char band, struct_band *data) {
	eeprom_m24_write_block(eeprom_table.band[band-1], sizeof(struct_band), (unsigned char *)data);
}

/*! \brief Save the band data to the eeprom
 * \param data The data to save to the EEPROM */
void eeprom_save_ptt_data(struct_ptt *data) {
	eeprom_m24_write_block(eeprom_table.struct_ptt, sizeof(struct_ptt), (unsigned char *)data);
}

/*! \brief Save the sub menu array data to the EEPROM
 *  \param band_index The band we wish to save the settings for
 *  \param ant_index The antenna index of the data
 *  \param data The data to save to the EEPROM */
void eeprom_save_ant_sub_menu_array_structure(unsigned char band_index, unsigned char ant_index, struct_sub_menu_array *data) {
	if (ant_index == 0)
		eeprom_m24_write_block(eeprom_table.antenna1_sub_menu[band_index-1], sizeof(struct_sub_menu_array), (unsigned char *)data);
	else if (ant_index == 1)
		eeprom_m24_write_block(eeprom_table.antenna2_sub_menu[band_index-1], sizeof(struct_sub_menu_array), (unsigned char *)data);
	else if (ant_index == 2)
		eeprom_m24_write_block(eeprom_table.antenna3_sub_menu[band_index-1], sizeof(struct_sub_menu_array), (unsigned char *)data);
	else if (ant_index == 3)
		eeprom_m24_write_block(eeprom_table.antenna4_sub_menu[band_index-1], sizeof(struct_sub_menu_array), (unsigned char *)data);
}

/*! \brief Save the sub menu stack data to the EEPROM
 *  \param band_index The band we wish to save the settings for
 *  \param ant_index The antenna index of the data
 *  \param data The data to save to the EEPROM */
void eeprom_save_ant_sub_menu_stack_structure(unsigned char band_index, unsigned char ant_index, struct_sub_menu_stack *data) {
	if (ant_index == 0)
		eeprom_m24_write_block(eeprom_table.antenna1_sub_menu[band_index-1], sizeof(struct_sub_menu_stack), (unsigned char *)data);
	else if (ant_index == 1)
		eeprom_m24_write_block(eeprom_table.antenna2_sub_menu[band_index-1], sizeof(struct_sub_menu_stack), (unsigned char *)data);
	else if (ant_index == 2)
		eeprom_m24_write_block(eeprom_table.antenna3_sub_menu[band_index-1], sizeof(struct_sub_menu_stack), (unsigned char *)data);
	else if (ant_index == 3)
		eeprom_m24_write_block(eeprom_table.antenna4_sub_menu[band_index-1], sizeof(struct_sub_menu_stack), (unsigned char *)data);
}