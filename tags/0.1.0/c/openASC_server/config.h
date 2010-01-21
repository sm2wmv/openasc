/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#ifndef __CONFIG_H
#define __CONFIG_H

typedef struct {
	char daemon;
	char debug;
	char send_sync;
	char *serial_device;
	unsigned int tcp_port;
} *CONF;

extern CONF conf;

void parseOpts(int argc, char **argv);

#endif
