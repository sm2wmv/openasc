/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include <stdio.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include "socket.h"
#include "config.h"
#include "logger.h"
#include "bus.h"
#include "bus_msg.h"
#include "mqueue.h"

// Size of the incoming buffer
#define BUFFSIZE 4196

// The socket used for TCP communication
static int sock, csock = 0;

// Setup the TCPlistener socket
static void init_socket_runner(void)
{
	// Setup a socket
	struct sockaddr_in addr;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		log_info("Socket createion failed");
		exit(42);
	}

	/* Construct the server sockaddr_in structure */
	memset(&addr, 0, sizeof(addr));	/* Clear struct */
	addr.sin_family = AF_INET;	/* Internet/IP */
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	/* Incoming addr */
	addr.sin_port = htons(conf->tcp_port);	/* server port */

	/* Bind and listen */
	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		log_info("Socket binding failed");
		exit(42);
	}

	if (listen(sock, 1) < 0) {
		log_info("Socket listen failed");
		exit(42);
	}
}

void *socketRunner(void *tid)
{
	struct sockaddr_in caddr;
	char buffer[BUFFSIZE];
	unsigned int clen;

	// Stuff for the select etc below
	fd_set selset;
	struct timeval tv;
	int retval;

	// Setup the actual socket
	init_socket_runner();

	while (1) {

		// Setup the select interface
		FD_ZERO(&selset);
		FD_SET(sock, &selset);

		// If there is an active client the socket is added to the select
		if (csock > 0)
			FD_SET(csock, &selset);

		tv.tv_sec = 1;
		tv.tv_usec = 0;
		retval = select(FD_SETSIZE, &selset, NULL, NULL, &tv);

		// Check if there was any action on the sockets
		if (retval == -1) {
			log_info("Problem in select loop");
			// TBD: Mark connection as down

		} else if (retval) {
			log_info("There is socket action");

			// Connection attempt
			if (FD_ISSET(sock, &selset)) {

				// Accept the connection
				int tsock = accept(sock, (struct sockaddr *) &caddr, &clen);
				log_info("Client connected: %s\n", inet_ntoa(caddr.sin_addr));

				if(tsock == -1) {
					log_info("Problem with accept");
					exit(42);
				}

				// If the server is idle the connection is accepted
				if(csock==0) {
					csock=tsock;
				} else {
					// If there is an existing connection the new
					// one is ignored. (These two blocks of code could
					// be toggled so that the new connection "kicks the
					// old one out".
					sprintf(buffer, "Server is busy.");
					send(tsock, buffer, strlen(buffer), 0);
					close(tsock);
				}
			}

			// If the active client has sent data it is recieved
			if(csock>0 && FD_ISSET(csock, &selset)) {
				int n;
				n=recv(csock, &buffer, sizeof(buffer), 0);
				log_info("There is client data to be read, n=%d", n);
				if(n==-1 || n==0) {
					log_info("Error in recv, resetting connection");
					close(csock);
					csock=0;
				} else {
					MSG *msg = malloc(sizeof(MSG));
					memcpy(msg, buffer, n);
					add_to_queue(BUS_TX_QUEUE, msg);
				}
			}
		} else {
			log_debug("No data on socket, checking BUS_RX_QUEUE");
			
			MSG *msg;
			while( (msg = read_from_queue(BUS_RX_QUEUE)) != NULL) {
				log_info("Got a message from the RX_QUEUE, %x", msg);
				if(csock>0) {
					if(send(csock, msg, sizeof(MSG), 0) == -1) {
						log_info("Error writing to socket, resetting.");
						close(csock);
						csock=0;
					}
				}
				free(msg);
		        }
		}
	}
}
