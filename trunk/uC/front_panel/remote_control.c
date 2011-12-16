/*! \file front_panel/remote_control.c
 *  \brief Remote control of the openASC box
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-08-15
 *  \code #include "front_panel/remote_control.c" \endcode
 */
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

#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>x
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "../global.h"
#include "event_handler.h"
#include "remote_control.h"
#include "antenna_ctrl.h"
#include "band_ctrl.h"
#include "sub_menu.h"
#include "errors.h"
#include "led_control.h"
#include "../remote_commands.h"

//! Flag that the remote control is active
#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

//! Flags used in the remote control
static unsigned char remote_control_flags = 0;

static unsigned char remote_current_band = 0;

#define REMOTE_UPDATE_BAND_INFO       0
#define REMOTE_UPDATE_TX_ANT_INFO     1

static unsigned char remote_update_vector = 0;
static unsigned int remote_update_rx_ant_text = 0;
static unsigned char remote_update_ant_text = 0;
static unsigned char remote_update_ant_dir_info = 0;
static unsigned char remote_update_sub_menu_info_ant1 = 0;
static unsigned char remote_update_sub_menu_info_ant2 = 0;
static unsigned char remote_update_sub_menu_info_ant3 = 0;
static unsigned char remote_update_sub_menu_info_ant4 = 0;

//static char linefeed[3] = {"\r\n\0"};
//static char huh[7] = {"Huh?\r\n\0"};

