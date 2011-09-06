/*! \file front_panel/antenna_ctrl.c \brief Antenna control functions
 *  \ingroup front_panel_group
 *  \brief Antenna control functions
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/antenna_ctrl.c" \endcode
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
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

#include "antenna_ctrl.h"
#include "main.h"
#include "eeprom.h"
#include "led_control.h"
#include "band_ctrl.h"
#include "event_handler.h"

#include "../global.h"

/* Include the bus headers */
#include "../wmv_bus/bus.h"
#include "../wmv_bus/bus_rx_queue.h"
#include "../wmv_bus/bus_tx_queue.h"
#include "../wmv_bus/bus_commands.h"

//! Contains the current antenna information
struct_antenna current_antennas;

//! Contains the rx antenna information
struct_rx_antennas rx_antennas;

extern unsigned int main_flags;

//! Array which we store the current devices which we have activated antenna outputs on
unsigned char current_activated_ant_outputs[ANTENNA_OUTPUT_COMB_SIZE];
//! How many devices we have activated antenna outputs on
unsigned char current_activated_ant_outputs_length = 0;

//! Array which we store the current devices which we have rx antenna activated outputs on
unsigned char current_activated_rx_ant_outputs[RX_ANTENNA_OUTPUT_STR_LENGTH];
//! How many devices we have activated rx antenna outputs on
unsigned char current_activated_rx_ant_outputs_length = 0;

//! Array which we store the current devices which we have activated rx antenna band outputs on
unsigned char current_band_activated_outputs_rx[RX_ANTENNA_BAND_OUTPUT_STR_LENGTH];
//! How many devices we have activated rx antenna band outputs on
unsigned char current_band_activated_outputs_rx_length = 0;

//! The last antenna that was rotated (-1 = none)
char last_rotated_antenna = -1;

/*! \brief Function which goes through the different addresses of cards that have been activated and compares it to the sent in address
    \param addr The address we wish to check if it is activated 
    \return 1 if the address is in use, 0 if not */
unsigned char antenna_ctrl_check_address_in_use(unsigned char addr) {
	unsigned char ret_val = 0;
	
	for (unsigned char i=0;i<current_activated_ant_outputs_length;i++) {
		if (current_activated_ant_outputs[i] == addr)
			ret_val = 1;
	}
	
	for (unsigned char i=0;i<current_activated_rx_ant_outputs_length;i++) {
		if (current_activated_rx_ant_outputs[i] == addr)
			ret_val = 1;
	}	
		
	for (unsigned char i=0;i<current_band_activated_outputs_rx_length;i++) {
		if (current_band_activated_outputs_rx[i] == addr)
			ret_val = 1;
	}	
	
	return(ret_val);
}

unsigned char antenna_ctrl_get_comb_value(unsigned char antenna_comb) {
	unsigned char result = 0;

	if (antenna_comb == 1)
		result = 0;
	else if (antenna_comb == 2)
		result = 1;
	else if (antenna_comb == 4)
		result = 2;
	else if (antenna_comb == 8)
		result = 3;
	else if (antenna_comb == 3)
		result = 4;
	else if (antenna_comb == 5)
		result = 5;
	else if (antenna_comb == 9)
		result = 6;
	else if (antenna_comb == 6)
		result = 7;
	else if (antenna_comb == 10)
		result = 8;
	else if (antenna_comb == 12)
		result = 9;
	else if (antenna_comb == 7) 
		result = 10;
	else if (antenna_comb == 11)
		result = 11;
	else if (antenna_comb == 13) 
		result = 12;
	else if (antenna_comb == 14) 
		result = 13;
	else if (antenna_comb == 15) 
		result = 14;
		
	return(result);
}

/*! \brief Retrieve if a certain antenna combination is allowed 
 *  \param antenna_comb The antenna configuration you wish to check 
 *  \return 1 if the combination is allowed, 0 if it is not allowed */
