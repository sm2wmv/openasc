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
#include "display_handler.h"
#include "antenna_ctrl.h"
#include "band_ctrl.h"
#include "sub_menu.h"
#include "errors.h"
#include "led_control.h"
#include "../remote_commands.h"
#include "ethernet.h"

//! Flag that the remote control is active
#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

static unsigned char ascii_comm_dev_addr = 0;

/*! \brief Parses an ethernet remote command */
void remote_control_parse_command(unsigned char command, unsigned char length, char *data) {
  switch(command) {
    case REMOTE_COMMAND_BUTTON_EVENT: 
      event_process_task(data[0]);
      break;
    case REMOTE_COMMAND_KEYPAD_BUTTON_EVENT:
      event_process_task(ext_key_get_assignment(data[0]));
      break;
    case REMOTE_COMMAND_SET_NEW_BAND:
      if (length > 0)
        main_set_new_band(data[0]);
      break;
    case REMOTE_COMMAND_FORCE_RESET:
      forceHardReset();
      break;
    case REMOTE_COMMAND_TERMINAL_CONNECT:
      ascii_comm_dev_addr = data[0]; 
      break;
    case REMOTE_COMMAND_TERMINAL_DATA:
      if (ascii_comm_dev_addr != 0) {
        bus_add_tx_message(bus_get_address(), ascii_comm_dev_addr,(1<<BUS_MESSAGE_FLAGS_NEED_ACK),BUS_CMD_ASCII_DATA,length,(unsigned char *)data);
      }
      break;
    case REMOTE_COMMAND_ROTATOR_SET_HEADING:
      #ifdef DEBUG_COMPUTER_USART_ENABLED
        printf("NEW HEADING[%i]: %i\r\n",data[0],(data[1]<<8)+data[2]);
      #endif

      if (length > 2)
        antenna_ctrl_rotate(data[0],(data[1]<<8) + data[2]);
      break;
    case REMOTE_COMMAND_ROTATOR_STOP:
      #ifdef DEBUG_COMPUTER_USART_ENABLED
        printf("STOP ROTATOR[%i]\r\n",data[0]);
      #endif
      antenna_ctrl_set_antenna_to_rotate(data[0]);
      antenna_ctrl_rotate_stop();
      break;
    case REMOTE_COMMAND_ROTATOR_TURN_CW:
      #ifdef DEBUG_COMPUTER_USART_ENABLED
        printf("TURN ROTATOR CW[%i]\r\n",data[0]);
      #endif
      antenna_ctrl_set_antenna_to_rotate(data[0]);
      antenna_ctrl_rotate_cw();
      break;
    case REMOTE_COMMAND_ROTATOR_TURN_CCW:
      #ifdef DEBUG_COMPUTER_USART_ENABLED
        printf("TURN ROTATOR CCW[%i]\r\n",data[0]);
      #endif
      antenna_ctrl_set_antenna_to_rotate(data[0]);
      antenna_ctrl_rotate_ccw();
      break;
    case REMOTE_COMMAND_SET_ARRAY_DIR:
      if (sub_menu_get_type(data[0]) == SUBMENU_VERT_ARRAY) {
        sub_menu_set_array_dir(data[1]);
        
        display_handler_repaint();
      }
      break;
    default:
      break;
  }
}

void remote_control_send_status(void) {
  unsigned char var[5] = {0};

  var[0] = led_get_ptt_status();
  var[1] = (led_get_status() >> 8) & 0xFF;
  var[2] = led_get_status() & 0xFF;
  var[3] = status.selected_band;
  var[4] = runtime_settings.band_change_mode;

  led_status_clear();

  ethernet_send_data(0, REMOTE_COMMAND_STATUS, 5, var);
}

void remote_control_send_rx_antennas(void) {
  unsigned char buff[25];
  
  char *rx_ant_name;
  
  for (unsigned char i=0;i<antenna_ctrl_get_rx_antenna_count();i++) {
    rx_ant_name = antenna_ctrl_get_rx_antenna_name(i);
    
    buff[0] = i;  //Antenna index
    
    for (unsigned char cnt=0;cnt<antenna_ctrl_get_rx_antenna_name_length(i);cnt++) {
      buff[1+cnt] = *(rx_ant_name+cnt);
    }
    
    ethernet_send_data(0,REMOTE_COMMAND_RX_ANT_INFO,antenna_ctrl_get_rx_antenna_name_length(i)+1,buff);
  }
}