/*! \brief Activate the remote control mode */
void remote_control_activate_remote_mode(void) {
  PORTC |= (1<<7);
  
	remote_control_flags |= (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
  
  remote_update_vector |= (1<<REMOTE_UPDATE_BAND_INFO);
  remote_update_vector |= (1<<REMOTE_UPDATE_TX_ANT_INFO);
  
  for (unsigned char i=0;i<antenna_ctrl_get_rx_antenna_count();i++)
    remote_update_rx_ant_text |= (1<<i);
  
  //Bit 0-3
  remote_update_ant_text = 15;
  
  remote_update_ant_dir_info = 15;
 
  if (sub_menu_get_type(0) != SUBMENU_NONE)
    //remote_update_sub_menu_info_ant1 = 255;
  
  if (sub_menu_get_type(1) != SUBMENU_NONE)
    //remote_update_sub_menu_info_ant2 = 255;
  
  if (sub_menu_get_type(2) != SUBMENU_NONE)
    remote_update_sub_menu_info_ant3 = 255;

  if (sub_menu_get_type(3) != SUBMENU_NONE)
    remote_update_sub_menu_info_ant4 = 255;  
}

void remote_control_changed_band(void) {
  if (status.selected_band != BAND_UNDEFINED) {
    remote_update_vector |= (1<<REMOTE_UPDATE_BAND_INFO);
    remote_update_vector |= (1<<REMOTE_UPDATE_TX_ANT_INFO);
    
    for (unsigned char i=0;i<antenna_ctrl_get_rx_antenna_count();i++)
      remote_update_rx_ant_text |= (1<<i);
    
    //Bit 0-3
    remote_update_ant_text = 15;
    
    remote_update_ant_dir_info = 15;
    
    if (sub_menu_get_type(0) != SUBMENU_NONE) {
      remote_update_sub_menu_info_ant1 = 255;
    }
    
    if (sub_menu_get_type(1) != SUBMENU_NONE) {
      remote_update_sub_menu_info_ant2 = 255;
    }
    
    if (sub_menu_get_type(2) != SUBMENU_NONE) {
      remote_update_sub_menu_info_ant3 = 255;
    }

    if (sub_menu_get_type(3) != SUBMENU_NONE) {
      remote_update_sub_menu_info_ant4 = 255;
    }
  }
  else {
    //TODO: FIX THIS PART
  }
}

void remote_control_process(void) {
  if (remote_control_get_remote_mode()) {
    if (remote_update_vector != 0) {
      if (remote_update_vector & (1<<REMOTE_UPDATE_BAND_INFO)) {
        if (!internal_comm_is_tx_queue_full()) {
          remote_control_send_band_info(status.selected_band);
          remote_update_vector &= ~(1<<REMOTE_UPDATE_BAND_INFO);
        }
      }
      
      if (remote_update_vector & (1<<REMOTE_UPDATE_TX_ANT_INFO)) {
        if (!internal_comm_is_tx_queue_full()) {
          remote_control_send_ant_info();
          remote_update_vector &= ~(1<<REMOTE_UPDATE_TX_ANT_INFO);
        }
      }
    }
    
    if (remote_update_rx_ant_text != 0) {
      for (unsigned char i=0;i<10;i++) {
        if (!internal_comm_is_tx_queue_full()) {
          if (remote_update_rx_ant_text & (1<<i)) {
            remote_control_send_rx_ant_info(i);
            
            remote_update_rx_ant_text &= ~(1<<i);
          }
        }
        else
          break;
      }
    }
    
    if (remote_update_ant_text != 0) {
      for (unsigned char i=0;i<4;i++) {
        if (!internal_comm_is_tx_queue_full()) {
          if (remote_update_ant_text & (1<<i)) {
            remote_control_send_ant_text(i);
            
            remote_update_ant_text &= ~(1<<i);
          }
        }
        else
          break;        
      }
    }
    
    if (remote_update_ant_dir_info != 0) {
      for (unsigned char i=0;i<4;i++) {
        if (!internal_comm_is_tx_queue_full()) {
          if (remote_update_ant_dir_info & (1<<i)) {
            remote_control_send_antenna_dir_info(i);
            
            remote_update_ant_dir_info &= ~(1<<i);
          }
        }
        else
          break;
      }
    }
    
/*    if (remote_update_sub_menu_info_ant1 != 0) {
      for (unsigned char i=0;i<8;i++) {
        if (sub_menu_get_combination_count(0) < i) {
          if (!internal_comm_is_tx_queue_full()) {
            if (remote_update_sub_menu_info_ant1 & (1<<i)) {
                remote_control_send_sub_menu(0,i);
              
              remote_update_sub_menu_info_ant1 &= ~(1<<i);
            }
          }
          else
            break;
        }
        else
          remote_update_sub_menu_info_ant1 &= ~(1<<i);
      }
    }
    
    if (remote_update_sub_menu_info_ant2 != 0) {
      for (unsigned char i=0;i<8;i++) {
        if (sub_menu_get_combination_count(0) < i) {
          if (!internal_comm_is_tx_queue_full()) {
            if (remote_update_sub_menu_info_ant2 & (1<<i)) {
              remote_control_send_sub_menu(1,i);
              
              remote_update_sub_menu_info_ant2 &= ~(1<<i);
            }
          }
          else
            break;
        }
        else
          remote_update_sub_menu_info_ant2 &= ~(1<<i);
      }
    }
    
    if (remote_update_sub_menu_info_ant3 != 0) {
      for (unsigned char i=0;i<8;i++) {
        if (sub_menu_get_combination_count(0) < i) {
          if (!internal_comm_is_tx_queue_full()) {
            if (remote_update_sub_menu_info_ant3 & (1<<i)) {
              remote_control_send_sub_menu(2,i);
              
              remote_update_sub_menu_info_ant3 &= ~(1<<i);
            }
          }
          else
            break;
        }
        else
          remote_update_sub_menu_info_ant3 &= ~(1<<i);
      }
    }
    
    if (remote_update_sub_menu_info_ant4 != 0) {
      for (unsigned char i=0;i<8;i++) {
        if (sub_menu_get_combination_count(0) < i) {
          if (!internal_comm_is_tx_queue_full()) {
            if (remote_update_sub_menu_info_ant4 & (1<<i)) {
              remote_control_send_sub_menu(3,i);
              
              remote_update_sub_menu_info_ant4 &= ~(1<<i);
            }
          }
          else
            break;
        }
        else
          remote_update_sub_menu_info_ant4 &= ~(1<<i);
      }
    }*/
  }
}

void remote_control_set_update_band_info(void) {
  remote_update_vector |= (1<<REMOTE_UPDATE_BAND_INFO);
}

void remote_control_set_update_tx_ant_info(void) {
  remote_update_vector |= (1<<REMOTE_UPDATE_TX_ANT_INFO);
}

/*! \brief Deactivate the remote control mode */
void remote_control_deactivate_remote_mode(void) {
  //AUX LED
  PORTC &= ~(1<<7);
	remote_control_flags &= ~(1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Get the current remote control mode 
 *  \return 1 if remote mode is active, 0 if it is not active */
unsigned char remote_control_get_remote_mode(void) {
	if (remote_control_flags & (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE))
    return(1);

  return(0);
}

void remote_control_send_rx_ant_info(unsigned char ant_index) {
  if (remote_control_get_remote_mode()) {
    unsigned char temp_str[RX_ANTENNA_NAME_LENGTH+2];
    //Send the RX antenna names to the motherboard and the current band information
    temp_str[0] = ant_index;
    temp_str[1] = antenna_ctrl_get_rx_antenna_name_length(ant_index);
      
    strcpy((char *)(temp_str+2),antenna_ctrl_get_rx_antenna_name(ant_index));
      
    internal_comm_add_tx_message(INT_COMM_REMOTE_RXANT_TEXT,sizeof(temp_str),temp_str);
  }
}

void remote_control_send_band_info(unsigned char band) {
  if (remote_control_get_remote_mode()) {
    remote_current_band = band;
    
    unsigned char temp_data[11];
    temp_data[0] = band;
    temp_data[1] = status.selected_ant;
    temp_data[2] = status.selected_rx_antenna;
    temp_data[3] = antenna_ctrl_get_rx_antenna_count();
    temp_data[4] = status.function_status;
    temp_data[5] = sub_menu_get_current_pos(0);
    temp_data[6] = sub_menu_get_current_pos(1);
    temp_data[7] = sub_menu_get_current_pos(2);
    temp_data[8] = sub_menu_get_current_pos(3);
    
    //Get the errors from the error handler
    unsigned int errors = error_handler_get_errors();
    
    temp_data[9] = (errors >> 8);
    temp_data[10] = (errors & 0x00FF);

    internal_comm_add_tx_message(INT_COMM_REMOTE_BAND_INFO,sizeof(temp_data),temp_data);
    
    //TODO: Continue to send more info to the motherboard of the new selected band
  }
}

void remote_control_send_ant_text(unsigned char ant_index) {
  if (remote_current_band != BAND_UNDEFINED) {
    unsigned char temp_data[ANTENNA_TEXT_SIZE+2]; 
    //byte 0   -> Antenna index
    //byte 1   -> Length
    //byte 2-x -> data
    
    if (antenna_ctrl_get_antenna_text_length(ant_index) > 0) {
      temp_data[0] = ant_index;
      temp_data[1] = antenna_ctrl_get_antenna_text_length(ant_index);
      
      memcpy(temp_data+2,antenna_ctrl_get_antenna_text(ant_index),antenna_ctrl_get_antenna_text_length(ant_index));
      
      internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_ANT_TEXT,sizeof(temp_data),temp_data);
    }
  }
}

void remote_control_send_antenna_dir_info(unsigned char index) {
  if (remote_control_get_remote_mode()) {
    if (remote_current_band != BAND_UNDEFINED) {
      unsigned char temp_data[4] = {0,0,0,0,0};
      
      if (index < 4) {
        if (antenna_ctrl_get_flags(index) & (1<<ANTENNA_ROTATOR_FLAG)) {
          // Char index 0 -> Antenna index (0-3)
          // Char index 1 -> Antenna dir (upper 8 bits)
          // Char index 2 -> Antenna dir (lower 8 bits)
          // Char index 3 -> Antenna rotator flags
          unsigned int ant_dir = antenna_ctrl_get_direction(index);
        
          temp_data[0] = index;
          temp_data[1] = ant_dir >> 8;
          temp_data[2] = ant_dir & 0x00FF;
          temp_data[3] = antenna_ctrl_get_rotator_flags(index);
        }

        internal_comm_add_tx_message(INT_COMM_REMOTE_ANT_DIR_INFO,sizeof(temp_data),(char *)temp_data);
      }
    }
  }
}

void remote_control_send_ant_info(void) {
 if (remote_control_get_remote_mode()) {
    if (remote_current_band != BAND_UNDEFINED) {
      unsigned char temp_data[8];
      
      temp_data[0] = antenna_ctrl_get_flags(0);
      temp_data[1] = antenna_ctrl_get_flags(1);
      temp_data[2] = antenna_ctrl_get_flags(2);
      temp_data[3] = antenna_ctrl_get_flags(3);
      temp_data[4] = antenna_ctrl_get_sub_menu_type(0);
      temp_data[5] = antenna_ctrl_get_sub_menu_type(1);
      temp_data[6] = antenna_ctrl_get_sub_menu_type(2);
      temp_data[7] = antenna_ctrl_get_sub_menu_type(3);
      
      internal_comm_add_tx_message(INT_COMM_REMOTE_ANT_INFO,sizeof(temp_data),(char *)temp_data);
    }
  }
}

void remote_control_send_sub_menu(unsigned char ant_index, unsigned char sub_pos) {
   if (remote_control_get_remote_mode()) {
    if (remote_current_band != BAND_UNDEFINED) {
      if (sub_menu_get_type(ant_index) == SUBMENU_VERT_ARRAY) {
        if (sub_pos < 8) {
          unsigned char temp_data[SUB_MENU_ARRAY_NAME_SIZE+3];
          
          temp_data[0] = ant_index;
          temp_data[1] = sub_menu_get_combination_count(ant_index);
          temp_data[2] = sub_pos;
          
          strcpy((char *)(temp_data+3), (char *)sub_menu_get_text(ant_index,sub_pos));
          
          internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_SUBMENU_ARRAY_TEXT,sizeof(temp_data),(char *)temp_data);
        }
      }
      else if (sub_menu_get_type(ant_index) == SUBMENU_STACK) {
        if (sub_pos < 6) {
          unsigned char temp_data[SUB_MENU_STACK_NAME_SIZE+3];
          
          temp_data[0] = ant_index;
          temp_data[1] = sub_menu_get_combination_count(ant_index);
          temp_data[2] = sub_pos;
          
          strcpy((char *)(temp_data+3),(char *) sub_menu_get_text(ant_index,sub_pos));
          
          internal_comm_add_tx_message(COMPUTER_COMM_REMOTE_SUBMENU_STACK_TEXT,sizeof(temp_data),(char *)temp_data);      
        }
      }
    }
  }
}