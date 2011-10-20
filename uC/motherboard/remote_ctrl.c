/*! \file motherboard/remote_ctrl.c 
 *  \ingroup motherboard_group
 *  \brief Remote control
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-21
 *  \code #include "computer_interface.c" \endcode */
//    Copyright (C) 2011  Mikael Larsmark, SM2WMV
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

//Debug flag for the computer communication protocol
//#define COM_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <string.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "usart.h"
#include "main.h"
#include "remote_ctrl.h"
#include "computer_comm.h"
#include "computer_comm_commands.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

unsigned char remote_ctrl_band_data_updated = 0;
unsigned char remote_ctrl_ant_text_updated = 0;
unsigned char remote_ctrl_rxant_text_updated = 0;
unsigned char remote_ctrl_ant_info_updated = 0;
unsigned char remote_ctrl_ant_dir_updated = 0;

static unsigned char remote_ctrl_active = 0;

struct_band_data band_data;

void remote_ctrl_update_info(void) {
  if (remote_ctrl_band_data_updated) {
    char temp_data[8];
    
    temp_data[0] = band_data.current_band;
    temp_data[1] = band_data.curr_ant_selected;
    temp_data[2] = band_data.curr_rx_ant_selected;
    
    computer_comm_add_tx_message(COMPUTER_COMM_REMOTE_BAND_INFO,3,(char *)temp_data);
    
    remote_ctrl_band_data_updated = 0;
  }
  
  if (remote_ctrl_ant_text_updated) {
  }
}

/*! \brief Activate the remote control mode */
void remote_ctrl_set_active(void) {
  remote_ctrl_active = 1;
  
  internal_comm_add_tx_message(INT_COMM_REMOTE_SET_STATUS,1,&remote_ctrl_active);
}

/*! \brief Deactivate the remote control mode */
void remote_ctrl_set_deactive(void) {
  remote_ctrl_active = 0;
  
  internal_comm_add_tx_message(INT_COMM_REMOTE_SET_STATUS,1,&remote_ctrl_active);
}

/*! \brief Retrieve the status of the remote control mode 
 *  \return 1 if active, 0 if not active */
unsigned char remote_ctrl_get_active_status(void) {
  return(remote_ctrl_active);
}

/*! \brief Retrieve the current band 
 *  \return The current band */
unsigned char remote_ctrl_get_current_band(void) {
  return(band_data.current_band);
}

/*! \brief Retrieve the antennas selected
 *  \return The antennas selected, TX ANT 1-4 = bit 0-3, RX ANT 1-4 = bit 4-7 */
unsigned char remote_ctrl_get_selected_ants(void) {
  return(band_data.curr_ant_selected);
}

/*! \brief Retrieve the rx antenna selected
 *  \return The antenna selected, 0 = none, 1-10 antenna 1 to 10 */
unsigned char remote_ctrl_get_rx_antenna(void) {
  return(band_data.curr_rx_ant_selected);
}

char* remote_ctrl_get_antenna_name(unsigned char index) {
  if (index < 4)
    return(band_data.antenna_name[index]);
  
  return(NULL);
}

char* remote_ctrl_get_rx_antenna_name(unsigned char index) {
  if (index < 10)
    return(band_data.rx_antenna_name[index]);
  
  return(NULL);
}

