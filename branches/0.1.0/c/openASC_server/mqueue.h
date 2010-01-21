/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#ifndef __MQUEUE_H_
#define __MQUEUE_H_

#include "bus_msg.h"

// The two queues avaible in the mqueue handler
typedef enum { BUS_RX_QUEUE, BUS_TX_QUEUE } QUEUE;

// The exported methods for the queue manager
void add_to_queue(QUEUE q, MSG * msg);
MSG *read_from_queue(QUEUE q);
int queue_msg_count(QUEUE q);
void dump_queue(QUEUE q);

#endif