unsigned char antenna_ctrl_comb_allowed(unsigned char antenna_comb) {
	if (antenna_comb == 0)
		return(1);
	else if (current_antennas.antenna_comb_allowed & (1<<antenna_ctrl_get_comb_value(antenna_comb))) {
		return(1);
	}
	
	return(0);
}

char antenna_ctrl_get_rotates_char(unsigned char index) {
  if (current_antennas.rotator_flags[index] & (1<<FLAG_ROTATES_OVER_SOUTH))
    return('S');

  return('N');
}

/*! \brief This function will go through a parameter with addresses and send a command to it
 *  \param addresses The list of addresses to send CMD to
 *  \param length The length of the address list
 *  \param cmd The command we wish to send to the boards in the address list */
void antenna_ctrl_deactivate_outputs(unsigned char *addresses, unsigned char length, unsigned char cmd) {
	if (length > 0) {
		for (unsigned char i=0;i<length;i++) {
			if (addresses[i] != 0x00)
				bus_add_tx_message(bus_get_address(), addresses[i], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), cmd, 0, 0);
			else
				internal_comm_add_tx_message(cmd, 0 , 0);
		}
	}
}

/*! \brief This function returns the selected antenna combination 
 * \return The selected antenna combination, for example 1 means antenna 1, 3 means antenna 1 and 2 (binary representation) */
unsigned char antenna_ctrl_antenna_selected(void) {
	return(status.selected_ant & 0x0F);
}

unsigned char antenna_ctrl_rx_antenna_selected(void) {
	return(status.selected_rx_antenna);
}

/*! \brief Send the output string for the current antenna to the bus */
void antenna_ctrl_send_ant_data_to_bus(void) {
	if (status.selected_ant != 0) {	
		unsigned char value = antenna_ctrl_get_comb_value(status.selected_ant);
		
		unsigned char length = current_antennas.antenna_output_length[value];
		
		if (length > 0) {
			unsigned char temp[length];
	
			unsigned char start_pos = 0;
			unsigned char count = 0;
			unsigned char addr_count=0;
	
			unsigned char i=0;
				
			antenna_ctrl_deactivate_outputs(current_activated_ant_outputs, current_activated_ant_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS);
			current_activated_ant_outputs_length = 0;
			
			while(i<length) {
				if (current_antennas.antenna_comb_output_str[value][i] == OUTPUT_ADDR_DELIMITER) {
					//Will add which address the message was sent to
					current_activated_ant_outputs[addr_count++] = current_antennas.antenna_comb_output_str[value][i+1];
					
					if (current_antennas.antenna_comb_output_str[value][i+1] != 0x00)
						bus_add_tx_message(bus_get_address(), current_antennas.antenna_comb_output_str[value][i+1], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT, count-start_pos, temp+start_pos);
					else
						internal_comm_add_tx_message(BUS_CMD_DRIVER_ACTIVATE_ANT_OUTPUT,count-start_pos, (char *)(temp+start_pos));
	
					start_pos = count;
					i++;
				} 
				else {
					temp[count] = current_antennas.antenna_comb_output_str[value][i];
					count++;
				}
				
				i++;
			}
			
			current_activated_ant_outputs_length = addr_count;
		}
	}
	else if (current_activated_ant_outputs_length > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_ant_outputs, current_activated_ant_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS);
		current_activated_ant_outputs_length = 0;
	}
}

/*! \brief Send the output string for the rx antenna to the bus 
 *  \param antenna_index The index of the antenna you wish to send the string of */
