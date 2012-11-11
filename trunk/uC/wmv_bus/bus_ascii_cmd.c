/*! \file     wmv_bus/bus_ascii_cmd.h
 *  \ingroup  bus_group
 *  \brief    ASCII bus command handling
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-11-11
 *
 * Contains functions for sending and receiving ASCII commands on the bus.
 */

//    Copyright (C) 2012  Mikael Larsmark, SM2WMV
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


/******************************************************************************
 *
 * Standard library includes
 *
 *****************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <avr/io.h>
#include <avr/pgmspace.h> 


/******************************************************************************
 *
 * Project includes
 *
 *****************************************************************************/

#include <wmv_bus/bus_commands.h>


/******************************************************************************
 *
 * Local includes
 *
 *****************************************************************************/

#include "bus_ascii_cmd.h"


/******************************************************************************
 *
 * Macros and type definitions
 *
 *****************************************************************************/


/******************************************************************************
 *
 * Global variable declarations
 *
 *****************************************************************************/

//! The number of defined commands
static uint8_t ascii_cmd_cnt = 0;
//! Command specification for ASCII commands. Points to flash memory.
static AsciiCommand *ascii_cmds = NULL;
//! Help text for ASCII commands. Points to flash memory.
static const char *ascii_cmd_help = NULL;
//! Command prompt. Points to RAM.
static const char *ascii_cmd_prompt = NULL;


/******************************************************************************
 *
 * Public functions
 *
 *****************************************************************************/

void bus_ascii_cmd_init(uint8_t cmd_cnt, AsciiCommand *cmds,
                        const char *cmd_help, const char *prompt) {
  ascii_cmd_cnt = cmd_cnt;
  ascii_cmds = cmds;
  ascii_cmd_help = cmd_help;
  ascii_cmd_prompt = prompt;
}


void bus_ascii_cmd_send(unsigned char to_addr, const char *str) {
  unsigned char flags = 0;
  if (to_addr != BUS_BROADCAST_ADDR) {
    flags = (1 << BUS_MESSAGE_FLAGS_NEED_ACK);
  }

  unsigned char len = strlen(str);
  while (len > 0) {
    unsigned char len_to_send = len < 15 ? len : 15;
    bus_add_tx_message(bus_get_address(),
                       to_addr,
                       flags,
                       BUS_CMD_ASCII_DATA,
                       len_to_send, (unsigned char *)str);
    len -= len_to_send;
    str += len_to_send;
  }
}


void bus_ascii_cmd_send_P(unsigned char to_addr, PGM_P str) {
  unsigned char flags = 0;
  if (to_addr != BUS_BROADCAST_ADDR) {
    flags = (1 << BUS_MESSAGE_FLAGS_NEED_ACK);
  }

  uint8_t buf[BUS_MESSAGE_DATA_SIZE];
  uint8_t len = 0;
  char ch = 255;
  while (ch != 0) {
    ch = pgm_read_byte(str++);
    buf[len++] = ch;
    if ((len == 15) || (ch == 0)) {
      if (ch == 0) {
        --len;
      }
      bus_add_tx_message(bus_get_address(),
                         to_addr,
                         flags,
                         BUS_CMD_ASCII_DATA,
                         len,
                         buf);
      len = 0;
    }
  }
}


void bus_ascii_cmd_sendf(unsigned char to_addr, const char *fmt, ...) {
  char str[BUS_MESSAGE_DATA_SIZE * 3 + 1];

  va_list ap;
  va_start(ap, fmt);
  int len = vsnprintf(str, sizeof(str), fmt, ap);
  va_end(ap);

  if (len >= sizeof(str) - 1) {
    strcpy(str + sizeof(str) - 6, "...\r\n");
  }

  bus_ascii_cmd_send(to_addr, str);
}


void bus_ascii_cmd_parse(BUS_MESSAGE *bus_message) {
  char data[BUS_MESSAGE_DATA_SIZE + 1];
  memcpy(data, bus_message->data, bus_message->length);
  data[bus_message->length] = '\0';

    /* Get the command and its arguments */
  unsigned char argc = 0;
  char *argv[(BUS_MESSAGE_DATA_SIZE + 1) / 2 + 1];
  memset(argv, 0, sizeof(argv));
  for (char *ptr = data; *ptr != '\0'; ++ptr) {
    if (argv[argc] == 0) {
      if (*ptr != ' ') {
        argv[argc] = ptr;
      }
    }
    else {
      if (*ptr == ' ') {
        *ptr = '\0';
        ++argc;
      }
    }
  }
  if (argv[argc] != 0) {
    ++argc;
  }

    /* The argc variable now contain the number of parts in the command.
     * The command name is also included in the count. The argv variable
     * contain the command and the arguments. argv[0] is the command name and
     * argv[1] is the first argument etc. */
  if (argc > 0) {
    uint8_t cmdno;
    for (cmdno = 0; cmdno < ascii_cmd_cnt; ++cmdno) {
      if (strncmp_P(argv[0], ascii_cmds[cmdno].name,
                    sizeof(ascii_cmds[cmdno].name)) == 0) {
        AsciiCommand cmd;
        memcpy_P(&cmd, &ascii_cmds[cmdno], sizeof(AsciiCommand));
        if ((argc < cmd.min_args+1) || (argc > cmd.max_args+1)
            || (cmd.handler(bus_message->from_addr, argc, argv) == -1)) {
          bus_ascii_cmd_send_P(bus_message->from_addr, ascii_cmd_help);
        }
        break;
      }
    }
    if (cmdno == ascii_cmd_cnt) {
      bus_ascii_cmd_send_P(bus_message->from_addr, ascii_cmd_help);
    }
  }
  bus_ascii_cmd_send(bus_message->from_addr, ascii_cmd_prompt);
}

