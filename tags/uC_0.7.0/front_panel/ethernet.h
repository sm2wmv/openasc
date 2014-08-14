/*! \file front_panel/ethernet.h 
 *  \brief Ethernet controller
 *  \ingroup front_panel_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2012-05-10
 *  \code #include "front_panel/ethernet.h" \endcode
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

#ifndef _ETHERNET_H_
#define _ETHERNET_H_

#define ETHERNET_TX_BUF_LEN 50

void ethernet_init(void);
void ethernet_process(void);
unsigned char ethernet_is_active(void);
unsigned int ethernet_send(unsigned char sock,const unsigned char *buf, unsigned int buflen);
unsigned int ethernet_send_display_data(unsigned char sock, unsigned char *buf, unsigned int buflen);
unsigned char ethernet_get_chip_enabled(void);

void ethernet_send_data(unsigned char sock, unsigned char cmd, unsigned char length, unsigned char *buf);

#endif