void antenna_ctrl_send_rx_ant_data_to_bus(unsigned char antenna_index) {
	unsigned char length = rx_antennas.output_length[antenna_index];
	
	if (length > 0) {
		unsigned char temp[length];
	
		unsigned char start_pos = 0;
		unsigned char count = 0;
		unsigned char addr_count=0;
	
		unsigned char i=0;
	
		antenna_ctrl_deactivate_outputs(current_activated_rx_ant_outputs, current_activated_rx_ant_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS);
		
		current_activated_rx_ant_outputs_length = 0;
		
		while(i<length) {
			if (rx_antennas.output_str[antenna_index][i] == OUTPUT_ADDR_DELIMITER) {
				//Will add which address the message was sent to
				current_activated_rx_ant_outputs[addr_count++] = rx_antennas.output_str[antenna_index][i+1];
				
				if (rx_antennas.output_str[antenna_index][i+1] != 0x00)
					bus_add_tx_message(bus_get_address(), rx_antennas.output_str[antenna_index][i+1], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT, count-start_pos, temp+start_pos);
				else
					internal_comm_add_tx_message(BUS_CMD_DRIVER_ACTIVATE_RX_ANT_OUTPUT,count-start_pos, (char *)(temp+start_pos));
			
				start_pos = count;
				i++;
			}
			else {
				temp[count] = rx_antennas.output_str[antenna_index][i];
				count++;
			}
			i++;
		}
			
		current_activated_rx_ant_outputs_length = addr_count;
	}
}

/*! \brief Send the output string for the rx antenna to the bus 
 *  \param index The index of the antenna you wish to send the string of */
void antenna_ctrl_send_rx_ant_band_data_to_bus(char index) {
	if ((index > 0) && (index < BAND_20M)){
		unsigned char band_index = index-1;
		unsigned char length = rx_antennas.band_output_length[band_index];
		
		if (length > 0) {
			unsigned char temp[length];
		
			unsigned char start_pos = 0;
			unsigned char count = 0;
			unsigned char addr_count=0;
		
			unsigned char i=0;
		
			antenna_ctrl_deactivate_outputs(current_band_activated_outputs_rx, current_band_activated_outputs_rx_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS);
			
			current_band_activated_outputs_rx_length = 0;
			
			while(i<length) {
				if (rx_antennas.band_output_str[band_index][i] == OUTPUT_ADDR_DELIMITER) {
					//Will add which address the message was sent to
					current_band_activated_outputs_rx[addr_count++] = rx_antennas.band_output_str[band_index][i+1];
					
					if (rx_antennas.band_output_str[band_index][i+1] != 0x00)
						bus_add_tx_message(bus_get_address(), rx_antennas.band_output_str[band_index][i+1], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT, count-start_pos, temp+start_pos);
					else
						internal_comm_add_tx_message(BUS_CMD_DRIVER_ACTIVATE_RX_BAND_OUTPUT, count-start_pos, (char *)(temp+start_pos));
		
					start_pos = count;
					i++;
				}
				else {
					temp[count] = rx_antennas.band_output_str[band_index][i];
					count++;
				}
				i++;
			}
				
			current_band_activated_outputs_rx_length = addr_count;
		}
	}
	else if (current_band_activated_outputs_rx_length > 0) {
		antenna_ctrl_deactivate_outputs(current_band_activated_outputs_rx, current_band_activated_outputs_rx_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS);
		current_band_activated_outputs_rx_length = 0;
	}
}

