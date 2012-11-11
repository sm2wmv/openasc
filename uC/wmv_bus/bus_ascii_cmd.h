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

#ifndef _BUS_ASCII_CMD_H_
#define _BUS_ASCII_CMD_H_


#include <avr/pgmspace.h>
#include <wmv_bus/bus.h>

#ifndef ASCII_CMD_MAX_LEN
#define ASCII_CMD_MAX_LEN 8
#endif

typedef struct {
  const char    name[ASCII_CMD_MAX_LEN];
  const uint8_t min_args;
  const uint8_t max_args;
  int8_t        (*handler)(uint8_t from_addr, uint8_t argc, char **argv);
} AsciiCommand;


/**
 * \brief Initialize the ASCII parser
 */
void bus_ascii_cmd_init(uint8_t cmd_cnt, AsciiCommand *cmds,
                        const char *cmd_help, const char *prompt);

/**
 * \brief Send an ASCII message on the bus
 * \param to_addr The address to send the message to
 * \param str     The string to send
 *
 * This function will send an ASCII message on the bus. The string to send
 * must be stored in RAM.
 */
void bus_ascii_cmd_send(unsigned char to_addr, const char *str);

/**
 * \brief Send an ASCII message from flash on the bus
 * \param to_addr The address to send the message to
 * \param str     The string, in flash memory, to send
 *
 * This function will send an ASCII message on the bus. The string to send must
 * be stored in flash memory. The easiest way to achieve that is to use the
 * PSTR macro. Example:
 *
 *   send_ascii_data_P(to_addr, PSTR("OK\r\n"));
 */
void bus_ascii_cmd_send_P(unsigned char to_addr, PGM_P str);

/**
 * \brief Send a formatted ASCII message on the bus
 * \param to_addr The address to send the message to
 * \param fmt     A formatted string with arguments, if any. Just like printf.
 *
 * This function will send a formatted ASCII message on the bus. Use it like
 * you use printf to format the string to send. There is a maximum length of
 * 45 characters for the message to send. If it is longer than that, the
 * string will be truncated and "..." will be added to the end of the string.
 */
void bus_ascii_cmd_sendf(unsigned char to_addr, const char *fmt, ...);

/**
 * \brief Parse a received ASCII message
 * \param bus_message The received message
 *
 * This function will parse a received ASCII message and execute the command
 * it contains. The command name should be a single word followed by space
 * separated command arguments. The total command length must not exceed
 * 15 characters since that is the maximum length for a bus message.
 */
void bus_ascii_cmd_parse(BUS_MESSAGE *bus_message);

#endif