void remote_ctrl_parse_message(UC_MESSAGE message) {
  switch (message.cmd) {
    case INT_COMM_REMOTE_BAND_INFO:
      // char index 0 -> Current band
      // char index 1 -> Selected ant combination
      // char index 2 -> Selected RX antenna
      // char index 3 -> Current modes
      // char index 4 -> Flags byte 1
      // char index 5 -> Flags byte 2
      // char index 6 -> Erorrs byte 1
      // char index 7 -> Erorrs byte 2
      
      band_data.current_band = message.data[0];
      band_data.curr_ant_selected = message.data[1];
      band_data.curr_rx_ant_selected = message.data[2];
      band_data.current_modes = message.data[3];
      band_data.flags = message.data[4] << 8;
      band_data.flags |= message.data[5];
      band_data.error = message.data[6] << 8;
      band_data.error |= message.data[7];
      
      #ifdef DEBUG_REMOTE_CTRL
        printf("Remote band info\n\r");
        printf("Current band: %i\n\r",band_data.current_band);
        printf("Curr ant selected: 0x%02X\n\r",band_data.curr_ant_selected);
        printf("Curr rx ant selected: 0x%02X\n\r",band_data.curr_rx_ant_selected);
        printf("Curr modes: 0x%02X\n\r",band_data.current_modes);
        printf("Flags: %u\n\r",band_data.flags);
        printf("Errors: %u\n\r",band_data.error);
      #endif

      remote_ctrl_band_data_updated = 1;
      break;
    case INT_COMM_REMOTE_ANT_TEXT:
      // Char index 0  -> Which antenna it is, 0-3
      // Char index 1  -> The length of the text
      // Char index 2- -> The data
      
      if (message.data[0] < 4) {
        for (unsigned char i=0;i<message.data[1];i++)
          band_data.antenna_name[message.data[0]][i] = message.data[2+i];
        
        band_data.antenna_name[message.data[0]][message.data[1]+2] = 0;
        
        #ifdef DEBUG_REMOTE_CTRL
          printf("Antenna %i name: %s\n\r",message.data[0]+1,band_data.antenna_name[message.data[0]]);
        #endif
        
        remote_ctrl_ant_text_updated = 1;
      }
      break;
    case INT_COMM_REMOTE_RXANT_TEXT:
      // Char index 0  -> Which antenna it is, 0-9
      // Char index 1  -> The length of the text
      // Char index 2- -> The data
      
      if (message.data[0] < 10) {      
        for (unsigned char i=0;i<message.data[1];i++)
          band_data.rx_antenna_name[message.data[0]][i] = message.data[2+i];
        
        band_data.rx_antenna_name[message.data[0]][message.data[1]+2] = 0;

        #ifdef DEBUG_REMOTE_CTRL
          printf("RX Antenna %i name: %s\n\r",message.data[0]+1,band_data.rx_antenna_name[message.data[0]]);
        #endif
        
        remote_ctrl_rxant_text_updated = 1;
      }      
      break;
    case INT_COMM_REMOTE_ANT_INFO:
      // Char index 0 -> Antenna 1 flags
      // Char index 1 -> Antenna 2 flags
      // Char index 2 -> Antenna 3 flags
      // Char index 3 -> Antenna 4 flags
      // Char index 4 -> Antenna 1 sub menu type
      // Char index 5 -> Antenna 2 sub menu type
      // Char index 6 -> Antenna 3 sub menu type
      // Char index 7 -> Antenna 4 sub menu type
      
      band_data.antenna_flags[0] = message.data[0];
      band_data.antenna_flags[1] = message.data[1];
      band_data.antenna_flags[2] = message.data[2];
      band_data.antenna_flags[3] = message.data[3];
      band_data.sub_menu_type[0] = message.data[4];
      band_data.sub_menu_type[1] = message.data[5];
      band_data.sub_menu_type[2] = message.data[6];
      band_data.sub_menu_type[3] = message.data[7];
      
      #ifdef DEBUG_REMOTE_CTRL
        for (unsigned char i=0;i<4;i++) {
          printf("Antenna %i flags: 0x%02X\n\r",i+1,band_data.antenna_flags[i]);
          printf("Antenna %i sub type: 0x%02X\n\r",i+1,band_data.sub_menu_type[i]);
        }
      #endif
      
      remote_ctrl_ant_info_updated = 1;
      break;
    case INT_COMM_REMOTE_ANT_DIR_INFO:
      // Char index 0 -> Antenna index (0-3)
      // Char index 1 -> Antenna dir (upper 8 bits)
      // Char index 2 -> Antenna dir (lower 8 bits)
      // Char index 3 -> Antenna rotator flags
      
      if (message.data[0] < 4) {
        band_data.antenna_curr_direction[message.data[0]] = (message.data[1] << 8) + message.data[2];
        band_data.antenna_rotator_flags[message.data[0]] = message.data[3];
        
        #ifdef DEBUG_REMOTE_CTRL
          printf("Antenna %i curr dir: %u\n\r",message.data[0]+1,band_data.antenna_curr_direction[message.data[0]]);
          printf("Antenna %i rotator flags: 0x%02X\n\r",message.data[0]+1,band_data.antenna_rotator_flags[message.data[0]]);
        #endif
      }
      
      remote_ctrl_ant_dir_updated = 1;
      break;
    case INT_COMM_REMOTE_SUBMENU_ARRAY_TEXT:
      break;
    case INT_COMM_REMOTE_SUBMENU_STACK_TEXT:
      break;
    default:
      break;
  }
}