void antenna_ctrl_rotate(unsigned char ant_index, unsigned int heading) {
	//TODO: CODE TO SEND TO THE ROTATOR BOARD
	unsigned char new_dir[3];
	
	new_dir[0] = antenna_ctrl_get_rotator_sub_addr(ant_index);
	new_dir[1] = (heading >> 8)	& 0x00FF;
	new_dir[2] = heading & 0x00FF;
	
	bus_add_tx_message(bus_get_address(), current_antennas.rotator_addr[ant_index], 
										 (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_ROTATOR_SET_ANGLE, sizeof(new_dir), new_dir);
  
  last_rotated_antenna = ant_index;
}

/*! \brief Set the antenna to be rotated 
 *  \param index The index of the antenna to be rotated */
void antenna_ctrl_set_antenna_to_rotate(unsigned char index) {
  last_rotated_antenna = index;
}

/*! \brief Rotate the last rotatable antenna clockwise */
void antenna_ctrl_rotate_cw(void) {
  if ((last_rotated_antenna != -1) && (last_rotated_antenna < 4) && (current_antennas.rotator_addr[(unsigned char)last_rotated_antenna] != 0)) {
    bus_add_tx_message(bus_get_address(), current_antennas.rotator_addr[(unsigned char)last_rotated_antenna], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_ROTATOR_ROTATE_CW, 1, &current_antennas.rotator_sub_addr[(unsigned char)last_rotated_antenna]);
  }
}

/*! \brief Rotate the last rotatable antenna counter-clockwise */
void antenna_ctrl_rotate_ccw(void) {
  if ((last_rotated_antenna != -1) && (last_rotated_antenna < 4) && (current_antennas.rotator_addr[(unsigned char)last_rotated_antenna] != 0)) {
    bus_add_tx_message(bus_get_address(), current_antennas.rotator_addr[(unsigned char)last_rotated_antenna], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_ROTATOR_ROTATE_CCW, 1, &current_antennas.rotator_sub_addr[(unsigned char)last_rotated_antenna]);
  }
}

/*! \brief Stop rotation of the last antenna */
void antenna_ctrl_rotate_stop(void) {
  if ((last_rotated_antenna != -1) && (last_rotated_antenna < 4) && (current_antennas.rotator_addr[(unsigned char)last_rotated_antenna] != 0)) {
      bus_add_tx_message(bus_get_address(), current_antennas.rotator_addr[(unsigned char)last_rotated_antenna], (1<<BUS_MESSAGE_FLAGS_NEED_ACK), BUS_CMD_ROTATOR_STOP, 1, &current_antennas.rotator_sub_addr[(unsigned char)last_rotated_antenna]);
  }
}

void antenna_ctrl_rotate_set_ant_index(char ant_index) {
  last_rotated_antenna = ant_index;
}

/*! \brief Function used to change an rx antenna 
 * \param ant_index Which RX antenna we wish to chose. If ant_index = 0 the rx antenna outputs are disabled */
void antenna_ctrl_change_rx_ant(unsigned char ant_index) {
	if (ant_index != 0) {
		antenna_ctrl_send_rx_ant_data_to_bus(ant_index-1);
	}
	else if (current_activated_rx_ant_outputs_length > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_rx_ant_outputs, current_activated_rx_ant_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_ANTENNA_OUTPUTS);
		current_activated_rx_ant_outputs_length = 0;
	}
}

/*! \brief Get which antennas can be rotated
 *  \return Which antennas can be rotated, in binary form starting with ant 0 from byte 0 */
unsigned char antenna_ctrl_get_rotatable(void) {
	unsigned char flags = 0;
	
	for (unsigned char i = 0;i<4;i++)
		if (current_antennas.antenna_flag[i] & (1<<ANTENNA_ROTATOR_FLAG))
			flags |= (1<<i);
	
	return(flags);
}

/*! \brief Function which will deactivate all activated antenna ctrl outputs, using type BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS */
void antenna_ctrl_deactivate_all(void) {
	if (current_activated_ant_outputs_length > 0) {
		antenna_ctrl_deactivate_outputs(current_activated_ant_outputs, current_activated_ant_outputs_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_ANT_OUTPUTS);
		current_activated_ant_outputs_length = 0;
	}
}

/*! \brief Function which will deactivate all activated rx antenna ctrl band outputs, using type BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS */
void antenna_ctrl_deactivate_all_rx_band(void) {
	if (current_band_activated_outputs_rx_length > 0) {
		antenna_ctrl_deactivate_outputs(current_band_activated_outputs_rx, current_band_activated_outputs_rx_length, BUS_CMD_DRIVER_DEACTIVATE_ALL_RX_BAND_OUTPUTS);
		current_band_activated_outputs_rx_length = 0;	
	}
}

/*! \brief Set the antenna text
 *  \param str Which data should be saved
 *  \param index The index of the antenna */
void antenna_ctrl_set_antenna_text(char *str, unsigned char index) {
	strcpy(current_antennas.antenna_text[index], str);
}

/*! Get the antenna text
 *  \param index The index of the antenna
 *  \return A pointer to the string */
