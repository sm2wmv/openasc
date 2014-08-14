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

#ifndef _COMPUTER_COMM_TX_QUEUE_H_
#define _COMPUTER_COMM_TX_QUEUE_H_

#include "computer_comm.h"

//! \brief Size of the TX queue
#define COMPUTER_COMM_TX_QUEUE_SIZE	20

//! The structure of the TX circular buffer
typedef struct tx_linked_list {
	//! A UC_MESSAGE
	COMM_MESSAGE message[COMPUTER_COMM_TX_QUEUE_SIZE];
	//! first position in the list
	unsigned char first;
	//! last position in the list
	unsigned char last;
} computer_comm_tx_queue_struct;

void computer_comm_tx_queue_add(COMM_MESSAGE message);
COMM_MESSAGE computer_comm_tx_queue_get_pos(unsigned char pos);
COMM_MESSAGE computer_comm_tx_queue_get(void);
void computer_comm_tx_queue_drop(void);
void computer_comm_tx_queue_dropall(void);
void computer_comm_tx_queue_init(void);
unsigned char computer_comm_tx_queue_is_empty(void);
unsigned char computer_comm_tx_queue_size(void);

#endif
