/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include <getopt.h>
#include <stdio.h>
#include <pthread.h>
#include "mqueue.h"
#include "config.h"
#include "bus.h"
#include "bus_sync.h"
#include "socket.h"
#include "logger.h"

int main(int argc, char **argv)
{
	pthread_t bus_thread, socket_thread, bus_sync_thread;
	struct timespec ts;

	/* Parse command line arguments into the
	 * global configuration structure */
	parseOpts(argc, argv);

	/* Init the syslog interface */
	init_log("openASC");

	/* Output config settings using the logger */
	log_debug("Daemon=%d", conf->daemon);
	log_debug("Debug=%d", conf->debug);
	log_debug("Port=%s", conf->serial_device);

	if (conf->daemon) {
		log_info("If this was for real to app would daemonize now");
		//daemonize();
	}
	pthread_create(&bus_thread, NULL, busRunner, NULL);
	pthread_create(&bus_sync_thread, NULL, syncRunner, NULL);
	pthread_create(&socket_thread, NULL, socketRunner, NULL);

	while (1) {
		log_info("RX=%d, TX=%d", queue_msg_count(BUS_RX_QUEUE),
			 queue_msg_count(BUS_TX_QUEUE));

		dump_queue(BUS_TX_QUEUE);
		dump_queue(BUS_RX_QUEUE);
		ts.tv_sec = 1;
		ts.tv_nsec = 0;
		nanosleep(&ts, NULL);
	}

	return (0);
}