char* antenna_ctrl_get_antenna_text(unsigned char index) {
	return(current_antennas.antenna_text[index]);
}

/*! \brief Get the antenna text length
 *  \param index The index of the antenna
 *  \return the length of the text */
unsigned char antenna_ctrl_get_antenna_text_length(unsigned char index) {
	return(current_antennas.antenna_text_length[index]);
}


/*! \brief Set the output combination string
 *  \param data The string you wish to save
 *  \param index The index of the output combination
 *  \param length The length of the output string */
void antenna_ctrl_set_output_comb(unsigned char *data, unsigned char index, unsigned char length) {
	memcpy(current_antennas.antenna_comb_output_str[index], data, length);
	
	current_antennas.antenna_output_length[index] = length;
}

/*! \brief Retrieve the output combination string
 *  \param index Which of the bands you wish to get the output string for
 *  \return pointer to the string beginning */
unsigned char* antenna_ctrl_get_output_comb(unsigned char index) {
	return(current_antennas.antenna_comb_output_str[index]);
}

/*! \brief Retrieve the length of the output combination string 
 *  \param index Which of the combinations you wish to retrieve the length of
 *  \return The length of the output string */
unsigned char antenna_ctrl_get_output_comb_length(unsigned char index) {
	return(current_antennas.antenna_output_length[index]);
}

/*! \brief Set the direction of a specific antenna
 *  \param dir The direction of the antenna
 *  \param index The index of the antenna */
void antenna_ctrl_set_direction(unsigned int dir, unsigned char index) {
	current_antennas.antenna_direction[index] = dir;
}

/*! \brief Get the direction of a specific antenna
 *  \param  index The index of the antenna
 *  \return The direction of the antenna */
unsigned int antenna_ctrl_get_direction(unsigned char index) {
	return(current_antennas.antenna_direction[index]);
}

/*! \brief Get the address of the rotator at a certain antenna index
 *  \param  ant_index The index of the antenna
 *  \return The address of the rotator */
unsigned char antenna_ctrl_get_rotator_addr(unsigned char ant_index) {
	if (current_antennas.antenna_flag[ant_index] & (1<<ANTENNA_ROTATOR_FLAG))
		return(current_antennas.rotator_addr[ant_index]);
	else
		return(0);
}

/*! \brief Set the antenna flags
 *  \param flags The flags you wish to be enabled for this antenna
 *  \param index The index of the antenna which the flags should be set to
 */
void antenna_ctrl_set_flags(unsigned char flags, unsigned char index) {
	current_antennas.antenna_flag[index] = flags;
}

/*! \brief Get the antenna flags
 *  \param  index The index of which antenna you wish to get the flag content from
 *  \return The flags
 */
unsigned char antenna_ctrl_get_flags(unsigned char index) {
	return(current_antennas.antenna_flag[index]);
}

/*! \brief Set the value of combination allowed
 *  \param comb The combination that is allowed */
void antenna_ctrl_set_comb_allowed(unsigned int comb) {
	current_antennas.antenna_comb_allowed = comb;
}

/*! \brief Set the flags of the rotator, see antenna_ctrl.h for defines
 *  \param ant_index The antenna index
 *  \param flags Flags from the rotator */
void antenna_ctrl_set_rotator_flags(unsigned char ant_index,unsigned char flags) {
	current_antennas.rotator_flags[ant_index] = flags;
}

/*! \brief Get the flags of the rotator, see antenna_ctrl.h for defines
 *  \return The rotator flags of the antenna */
unsigned char antenna_ctrl_get_rotator_flags(unsigned char ant_index) {
	return(current_antennas.rotator_flags[ant_index]);
}

/*! \brief Get the value of combination allowed
 *  \return The combination allowed value
 */
unsigned int antenna_ctrl_get_comb_allowed(void) {
	return(current_antennas.antenna_comb_allowed);
}

/*! \brief Set the antenna data
 *  \param data The data we wish to use as antenna data */
void antenna_ctrl_set_antenna_data(struct_antenna *data) {
	memcpy(&current_antennas, &data, sizeof(struct_antenna));
}

