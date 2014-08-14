/*! \file motherboard/remote_ctrl.c 
 *  \ingroup motherboard_group
 *  \brief Remote control
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-09-21
 *  \code #include "remote_ctrl.c" \endcode */
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
#include "../remote_commands.h"

#include "../comm_interface.h"

#include "../internal_comm.h"
#include "../internal_comm_commands.h"

static unsigned char remote_ctrl_band_data_updated = 0;
static unsigned char remote_ctrl_ant_text_updated = 0;
static unsigned int remote_ctrl_rxant_text_updated = 0;
static unsigned char remote_ctrl_ant_info_updated = 0;
static unsigned char remote_ctrl_ant_dir_updated = 0;
static unsigned char remote_ctrl_sub_menu_stack_updated = 0;
static unsigned char remote_ctrl_sub_menu_array_updated = 0;

volatile static unsigned char remote_ctrl_active[2] = {0,0};

struct_band_data band_data;

struct_sub_menu_array sub_menu_array[4];
struct_sub_menu_stack sub_menu_stack[4];

void remote_ctrl_update_info(void) {
  if (remote_ctrl_band_data_updated) {
    char temp_data[11];
    
    temp_data[0] = band_data.current_band;
    temp_data[1] = band_data.curr_ant_selected;
    temp_data[2] = band_data.curr_rx_ant_selected;
    temp_data[3] = band_data.rx_antenna_count;
    temp_data[4] = band_data.function_status;
    temp_data[5] = band_data.curr_sub_menu_option_selected[0];
    temp_data[6] = band_data.curr_sub_menu_option_selected[1];
    temp_data[7] = band_data.curr_sub_menu_option_selected[2];
    temp_data[8] = band_data.curr_sub_menu_option_selected[3];

    temp_data[9] = band_data.error >> 8;
    temp_data[10] = band_data.error & 0xFF;
    
    
    comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_BAND_INFO,sizeof(temp_data),(char *)temp_data);
    
    remote_ctrl_band_data_updated = 0;
  }
  
  if (remote_ctrl_ant_info_updated) {
    for (unsigned char i=0;i<4;i++) {
      if (remote_ctrl_ant_info_updated & (1<<i)) {
        unsigned char temp_data[8];

        temp_data[0] = band_data.antenna_flags[0];
        temp_data[1] = band_data.antenna_flags[1];
        temp_data[2] = band_data.antenna_flags[2];
        temp_data[3] = band_data.antenna_flags[3];
        
        temp_data[4] = band_data.sub_menu_type[0];
        temp_data[5] = band_data.sub_menu_type[1];
        temp_data[6] = band_data.sub_menu_type[2];
        temp_data[7] = band_data.sub_menu_type[3];
        
        comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_ANT_INFO,sizeof(temp_data),(char *)temp_data);
      }
      
      remote_ctrl_ant_info_updated &= ~(1<<i);
    }
  }

  if (remote_ctrl_ant_text_updated != 0) {
    for (unsigned char i=0;i<4;i++) {
      if (remote_ctrl_ant_text_updated & (1<<i)) {
        char temp_data[COMM_INTERFACE_DATA_LENGTH];
        
        temp_data[0] = i;

        strcpy((char *)(temp_data+1), band_data.antenna_name[i]);
        
        comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_ANT_TEXT,strlen(band_data.antenna_name[i])+2,(char *)temp_data);
      }
      
      remote_ctrl_ant_text_updated &= ~(1<<i);
    }
  } 
  
  if (remote_ctrl_rxant_text_updated != 0) {
    for (unsigned char i=0;i<10;i++) {
      if (remote_ctrl_rxant_text_updated & (1<<i)) {
        
        if (strlen(band_data.rx_antenna_name[i]) > 0) {
          char temp_data[COMM_INTERFACE_DATA_LENGTH];
          temp_data[0] = i;
          strcpy(temp_data+1, band_data.rx_antenna_name[i]);
          
          comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_RXANT_TEXT,strlen(band_data.rx_antenna_name[i])+2,(char *)temp_data);
        }
    
        remote_ctrl_rxant_text_updated &= ~(1<<i);
      }
    }
  }
  
  if (remote_ctrl_ant_dir_updated != 0) {
    char temp_data[4] = {0,0,0,0};
    
    for (unsigned char i=0;i<4;i++) {
      if (remote_ctrl_ant_dir_updated & (1<<i)) {
        temp_data[0] = i; //Which index
        temp_data[1] = band_data.antenna_curr_direction[i][0];
        temp_data[2] = band_data.antenna_curr_direction[i][1];
        temp_data[3] = band_data.antenna_rotator_flags[i];
        
        comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_ANT_DIR_INFO,sizeof(temp_data),(char *)temp_data);
        
        remote_ctrl_ant_dir_updated &= ~(1<<i);
      }
    }
  }
  
  if (remote_ctrl_sub_menu_array_updated != 0) {
    for (unsigned char i=0;i<4;i++) {
      if (remote_ctrl_sub_menu_array_updated & (1<<i)) {
        for (unsigned char c=0;c<8;c++) {
          char temp_data[sizeof(sub_menu_array[i])+2];
          
          temp_data[0] = i;
          temp_data[1] = c;
          temp_data[2] = sub_menu_array[i].direction_count;
          memcpy((char *)temp_data[3],sub_menu_array[i].direction_name[c], sizeof(sub_menu_array[i].direction_name[c]));
          
          comm_interface_add_tx_message(COMPUTER_COMM_REMOTE_SUBMENU_ARRAY_TEXT,sizeof(sub_menu_array[i].direction_name[c])+4,(char *)temp_data);
        }
      
        remote_ctrl_sub_menu_array_updated &= (1<<i);
      }
    }
  }
}

