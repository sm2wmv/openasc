/*! \file     rotator/bus_handler.h
 *  \ingroup  rotator_group
 *  \brief    Handle bus communications for this application
 *  \author   Tobias Blomberg, SM0SVX
 *  \date     2012-06-16
 * 
 * Contains functions for handling application specific bus communications.
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

#ifndef _BUS_HANDLER_H_
#define _BUS_HANDLER_H_

#include <avr/pgmspace.h>


/**
 * \brief Initialize the bus handler
 */
void bus_handler_init(void);

/**
 * \brief Call from the periodic tick timer with 1ms interval
 */
void bus_handler_tick(void);

/**
 * \brief Handle the core of the bus communications
 * 
 * This function handle the core of the bus communications. It is normally
 * called from the bus_handler_poll function but under some circumstances,
 * like when in an assert condition, this function can be called directly
 * for just sustaining the base functionality.
 */
void bus_handler_poll_core(void);

/**
 * \brief Handle bus communications
 * 
 * This function should be called from the main loop or similar to "drive"
 * the bus. It will handle received mesages and perform the correct action.
 */
void bus_handler_poll(void);

/**
 * \brief Send an ASCII message on the bus
 * \param to_addr The address to send the message to
 * \param str     The string to send
 *
 * This function will send an ASCII message on the bus. The string to send
 * must be stored in RAM.
 */
void send_ascii_data(unsigned char to_addr, const char *str);

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
void send_ascii_data_P(unsigned char to_addr, PGM_P str);

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
void send_ascii_dataf(unsigned char to_addr, const char *fmt, ...);

#endif
