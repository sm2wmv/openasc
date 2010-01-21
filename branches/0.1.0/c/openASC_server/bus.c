/*
 * Linus Widstr�mer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include "mqueue.h"
#include "logger.h"
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// All of this code should be rewritten with a select loop
// in the same manner as socket.c

static int check_queue(void);
static int check_bus(void);

static void transmit_msg_onto_bus(MSG * msg);
static MSG *read_msg_from_bus();

void *busRunner(void *tid)
{
	struct timespec ts;

	while (1) {
		//log_debug("This is busRunner");
		if (check_bus() == 0 && check_queue() == 0) {
			ts.tv_sec = 0;
			ts.tv_nsec = 100000000;
			nanosleep(&ts, NULL);
		}
	}
}

static int check_queue(void)
{
	MSG *msg;
	if ((msg = read_from_queue(BUS_TX_QUEUE)) != NULL) {
		log_debug("Incoming message to be sent to the bus");
		transmit_msg_onto_bus(msg);
		free(msg);
		return 1;
	} else {
		return 0;
	}
}

static int check_bus(void)
{
	MSG *msg;
	if ((msg = read_msg_from_bus()) != NULL) {
		log_debug("Read msg from bus, adding to queue");
		add_to_queue(BUS_RX_QUEUE, msg);
		return 1;
	} else {
		return 0;
	}
}

// TBD
static void transmit_msg_onto_bus(MSG * msg)
{
	log_debug("Message sent to bus/com");
}

// TBD
static MSG *read_msg_from_bus()
{
	return NULL;
}