/*! \brief Activate the remote control mode */
void remote_ctrl_set_active(void) {
  remote_ctrl_active[0] = 1;
  
  #ifdef DEBUG_REMOTE_CTRL
    printf("Remote ctrl mode activated\n\r");
  #endif
  
  internal_comm_add_tx_message(INT_COMM_REMOTE_SET_STATUS,1,(char *)remote_ctrl_active);
}

/*! \brief Deactivate the remote control mode */
void remote_ctrl_set_deactive(void) {
  remote_ctrl_active[0] = 0;

  #ifdef DEBUG_REMOTE_CTRL
    printf("Remote ctrl mode deactivated\n\r");
  #endif
  
  internal_comm_add_tx_message(INT_COMM_REMOTE_SET_STATUS,1,(char *)remote_ctrl_active);
}

/*! \brief Retrieve the status of the remote control mode 
 *  \return 1 if active, 0 if not active */
unsigned char remote_ctrl_get_active_status(void) {
  return(remote_ctrl_active[0]);
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

void remote_ctrl_parse_message(struct_comm_interface_msg message) {
  switch (message.cmd) {
    case INT_COMM_REMOTE_BAND_INFO:
      // char index 0 -> Current band
      // char index 1 -> Selected ant combination
      // char index 2 -> Selected RX antenna
      // char index 3 -> Current modes (status.function_status)
      // char index 4 -> Flags byte 1
      // char index 5 -> Flags byte 2
      // char index 6 -> Erorrs byte 1
      // char index 7 -> Erorrs byte 2
      
      band_data.current_band = message.data[0];
      band_data.curr_ant_selected = message.data[1];
      band_data.curr_rx_ant_selected = message.data[2];
      band_data.rx_antenna_count = message.data[3];
      band_data.function_status = message.data[4];
      band_data.curr_sub_menu_option_selected[0] = message.data[5];
      band_data.curr_sub_menu_option_selected[1] = message.data[6];
      band_data.curr_sub_menu_option_selected[2] = message.data[7];
      band_data.curr_sub_menu_option_selected[3] = message.data[8];
      band_data.error = message.data[9] << 8;
      band_data.error |= message.data[10];
      
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
        
        band_data.antenna_name[message.data[0]][message.data[1]] = 0;
        
        #ifdef DEBUG_REMOTE_CTRL
          printf("Antenna %i name: %s\n\r",message.data[0]+1,band_data.antenna_name[message.data[0]]);
        #endif
        
        remote_ctrl_ant_text_updated = (1<<message.data[0]);
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
        
        remote_ctrl_rxant_text_updated |= (1<<message.data[0]);
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
        band_data.antenna_curr_direction[message.data[0]][0] = message.data[1];
        band_data.antenna_curr_direction[message.data[0]][1] = message.data[2];
        band_data.antenna_rotator_flags[message.data[0]] = message.data[3];
        
        #ifdef DEBUG_REMOTE_CTRL
          printf("Antenna %i curr dir: %u\n\r",message.data[0]+1,band_data.antenna_curr_direction[message.data[0]]);
          printf("Antenna %i rotator flags: 0x%02X\n\r",message.data[0]+1,band_data.antenna_rotator_flags[message.data[0]]);
        #endif
      }
      
      remote_ctrl_ant_dir_updated = (1<<message.data[0]);
      break;
    case INT_COMM_REMOTE_SUBMENU_ARRAY_TEXT:
      sub_menu_array[message.data[0]].direction_count = message.data[1];
      memcpy(sub_menu_array[message.data[0]].direction_name[message.data[2]],(char *)(message.data+3),SUB_MENU_ARRAY_NAME_SIZE);
      
      remote_ctrl_sub_menu_array_updated |= (1<<message.data[0]);
      break;
    case INT_COMM_REMOTE_SUBMENU_STACK_TEXT:
      sub_menu_stack[message.data[0]].comb_count = message.data[1];
      memcpy(sub_menu_stack[message.data[0]].comb_name[message.data[2]],(char *)(message.data+3),SUB_MENU_STACK_NAME_SIZE);
      
      remote_ctrl_sub_menu_stack_updated |= (1<<message.data[0]);
      break;
    default:
      break;
  }
}

