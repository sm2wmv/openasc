/*! \file wmv_bus/bus_usart.h
 *  \ingroup bus_group 
 *  \brief BUS usart routines. 
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2010-01-25
 *  \code #include "wmv_bus/bus_usart.h" \endcode
 */
//    Copyright (C) 2007  Mikael Larsmark, SM3WMV
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

#ifndef _BUS_USART_H_
#define _BUS_USART_H_

unsigned char bus_poll_usart_receive (void);
void bus_usart_init (unsigned int baudrate);
unsigned char bus_usart_transmit (unsigned char data);
unsigned char bus_usart_receive (void);
unsigned char bus_usart_receive_loopback (void);
unsigned char bus_poll_usart_receive (void);
unsigned char bus_usart_sendstring (char *data,unsigned char length);

#endif
