/*
 * Linus Widströmer, OH0GRY
 * License: GNU GENERAL PUBLIC LICENSE
 */

#include "mqueue.h"
#include "logger.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

// The queues are implemented as linked lists
struct queue_t {
	MSG *msg;
	struct queue_t *next;
};

// Head pointers for the RX queue and the TX queue
static struct queue_t *rxq = NULL, *txq = NULL;

// The mutex for the list modifying code
static pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

/* Push a message onto the specific queue */
void add_to_queue(QUEUE q, MSG * msg)
{
	struct queue_t **qp;
	if (q == BUS_RX_QUEUE)
		qp = &rxq;
	else if (q == BUS_TX_QUEUE)
		qp = &txq;
	else
		return;

	/* Setup a new (empty) storage slot to use */
	struct queue_t *new_msg;

	if ((new_msg = malloc(sizeof(*new_msg))) == NULL) {
		log_info("Great fail in malloc");
		return;
	}

	/* Lock the mutex while iterating the list */
	pthread_mutex_lock(&m);

	/* Place qp at the last next pointer */
	while (*qp != NULL) {
		qp = &((*qp)->next);
	}

	/* Append the message at the end of the queue */
	new_msg->msg = msg;
	new_msg->next = *qp;
	*qp = new_msg;

	/* Unlock the mutex and return */
	pthread_mutex_unlock(&m);
}

/* Try to read from the queue, either returns a
 * message or NULL if the queue was empty */
MSG *read_from_queue(QUEUE q)
{
	struct queue_t **qp;
	if (q == BUS_RX_QUEUE)
		qp = &rxq;
	else if (q == BUS_TX_QUEUE)
		qp = &txq;
	else
		return NULL;

	/* Lock the mutex while iterating the list */
	pthread_mutex_lock(&m);

	/* Setup a pointer to the first message in the queue */
	struct queue_t *first_msg = *qp;

	/* Update the head pointer (unless the queue is already empty */
	if (*qp != NULL) {
		*qp = (*qp)->next;
	}

	/* Unlock the queue */
	pthread_mutex_unlock(&m);

	/* Return the message */
	if (first_msg != NULL)
		return first_msg->msg;
	else
		return NULL;
}

/* Returns the number of messages in the queue */
int queue_msg_count(QUEUE q)
{
	int n = 0;
	struct queue_t **qp;
	if (q == BUS_RX_QUEUE)
		qp = &rxq;
	else if (q == BUS_TX_QUEUE)
		qp = &txq;
	else
		return -1;

	/* Lock the mutex while iterating the list */
	pthread_mutex_lock(&m);
	while (*qp != NULL) {
		n++;
		qp = &((*qp)->next);
	}
	/* Unlock the mutex and return */
	pthread_mutex_unlock(&m);
	return n;
}

void dump_queue(QUEUE q)
{
	struct queue_t **qp;
	if (q == BUS_RX_QUEUE) {
		qp = &rxq;
		fprintf(stderr,"RX: ");
	}
	else if (q == BUS_TX_QUEUE) {
		qp = &txq;
		fprintf(stderr,"TX: ");
	}
	else
		return;

	/* Lock the mutex while iterating the list */
	pthread_mutex_lock(&m);
	while (*qp != NULL) {
		fprintf(stderr,"%p ", (void *)*qp);
		qp = &((*qp)->next);
	}

	fprintf(stderr, "\n");
	/* Unlock the mutex and return */
	pthread_mutex_unlock(&m);
}

