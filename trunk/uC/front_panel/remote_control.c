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

//! Flag that the remote control is active
#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

//! Flags used in the remote control
unsigned char remote_control_flags;

unsigned char remote_current_band = 0;

char linefeed[3] = {"\r\n\0"};
char huh[7] = {"Huh?\r\n\0"};

/*! \brief Activate the remote control mode */
void remote_control_activate_remote_mode(void) {
	remote_control_flags |= (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
  
  if (status.selected_band != BAND_UNDEFINED) {
    remote_control_send_band_info(status.selected_band);
    remote_control_send_ant_info();
    
    for (unsigned char i=0;i<4;i++)
      remote_control_send_antenna_dir_info(i);
  }
  
  remote_control_send_rx_ant_info();
}

/*! \brief Deactivate the remote control mode */
void remote_control_deactivate_remote_mode(void) {
	remote_control_flags &= ~(1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Get the current remote control mode 
 *  \return 1 if remote mode is active, 0 if it is not active */
unsigned char remote_control_get_remote_mode(void) {
	if (remote_control_flags & (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE))
    return(1);

  return(0);
}

unsigned char remote_control_send_rx_ant_info(void) {
  unsigned char temp_str[RX_ANTENNA_NAME_LENGTH+2];
  //Send the RX antenna names to the motherboard and the current band information
  for (unsigned char i=0;i<antenna_ctrl_get_rx_antenna_count();i++) {
    temp_str[0] = i;
    temp_str[1] = antenna_ctrl_get_rx_antenna_name_length(i);
    
    strcpy((char *)(temp_str+2),antenna_ctrl_get_rx_antenna_name(i));
    
    internal_comm_add_tx_message(INT_COMM_REMOTE_RXANT_TEXT,sizeof(temp_str),temp_str);
  }
  
  return(antenna_ctrl_get_rx_antenna_count());
}

void remote_control_send_band_info(unsigned char band) {
  remote_current_band = band;
  
  unsigned char temp_data[8];
  temp_data[0] = band;
  temp_data[1] = status.selected_ant;
  temp_data[2] = status.selected_rx_antenna;
  temp_data[3] = 0;
  temp_data[4] = 0;
  temp_data[5] = 0;
  
  //Get the errors from the error handler
  unsigned int errors = error_handler_get_errors();
  
  temp_data[6] = (errors >> 8);
  temp_data[7] = (errors & 0x00FF);

  internal_comm_add_tx_message(INT_COMM_REMOTE_BAND_INFO,sizeof(temp_data),temp_data);
  
  //TODO: Continue to send more info to the motherboard of the new selected band
}

void remote_control_send_antenna_dir_info(unsigned char index) {
  if (remote_current_band != BAND_UNDEFINED) {
    unsigned char temp_data[4] = {0,0,0,0};
    
    if (index < 4) {
      if (antenna_ctrl_get_flags(index) & (1<<ANTENNA_ROTATOR_FLAG)) {
        // Char index 0 -> Antenna index (0-3)
        // Char index 1 -> Antenna dir (upper 8 bits)
        // Char index 2 -> Antenna dir (lower 8 bits)
        // Char index 3 -> Antenna rotator flags
        unsigned int ant_dir = antenna_ctrl_get_direction(index);
      
        temp_data[0] = index;
        temp_data[1] = ant_dir << 8;
        temp_data[2] = ant_dir & 0x00FF;
        temp_data[3] = antenna_ctrl_get_rotator_flags(index);
      }
      else {
        temp_data[0] = 0;
        temp_data[1] = 0;
        temp_data[2] = 0;
        temp_data[3] = 0;
      }
      
      internal_comm_add_tx_message(INT_COMM_REMOTE_ANT_DIR_INFO,sizeof(temp_data),(char *)temp_data);
    }
  }
}

void remote_control_send_ant_info(void) {
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

/*! \brief Parse a button press event, will perform an action depending on which button we wish to press
 *  \param button The button we wish to press */
void remote_control_parse_button(unsigned char button) {
	event_process_task(button);
}

void send_ascii_data(unsigned char to_addr, const char *fmt, ...)
{
  char str[41];
  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  if (len >= sizeof(str)-1) {
    strcpy(str + sizeof(str) - 6, "...\r\n");
    len = sizeof(str)-1;
  }
  
  char *ptr = str;
  while (len > 0) {
    unsigned char len_to_send = len < 15 ? len : 15;
    
    if (to_addr != 0) {
      bus_add_tx_message(bus_get_address(),
                         to_addr,
                         (1<<BUS_MESSAGE_FLAGS_NEED_ACK),
                          BUS_CMD_ASCII_DATA,
                          len_to_send,
                          (unsigned char *)ptr
                        );
    }
    else {
      internal_comm_add_tx_message(INT_COMM_PC_SEND_TO_ADDR, len_to_send, (char *)ptr);
    }
    
    len -= len_to_send;
    ptr += len_to_send;
  }
}

void remote_control_parse_ascii_cmd(UC_MESSAGE *uc_message) {
  printf("MSG CMD: 0x%02X\n\r",uc_message->cmd);
  
  char data[16];
  memcpy(data, uc_message->data, uc_message->length);
  data[uc_message->length] = '\0';
  
    // Get the command and its arguments
  unsigned char argc = 0;
  char *argv[MAX_ASCII_CMD_ARGS];
  argv[0] = NULL;
  unsigned char pos = 0;
  for (pos=0; pos < uc_message->length; ++pos) {
    if (argv[argc] == NULL) {
      if (data[pos] != ' ') {
        argv[argc] = (char*)(data + pos);
      }
    }
    else {
      if (data[pos] == ' ') {
        data[pos] = '\0';
        if (argc >= MAX_ASCII_CMD_ARGS-1) {
          break;
        }
        ++argc;
        argv[argc] = NULL;
      }
    }
  }
  if (argv[argc] != NULL) {
    ++argc;
  }
  
  send_ascii_data(0, "\r\n");

  if (argc > 0) {
    if (strcmp(argv[0], "help") == 0) {
      send_ascii_data(0,"No help\r\n");
    }
    else if (strcmp(argv[0], "ant") == 0) {
      if (argc > 1) {
        if (strcmp(argv[1],"1") == 0)
          remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT1);
        else if (strcmp(argv[1],"2") == 0)
          remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT2);
        else if (strcmp(argv[1],"3") == 0)
          remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT3);
        else if (strcmp(argv[1],"4") == 0)
          remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT4);
      }
      else {
        send_ascii_data(0, huh);
      }
    }
    else if (strcmp(argv[0],"rxant") == 0) {
      if (argc > 1) {
        if (strcmp(argv[1],"1") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT1);
        else if (strcmp(argv[1],"2") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT2);
        else if (strcmp(argv[1],"3") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT3);
        else if (strcmp(argv[1],"4") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT4);
        else if (strcmp(argv[1],"5") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT5);
        else if (strcmp(argv[1],"6") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT6);
        else if (strcmp(argv[1],"7") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT7);
        else if (strcmp(argv[1],"8") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT8);
        else if (strcmp(argv[1],"9") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT9);
        else if (strcmp(argv[1],"10") == 0)
          remote_control_parse_button(EXT_CTRL_SEL_RX_ANT10);
        else
          remote_control_parse_button(EXT_CTRL_SEL_RX_NONE);
      }
      else {
        remote_control_parse_button(EXT_CTRL_TOGGLE_RX_ANT_MODE);
      }
    }
    else if (strcmp(argv[0], "array") == 0) {
      if (argc > 1) {
        if (strcmp(argv[1],"1") == 0)
          remote_control_parse_button(EXT_CTRL_SET_ARRAY_DIR1);
        else if (strcmp(argv[1],"2") == 0)
          remote_control_parse_button(EXT_CTRL_SET_ARRAY_DIR2);
        else if (strcmp(argv[1],"3") == 0)
          remote_control_parse_button(EXT_CTRL_SET_ARRAY_DIR3);
        else if (strcmp(argv[1],"4") == 0)
          remote_control_parse_button(EXT_CTRL_SET_ARRAY_DIR4);
        else
          send_ascii_data(0, huh);
      }
    }
    else if (strcmp(argv[0], "stack") == 0) {
      if (argc > 1) {
        if (strcmp(argv[1],"1") == 0)
          remote_control_parse_button(EXT_CTRL_SET_STACK_COMB1);
        else if (strcmp(argv[1],"2") == 0)
          remote_control_parse_button(EXT_CTRL_SET_STACK_COMB2);
        else if (strcmp(argv[1],"3") == 0)
          remote_control_parse_button(EXT_CTRL_SET_STACK_COMB3);
        else if (strcmp(argv[1],"4") == 0)
          remote_control_parse_button(EXT_CTRL_SET_STACK_COMB4);
        else
          send_ascii_data(0, huh);
      }
    }
    else if (strcmp(argv[0], "band") == 0) {
      if (strcmp_P(argv[1], PSTR("160")) == 0) {
        main_set_new_band(BAND_160M);
      }
      else if (strcmp_P(argv[1], PSTR("80")) == 0) {
        main_set_new_band(BAND_80M);
      }
      else if (strcmp_P(argv[1], PSTR("40")) == 0) {
        main_set_new_band(BAND_40M);
      }
      else if (strcmp_P(argv[1], PSTR("30")) == 0) {
        main_set_new_band(BAND_30M);
      }
      else if (strcmp_P(argv[1], PSTR("20")) == 0) {
        main_set_new_band(BAND_20M);
      }
      else if (strcmp_P(argv[1], PSTR("17")) == 0) {
        main_set_new_band(BAND_17M);
      }
      else if (strcmp_P(argv[1], PSTR("15")) == 0) {
        main_set_new_band(BAND_15M);
      }                        
      else if (strcmp_P(argv[1], PSTR("12")) == 0) {
        main_set_new_band(BAND_12M);
      }      
      else if (strcmp_P(argv[1], PSTR("10")) == 0) {
        main_set_new_band(BAND_10M);
      }
      else {
        send_ascii_data(0, huh);
      }
    }
    else if (strcmp(argv[0], "errors") == 0) {
      if (argc > 1) {
        if (strcmp_P(argv[1], PSTR("clear")) == 0) {
          error_handler_clear_all();
          led_set_error(LED_STATE_OFF);
        }
      }
      else {
        unsigned char error_count = 0;
        char line[NR_OF_ERRORS][20];
        
        //Show the current errors
        if (error_handler_get_state(ERROR_TYPE_BUS_RESEND) != 0) {
          strcpy_P(line[error_count], PSTR("Bus resend\r\n"));
          error_count++;
        }
        else if (error_handler_get_state(ERROR_TYPE_BUS_SYNC) != 0) {
          strcpy_P(line[error_count], PSTR("Sync error\r\n"));
          error_count++;
        }

        if (error_count == 0) {
          strcpy_P(line[error_count], PSTR("No errors\r\n"));
          error_count++;
        }
        
        for (int i=0;i<error_count;i++) {
          send_ascii_data(0, line[i]);
        }
      }
    }
    else if (strcmp(argv[0], "info") == 0) {
      char line[10][30];
      
      for (unsigned char i=0;i<10;i++)
        for (unsigned char j=0;j<25;j++)
          line[i][j] = 0;
      
      unsigned char cnt = 0;
      unsigned char line_nr = 0;
      
      strcpy_P(line[line_nr], PSTR("Band: "));
      
      if (status.selected_band == BAND_160M) {
        strcpy_P(line[line_nr]+6, PSTR("160m\r\n"));
      }
      else if (status.selected_band == BAND_80M) {
        strcpy_P(line[line_nr]+6, PSTR("80m\r\n"));
      }
      else if (status.selected_band == BAND_40M) {
        strcpy_P(line[line_nr]+6, PSTR("40m\r\n"));
      }
      else if (status.selected_band == BAND_30M) {
        strcpy_P(line[line_nr]+6, PSTR("30m\r\n"));
      }
      else if (status.selected_band == BAND_20M) {
        strcpy_P(line[line_nr]+6, PSTR("20m\r\n"));
      }
      else if (status.selected_band == BAND_17M) {
        strcpy_P(line[line_nr]+6, PSTR("17m\r\n"));
      }
      else if (status.selected_band == BAND_15M) {
        strcpy_P(line[line_nr]+6, PSTR("15m\r\n"));
      }
      else if (status.selected_band == BAND_12M) {
        strcpy_P(line[line_nr]+6, PSTR("12m\r\n"));
      }
      else if (status.selected_band == BAND_10M) {
        strcpy_P(line[line_nr]+6, PSTR("10m\r\n"));
      }
      else
        strcpy_P(line[line_nr]+6, PSTR("None\r\n"));

      
      line_nr++;
      
      if (status.selected_band != BAND_UNDEFINED) {
        for (int i=0;i<4;i++) {
          if (antenna_ctrl_get_antenna_text_length(i) > 0) {
            if ((status.selected_ant & (1<<i)) != 0) {
              if (i == 0)
                strcpy_P(line[line_nr], PSTR(" A1: *"));
              else if (i == 1)
                strcpy_P(line[line_nr], PSTR(" A2: *"));
              else if (i == 2)
                strcpy_P(line[line_nr], PSTR(" A3: *"));
              else if (i == 3)
                strcpy_P(line[line_nr], PSTR(" A4: *"));
              
              cnt = 6;
            }
            else {
              if (i == 0)
                strcpy_P(line[line_nr], PSTR(" A1: "));
              else if (i == 1)
                strcpy_P(line[line_nr], PSTR(" A2: "));
              else if (i == 2)
                strcpy_P(line[line_nr], PSTR(" A3: "));
              else if (i == 3)
                strcpy_P(line[line_nr], PSTR(" A4: "));
              
              cnt = 5;
            }
            
            strncpy((char *)(line[line_nr]+cnt),antenna_ctrl_get_antenna_text(i), antenna_ctrl_get_antenna_text_length(i));
            
            if (antenna_ctrl_get_sub_menu_type(i) == SUBMENU_VERT_ARRAY) {
              for (unsigned char c=antenna_ctrl_get_antenna_text_length(i);c<10;c++) {
                line[line_nr][c+cnt] = ' ';
              }
              
              cnt += 10 - antenna_ctrl_get_antenna_text_length(i);
              
              cnt+= sprintf((char *)(line[line_nr]+cnt+antenna_ctrl_get_antenna_text_length(i))," - (%s)",sub_menu_get_text(i,sub_menu_get_current_pos(i)));
            }
            
            if (antenna_ctrl_get_flags(i) & (1<<ANTENNA_ROTATOR_FLAG)) {
              for (unsigned char c=antenna_ctrl_get_antenna_text_length(i);c<10;c++) {
                line[line_nr][c+cnt] = ' ';
              }
              
              if (cnt == 6)
                cnt += 9 - antenna_ctrl_get_antenna_text_length(i);
              else
                cnt += 10 - antenna_ctrl_get_antenna_text_length(i);
              
              cnt+= sprintf((char *)(line[line_nr]+cnt+antenna_ctrl_get_antenna_text_length(i))," - %i deg",antenna_ctrl_get_direction(i));
            }

            strcpy((char *)(line[line_nr]+antenna_ctrl_get_antenna_text_length(i)+cnt),"\r\n");
            
            line_nr++;
          }
        }
  
        strncpy_P((char *)line[line_nr],PSTR("RX ant: \r\n"),10);
        
        if (antenna_ctrl_rx_antenna_selected() == 0) {
          strcpy_P((char *)(line[line_nr]+8),PSTR("None\r\n")); 
        }
        else {
          strncpy((char *)(line[line_nr]+8),antenna_ctrl_get_rx_antenna_name(antenna_ctrl_rx_antenna_selected()-1),antenna_ctrl_get_rx_antenna_name_length(antenna_ctrl_rx_antenna_selected()-1));
          strcpy((char *)(line[line_nr]+8+antenna_ctrl_get_rx_antenna_name_length(antenna_ctrl_rx_antenna_selected()-1)),linefeed);
        }
        
        line_nr++;
        
        if (status.function_status & (1<<FUNC_STATUS_RXANT)) {
          strcpy_P(line[line_nr],PSTR("RX ant: ON\r\n"));
        }
        else
          strcpy_P(line[line_nr],PSTR("RX ant: OFF\r\n"));
        
        line_nr++;
      }
      
      for (int i=0;i<line_nr;i++) {
        send_ascii_data(0, line[i]);        
      }
    }
     else if (strcmp(argv[0], "setdir") == 0) {
      if (argc > 2) {
        if (strcmp(argv[1],"1") == 0)
          antenna_ctrl_rotate(0,atoi(argv[2]));
        else if (strcmp(argv[1],"2") == 0)
          antenna_ctrl_rotate(1,atoi(argv[2]));
        else if (strcmp(argv[1],"3") == 0)
          antenna_ctrl_rotate(2,atoi(argv[2]));
        else if (strcmp(argv[1],"4") == 0)
          antenna_ctrl_rotate(3,atoi(argv[2]));
      }
      else if ((argc == 2) && (strcmp(argv[1],"stop") == 0))
        antenna_ctrl_rotate_stop();
    }
    else {
      send_ascii_data(0, huh);
    }
  }

  send_ascii_data(0, "%d> ",0);
}