/*! \brief Set the antenna rx data
 *  \param data The data we wish to use as rx antenna data */
void antenna_ctrl_set_rx_antenna_data(struct_rx_antennas *data) {
	memcpy(&rx_antennas, &data, sizeof(struct_rx_antennas));
}

/*! \brief Retrieve the number of rx antennas
 *  \return The number of rx antenna count */
unsigned char antenna_ctrl_get_rx_antenna_count(void) {
	unsigned char count = 0;
	
	for (unsigned char i=0;i<10;i++)
		if (rx_antennas.name[i][0] != '\0')
			count++;
	
	return(count);
}

/*! \brief Retrieve the rx antenna name
 *  \param ant_index The index of the antenna
 *  \return The name of the RX antenna */
char* antenna_ctrl_get_rx_antenna_name(unsigned char ant_index) {
	return(rx_antennas.name[ant_index]);
}

/*! \brief Retrieve the length of the  rx antenna name
 *  \param ant_index The index of the antenna
 *  \return The length of the name */
unsigned char antenna_ctrl_get_rx_antenna_name_length(unsigned char ant_index) {
  return(rx_antennas.name_length[ant_index]);
}

/*! \brief Retrieve the rx antenna output str
 *  \param ant_index The index of the antenna
 *  \return The output str of the rx antenna sent in */
char* antenna_ctrl_get_rx_antenna_output_str(unsigned char ant_index) {
	return(rx_antennas.output_str[ant_index]);
}

/*! \brief Function which will select the default antenna for this band if it is configured */
void antenna_ctrl_select_default_ant(void) {
  if (current_antennas.default_antenna == 0)
		event_tx_button1_pressed();
	else if (current_antennas.default_antenna == 1)
		event_tx_button2_pressed();
	else if (current_antennas.default_antenna == 2)
		event_tx_button3_pressed();
	else if (current_antennas.default_antenna == 3)
		event_tx_button4_pressed();
}

/*! \brief Read the eeprom for the antenna settings 
 *  \param band_index The band index */
void antenna_ctrl_ant_read_eeprom(unsigned char band_index) {
	eeprom_get_antenna_data(&current_antennas, band_index);
  
  last_rotated_antenna = -1;
        
  unsigned char rotatable = antenna_ctrl_get_rotatable();
  
  for (unsigned char i=0;i<4;i++) {
    if ((rotatable & (1<<i)) != 0) {
      last_rotated_antenna = i;
      break;
    }
  }
}

/*! \brief Read the eeprom for the rx antenna settings  */
void antenna_ctrl_rx_ant_read_eeprom(void) {
	eeprom_get_rx_antenna_data(&rx_antennas);
}

/*! \brief Function returns the start heading for a certain antenna 
 *  \param ant_index The index of the antenna we wish to retrieve the heading from
 *  \return The start heading of this antenna */
unsigned int antenna_ctrl_get_start_heading(unsigned char ant_index) {
	return(current_antennas.rotator_min_heading[ant_index]);
}

/*! \brief Function returns the maximal number of degrees we can rotate an antenna 
 *  \param ant_index The antenna index we wish to retrieve the information from
 *  \return The number of degrees the antenna can be rotated */
unsigned int antenna_ctrl_get_max_rotation(unsigned char ant_index) {
	return(current_antennas.rotator_max_rotation[ant_index]);
}

/*! \brief Get which kind of sub meny type an antenna has got 
 *  \param ant_index Which antenna index we wish to show the sub menu type for 
 *  \return The sub meny type */
unsigned char antenna_ctrl_get_sub_menu_type(unsigned char ant_index) {
	return(current_antennas.sub_menu_type[ant_index]);
}

unsigned char antenna_ctrl_get_rotator_sub_addr(unsigned char ant_index) {
	if (current_antennas.antenna_flag[ant_index] & (1<<ANTENNA_ROTATOR_FLAG))
		return(current_antennas.rotator_sub_addr[ant_index]);
	else
		return(0);
}