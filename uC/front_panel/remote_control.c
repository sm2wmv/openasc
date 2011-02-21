/*! \file front_panel/remote_control.c
 *  \brief Remote control of the openASC box
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "front_panel/remote_control.c" \endcode
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
#include <avr/io.h>x
#include <avr/interrupt.h>
#include <string.h>
#include <avr/pgmspace.h>

#include "../global.h"
#include "event_handler.h"
#include "remote_control.h"
#include "antenna_ctrl.h"
#include "band_ctrl.h"

//! Flag that the remote control is active
#define FLAG_REMOTE_CONTROL_MODE_ACTIVE	0

//! Flags used in the remote control
unsigned char remote_control_flags;

char linefeed[3] = {"\r\n\0"};

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
    else if (strcmp_P(argv[0], PSTR("a1")) == 0) {
      remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT1);
    }
    else if (strcmp_P(argv[0], PSTR("a2")) == 0) {
      remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT2);
    }
    else if (strcmp_P(argv[0], PSTR("a3")) == 0) {
      remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT3);
    }
    else if (strcmp_P(argv[0], PSTR("a4")) == 0) {
      remote_control_parse_button(EXT_CTRL_TOGGLE_TX_ANT4);
    }
    else if (strcmp_P(argv[0], PSTR("band")) == 0) {
      if (strcmp_P(argv[1], PSTR("160")) == 0) {
        band_ctrl_change_band(BAND_160M);
      }
      else if (strcmp_P(argv[1], PSTR("80")) == 0) {
        band_ctrl_change_band(BAND_80M);
      }
      else if (strcmp_P(argv[1], PSTR("40")) == 0) {
        band_ctrl_change_band(BAND_40M);
      }
      else if (strcmp_P(argv[1], PSTR("30")) == 0) {
        band_ctrl_change_band(BAND_30M);
      }
      else if (strcmp_P(argv[1], PSTR("20")) == 0) {
        band_ctrl_change_band(BAND_20M);
      }
      else if (strcmp_P(argv[1], PSTR("17")) == 0) {
        band_ctrl_change_band(BAND_17M);
      }
      else if (strcmp_P(argv[1], PSTR("15")) == 0) {
        band_ctrl_change_band(BAND_15M);
      }                        
      else if (strcmp_P(argv[1], PSTR("12")) == 0) {
        band_ctrl_change_band(BAND_12M);
      }      
      else if (strcmp_P(argv[1], PSTR("10")) == 0) {
        band_ctrl_change_band(BAND_10M);
      }
      else {
        send_ascii_data(0, "Huh?\r\n");
      }
    }
    else if (strcmp_P(argv[0], PSTR("info")) == 0) {
      char line[7][25];
      
      unsigned char cnt = 0;
      unsigned char line_nr = 0;
      
      strcpy_P(line[line_nr], PSTR("Band: "));
      if (status.selected_band == BAND_160M) {
        strcpy_P(line[line_nr]+6, PSTR("160m\n\r"));
      }
      else if (status.selected_band == BAND_80M) {
        strcpy_P(line[line_nr]+6, PSTR("80m\n\r"));
      }
      else if (status.selected_band == BAND_40M) {
        strcpy_P(line[line_nr]+6, PSTR("40m\n\r"));
      }
      else if (status.selected_band == BAND_30M) {
        strcpy_P(line[line_nr]+6, PSTR("30m\n\r"));
      }
      else if (status.selected_band == BAND_20M) {
        strcpy_P(line[line_nr]+6, PSTR("20m\n\r"));
      }
      else if (status.selected_band == BAND_17M) {
        strcpy_P(line[line_nr]+6, PSTR("17m\n\r"));
      }
      else if (status.selected_band == BAND_15M) {
        strcpy_P(line[line_nr]+6, PSTR("15m\n\r"));
      }
      else if (status.selected_band == BAND_12M) {
        strcpy_P(line[line_nr]+6, PSTR("12m\n\r"));
      }
      else if (status.selected_band == BAND_10M) {
        strcpy_P(line[line_nr]+6, PSTR("10m\n\r"));
      }

      
      line_nr++;
      
      for (int i=0;i<4;i++) {
        if (antenna_ctrl_get_antenna_text_length(i) > 0) {
          if ((status.selected_ant & (1<<i)) != 0) {
            if (i == 0)
              strcpy_P(line[line_nr], PSTR("  A1: *"));
            else if (i == 1)
              strcpy_P(line[line_nr], PSTR("  A2: *"));
            else if (i == 2)
              strcpy_P(line[line_nr], PSTR("  A3: *"));
            else if (i == 3)
              strcpy_P(line[line_nr], PSTR("  A4: *"));
            
            cnt = 7;
          }
          else {
            if (i == 0)
              strcpy_P(line[line_nr], PSTR("  A1: "));
            else if (i == 1)
              strcpy_P(line[line_nr], PSTR("  A2: "));
            else if (i == 2)
              strcpy_P(line[line_nr], PSTR("  A3: "));
            else if (i == 3)
              strcpy_P(line[line_nr], PSTR("  A4: "));
            
            cnt = 6;
          }
          
          strcpy(line[1+i]+cnt, antenna_ctrl_get_antenna_text(i));
          strcpy(line[1+i]+cnt+antenna_ctrl_get_antenna_text_length(i), linefeed);
          
          line_nr++;
        }
      }

      strcpy_P(line[line_nr],PSTR("RX ant: "));
      if (antenna_ctrl_rx_antenna_selected() == 0) {
        strcpy_P(line[line_nr]+8,PSTR("None\n\r\0")); 
      }
      else {
        strcpy(line[line_nr]+8,antenna_ctrl_get_rx_antenna_name(antenna_ctrl_rx_antenna_selected()-1));
        strcpy(line[line_nr]+8+antenna_ctrl_get_rx_antenna_name_length(antenna_ctrl_rx_antenna_selected()-1),linefeed);
      }
      
      line_nr++;
      
      for (int i=0;i<line_nr;i++)
        send_ascii_data(0, line[i]);
    }
    else {
      send_ascii_data(0, "Huh?\r\n");
    }
  }

  send_ascii_data(0, "%d> ",0);
}