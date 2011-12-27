/*! \file queue.h
 *  \brief A generic FIFO queue
 *  \defgroup queue_group
 *  \author Mikael Larsmark (SM2WMV), Henrik Mäkitaavola
 *  \date 2011-12-23
 *  \code #include "queue.h" \endcode
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

#ifndef _QUEUE_H_
#define _QUEUE_H_

/*! This queue is a generic FIFO queue. By using the CREATE_QUEUE and CREATE_QUEUE_WRAPPERS you can easily create a queue
 *  with wrapper functions with the name that was sent into the CREATE_QUEUE* macros. All these functions below can be used
 *  directly, but it is easier to use the wrapper functions. If for example assigning them with the name "comm_tx" that 
 *  means that queue_enqueue will be named queue_enqueue_comm_tx. If using the wrapper functions you never need to send 
 *  in the pointer for the queue structure, also the type casting is done automatically */

typedef struct {
  //! The first element in the queue
  unsigned int first;
  //! The last element in the queue
  unsigned int last;
  //! How many messages are in the queue
  unsigned int count;
  //! The element size
  unsigned int element_size;
  //! Number of "elements" in the buffer
  unsigned int queue_size;
  //! The data content in the queue
  char* content;
} queue;

/*! \brief Enqueues a message into the FIFO
 *  \param queue A pointer to the queue data structure 
 *  \param data that we wish to insert
 *  \return 0 if everything went well, 1 if something failed */ 
unsigned char queue_enqueue(queue* q, void* data);

/*! \brief Dequeues a message from the FIFO queue 
 *  \param queue A pointer to the queue we wish to retrieve an element from
 *  \param res A pointer to where the retrieved data content should be stored
 *  \return 0 if everything went OK, 1 if not */
unsigned char queue_dequeue(queue* q, void* res);

/*! \brief Get the first element/message in the queue
 *  \param queue A pointer to the queue
 *  \param res A pointer to where the retrieved data content should be stored
 *  \return 0 if everything went OK, 1 if it failed (empty queue) */
unsigned char queue_get_first(queue *q, void* res);

/*! \brief Get the number of messages in the queue 
 *  \param queue A pointer to the queue
 *  \return The size of the queue in the message/element count */
unsigned int queue_get_count(queue *q);

/*! \brief Check if the queue is empty 
 *  \param queue A pointer to the queue
 *  \return 1 if the Queue is empty, 0 if its not */
unsigned char queue_is_empty(queue *q);

/*! \brief Drop the first message in the queue 
 *  \param queue A pointer to the queue
 *  \return 1 if a message was dropped, 0 otherwise (queue was empty) */
unsigned char queue_drop(queue *q);

/*! \brief Drop all the messages in the queue 
 *  \param queue A pointer to the queue */
void queue_drop_all(queue *q);

/*! \brief This macro creates a queue buffer and also a function for initializing the queue
 *  \param size The size you wish the queue to have (number of elements)
 *  \param type The data type, for example char, int etc
 *  \param name The suffix you wish to add to the regular queue names */
#define CREATE_QUEUE(size,type,name) \
  type queue_buffer_##name[size]; \
  queue queue_##name = {0,size-1,0,sizeof(type),size,(void *)queue_buffer_##name};

/*! \brief This macro creates function wrappers for the regular queue functions
 *  \param type The data type, for example char, int etc
 *  \param name The suffix you wish to add to the regular queue names */
#define CREATE_QUEUE_WRAPPERS(type,name) \
  static __inline__ unsigned char queue_enqueue_##name(type* e) { return(queue_enqueue(&queue_##name,e)); }; \
  static __inline__ unsigned char queue_dequeue_##name(type* e) { return(queue_dequeue(&queue_##name,e)); }; \
  static __inline__ unsigned char queue_get_first_##name(type *e) { return(queue_get_first(&queue_##name, e)); }; \
  static __inline__ unsigned int  queue_get_count_##name(void) { return(queue_get_count(&queue_##name)); }; \
  static __inline__ unsigned char queue_is_empty_##name(void) { return(queue_is_empty(&queue_##name)); }; \
  static __inline__ unsigned char queue_drop_##name(void) { return(queue_drop(&queue_##name)); }; \
  static __inline__ void queue_drop_all_##name(void) { queue_drop_all(&queue_##name); };
  
#endif