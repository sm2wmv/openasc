/*! \file 4cx1500b/ctrl_board/main.h
 *  \brief Main file of the 4cx1500b amp control board
 *  \defgroup ctrl_board_group 4CX1500B amplifier control board
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-02-05
 *  \code #include "4cx1500b/ctrl_board/main.h" \endcode
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

#ifndef _MAIN_H_
#define _MAIN_H_

//! The current firmware revision nr
#define FIRMWARE_REV "0.1b\0"

//! Macro to enable timer 0 interrupt
#define ENABLE_TIMER0_INT() 	TIMSK0 |= (1<<OCIE0A);
//! Macro to disable timer 0 interrupt
#define DISABLE_TIMER0_INT() 	TIMSK0 &= ~(1<<OCIE0A);

//! Flag to indicate that the bus is allowed to transmit
#define BUS_STATUS_ALLOWED_TO_SEND_BIT	0
//! Flag to indicate that a preamble has been found in the bus
#define BUS_STATUS_PREAMBLE_FOUND_BIT		1

//! The size of the RX queue in buffers
#define BUS_RX_QUEUE_SIZE 10
//! The size of the TX queue in buffers
#define BUS_TX_QUEUE_SIZE	25

#define DEF_NR_DEVICES  3

#define BUS_ADDR 0x01

#define DEVICE_ID_AMP_CTRL_BOARD   1
#define DEVICE_ID_AMP_CTRL_BOX     2
#define DEVICE_ID_AMP_POWERMETER   3

#define AD_CONV_INTERVAL	10

//!Run the event first in the event queue
#define FLAG_RUN_EVENT_QUEUE      0
//! Flag which indicates that the device has started properly
#define FLAG_DEVICE_STARTED       1

void event_add_message(void (*func), unsigned int offset, unsigned char id);
unsigned int get_ad_curr_val(unsigned char ch);
void send_ping(void);

#endif
