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

char linefeed[3] = {"\r\n\0"};
char huh[7] = {"Huh?\r\n\0"};

static char ack[] = {"<ack>\r"};

/*! \brief Activate the remote control mode */
void remote_control_activate_remote_mode(void) {
	remote_control_flags |= (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Deactivate the remote control mode */
void remote_control_deactivate_remote_mode(void) {
	remote_control_flags &= ~(1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Get the current remote control mode 
 *  \return 1 if remote mode is active, 0 if it is not active */
unsigned char remote_control_get_remote_mode(void) {
	return((remote_control_flags & (1<<FLAG_REMOTE_CONTROL_MODE_ACTIVE)) >> FLAG_REMOTE_CONTROL_MODE_ACTIVE);
}

/*! \brief Parse a button press event, will perform an action depending on which button we wish to press
 *  \param button The button we wish to press */
void remote_control_parse_button(unsigned char button) {
	event_process_task(button);
}

/*! \brief Parse a remote control command and perform the proper action
 *  \param data The string we wish to parse */
void remote_control_parse_str(char* data) {
  
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
        
        remote_control_send_ack();
      }
      else {
        send_ascii_data(0, huh);
      }
    }
    else if (strcmp_P(argv[0],PSTR("rxant")) == 0) {
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
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("array")) == 0) {
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
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("stack")) == 0) {
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
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("band")) == 0) {
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
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("errors")) == 0) {
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
        
        remote_control_send_ack();
      }
    }
    else if (strcmp_P(argv[0], PSTR("info")) == 0) {
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
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("getants")) == 0) {
      char line[4][ANTENNA_TEXT_SIZE+6+2];
      char temp[ANTENNA_TEXT_SIZE+1];
      
      strncpy(temp, antenna_ctrl_get_antenna_text(0), antenna_ctrl_get_antenna_text_length(0));
      temp[antenna_ctrl_get_antenna_text_length(0)] = 0;
      remote_control_create_attr_str(line[0], PSTR("ant1"), temp);
      

      sprintf(temp, antenna_ctrl_get_antenna_text(1), antenna_ctrl_get_antenna_text_length(1));
      temp[antenna_ctrl_get_antenna_text_length(1)] = 0;
      remote_control_create_attr_str(line[1], PSTR("ant2"), temp);
      
      
      strncpy(temp, antenna_ctrl_get_antenna_text(2), antenna_ctrl_get_antenna_text_length(2));
      temp[antenna_ctrl_get_antenna_text_length(2)] = 0;
      remote_control_create_attr_str(line[2], PSTR("ant3"), temp);
      
      
      strncpy(temp, antenna_ctrl_get_antenna_text(3), antenna_ctrl_get_antenna_text_length(3));
      temp[antenna_ctrl_get_antenna_text_length(3)] = 0;
      remote_control_create_attr_str(line[3], PSTR("ant4"), temp);
      
      
      for (int i=0;i<4;i++)
        send_ascii_data(0, line[i]);
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("getdirs")) == 0) {
      char line[4][20];
      char temp[4];
      
      sprintf((char *)temp,"%i",antenna_ctrl_get_direction(0));
      remote_control_create_attr_str(line[0], PSTR("dir1"), temp);

      sprintf((char *)temp,"%i",antenna_ctrl_get_direction(1));
      remote_control_create_attr_str(line[1], PSTR("dir2"), temp);

      sprintf((char *)temp,"%i",antenna_ctrl_get_direction(2));
      remote_control_create_attr_str(line[2], PSTR("dir3"), temp);
      
      sprintf((char *)temp,"%i",antenna_ctrl_get_direction(3));
      remote_control_create_attr_str(line[3], PSTR("dir4"), temp);  
      
      for (int i=0;i<4;i++)
        send_ascii_data(0, line[i]);
      
      remote_control_send_ack();
    }
    else if (strcmp_P(argv[0], PSTR("getrxant")) == 0) {
      char line[RX_ANTENNA_NAME_LENGTH+8+9+1];
      char temp[RX_ANTENNA_NAME_LENGTH+1];
      unsigned char ant_index = 0;
      
      if (argc == 1) {
        if (antenna_ctrl_rx_antenna_selected() != 0) {
          strncpy(temp, antenna_ctrl_get_rx_antenna_name(antenna_ctrl_rx_antenna_selected()-1), antenna_ctrl_get_rx_antenna_name_length(antenna_ctrl_rx_antenna_selected()-1));
          temp[antenna_ctrl_get_rx_antenna_name_length(antenna_ctrl_rx_antenna_selected()-1)] = 0;
          remote_control_create_attr_str((char *)line, PSTR("rxant"), temp);
        }
        else
          remote_control_create_attr_str((char *)line, PSTR("rxant"), "None");
         
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"1") == 0)  {
        ant_index = 0;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant1"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"2") == 0)  {
        ant_index = 1;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant2"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"3") == 0)  {
        ant_index = 2;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant3"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"4") == 0)  {
        ant_index = 3;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant4"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"5") == 0)  {
        ant_index = 4;
       
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant5"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"6") == 0)  {
        ant_index = 5;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant6"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"7") == 0)  {
        ant_index = 6;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant7"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"8") == 0)  {
        ant_index = 7;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant8"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"9") == 0)  {
        ant_index = 8;
        
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant9"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else if (strcmp(argv[1],"10") == 0)  {
        ant_index = 9;
       
        strncpy(temp, antenna_ctrl_get_rx_antenna_name(ant_index), antenna_ctrl_get_rx_antenna_name_length(ant_index));
        temp[antenna_ctrl_get_rx_antenna_name_length(ant_index)] = 0;
        remote_control_create_attr_str((char *)line, PSTR("rxant10"), temp);
        
        send_ascii_data(0, (char *)line);
        remote_control_send_ack();
      }
      else
        send_ascii_data(0, huh);
    }
    else if (strcmp_P(argv[0], PSTR("getband")) == 0) {
      char line[13];
      
      if (status.selected_band == BAND_160M) {
        strcpy_P(line,PSTR("<band>160m\r"));
      }
      else if (status.selected_band == BAND_80M) {
        strcpy_P(line,PSTR("<band>80m\r"));
      }
      else if (status.selected_band == BAND_40M) {
        strcpy_P(line,PSTR("<band>40m\r"));        
      }
      else if (status.selected_band == BAND_30M) {
        strcpy_P(line,PSTR("<band>30m\r"));
      }
      else if (status.selected_band == BAND_20M) {
        strcpy_P(line,PSTR("<band>20m\r"));
      }
      else if (status.selected_band == BAND_17M) {
        strcpy_P(line,PSTR("<band>17m\r")); 
      }
      else if (status.selected_band == BAND_15M) {
        strcpy_P(line,PSTR("<band>15m\r"));
      }
      else if (status.selected_band == BAND_12M) {
        strcpy_P(line,PSTR("<band>12m\r"));
      }
      else if (status.selected_band == BAND_10M) {
        strcpy_P(line,PSTR("<band>10m\r")); 
      }
      else
        strcpy_P(line,PSTR("<band>None\r"));
      
      send_ascii_data(0, line);
      
      remote_control_send_ack();
    }    
    else if (strcmp_P(argv[0], PSTR("setdir")) == 0) {
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
    }
    else {
      send_ascii_data(0, huh);
    }
  }

  send_ascii_data(0, "%d> ",0);
}

unsigned char remote_control_create_attr_str(char *return_str, const PROGMEM char *attr, char *input_str) {
  unsigned char len = strlen_P(attr) + 3 + strlen(input_str);
  
  char c_attr[strlen_P(attr)+2];
  c_attr[0] = '<';
  strcpy_P(c_attr+1,attr);
  c_attr[strlen_P(attr)+1] = '>';
  
  strncpy(return_str,c_attr,strlen_P(attr)+2);
  strncpy(return_str+strlen_P(attr)+2, input_str, strlen(input_str));
  
  return_str[len-1] = 13;
  return_str[len] = 0;
  
  return(strlen(return_str));
}

void __inline__ remote_control_send_ack(void) {
  send_ascii_data(0,ack);
}