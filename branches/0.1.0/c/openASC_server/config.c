/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"

CONF conf;

static void exit_usage(void);
static void set_defaults();

void parseOpts(int argc, char **argv)
{
	if ((conf = malloc(sizeof(*conf))) == NULL)
		exit(42);

	set_defaults();

	while (1) {
		static struct option long_options[] = {
			{"nodaemon", no_argument, 0, 'n'},
			{"debug", no_argument, 0, 'd'},
			{"device", required_argument, 0, 't'},
			{"port", required_argument, 0, 'p'},
			{"sync", no_argument, 0, 's'},
			{"usage", no_argument, 0, 'u'},
			{0, 0, 0, 0}
		};

		int option_index = 0;
		int c = getopt_long(argc, argv, "ndtusp:", long_options,
				    &option_index);

		if (c == -1)
			break;

		switch (c) {
		case 'n':
			conf->daemon = 0;
			break;
		case 'd':
			conf->debug = 1;
			break;
		case 's':
			conf->send_sync = 1;
			break;
		case 't':
			conf->serial_device = strdup(optarg);
			break;
		case 'p':
			conf->tcp_port = atol(optarg);
			break;
		case 'u':
			exit_usage();
			break;
		default:
			exit_usage();
			break;
		}
	}

	if (conf->serial_device == NULL)
		exit_usage();
}

static void set_defaults()
{
	conf->daemon = 1;
	conf->debug = 0;
	conf->tcp_port = 9999;
	conf->send_sync = 0;
}
static void exit_usage(void)
{
	fprintf(stderr,
		"Usage: [--debug] [--nodaemon] [--usage] [--sync] [--port tcpport] --device </path/to/serial/port>\n");
	exit(2);
}
