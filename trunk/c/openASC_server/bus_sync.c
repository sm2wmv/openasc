#include "config.h"
#include "mqueue.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define SYNC_PERIOD_S 1
#define SYNC_PERIOD_NS 0

void *syncRunner(void *tid)
{
	struct timespec ts;
	MSG *sync_msg_bus;
	MSG *sync_msg_tcp;

	while (1) {
		if (conf->send_sync) {
			log_debug
			    ("[syncRunner] Ingesting a SYNC into the BUS_TX_QUEUE");

			if ((sync_msg_bus = malloc(sizeof(MSG))) == NULL) {
				log_info("Malloc failure");
				exit(42);
			}

			if ((sync_msg_tcp = malloc(sizeof(MSG))) == NULL) {
				log_info("Malloc failure");
				exit(42);
			}

			sync_msg_bus->from_addr = 0x01;
			sync_msg_bus->to_addr = 0x00;
			sync_msg_bus->checksum = 0x03;
			sync_msg_bus->flags = 0x01;
			sync_msg_bus->cmd = 0x01;
			sync_msg_bus->length = 0x01;
			memset(&(sync_msg_bus->data), 0, 1);

			memcpy(sync_msg_tcp, sync_msg_bus, sizeof(MSG));
			add_to_queue(BUS_TX_QUEUE, sync_msg_bus);
			add_to_queue(BUS_RX_QUEUE, sync_msg_tcp);
		}
		ts.tv_sec = SYNC_PERIOD_S;
		ts.tv_nsec = SYNC_PERIOD_NS;
		nanosleep(&ts, NULL);
	}

}
