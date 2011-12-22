/*! \file queue.h
 *  \brief A generic FIFO queue
 *  \defgroup queue_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-21
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


/*! \brief Initialize the QUEUE
 *  \param queue A pointer to the queue
 *  \param queue_size The number of elements/messages we wish to store to have size for
 *  \param element_size The number of bytes the data type we wish to store requires */
void queue_init(queue *q, unsigned int queue_size, unsigned int element_size);

/*! \brief Enqueue a message into the FIFO
 *  \param queue A pointer to the queue data structure 
 *  \param data that we wish to insert
 *  \return 0 if everything went well, 1 if something failed */ 
unsigned char queue_enqueue(queue* q, void* data);

/*! \brief Dequeue a message from the FIFO queue 
 *  \param queue A pointer to the queue we wish to retrieve an element from
 *  \param res A pointer to where the retrieved data content should be stored
 *  \return 0 if everything went OK, 1 if not */
unsigned char queue_dequeue(queue* q, void* res);

/*! \brief Get an specific message from the normally FIFO queue_dequeue
 *  \param queue A pointer to the queue
 *  \param res A pointer to where the retrieved data content should be stored
 *  \param pos The position we wish to retrieve
 *  \return 0 if everything went OK, 1 if something failed (pos out of range or no messages in the queue) */
unsigned char queue_get_pos(queue *q, void* res, unsigned int pos);

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
  queue queue_##name = {0,0,0,sizeof(type),size,&queue_buffer_##name}; \
  static __inline__ void queue_init_##name() { queue_init(&queue_##name,size,sizeof(type)); };

/*! \brief This macro creates function wrappers for the regular queue functions
 *  \param type The data type, for example char, int etc
 *  \param name The suffix you wish to add to the regular queue names */
#define CREATE_QUEUE_WRAPPERS(type,name) \
  static __inline__ void queue_enqueue_##name(type* e) { queue_enqueue(&queue_##name, e); }; \
  static __inline__ unsigned char queue_dequeue_##name(type* e) { queue_dequeue(&queue_##name,e); }; \
  static __inline__ unsigned char queue_get_pos_##name(type *e,unsigned int pos) { queue_get_pos(&queue_##name, e,pos); }; \
  static __inline__ unsigned char queue_get_first_##name(type *e) { queue_get_first(&queue_##name, e); }; \
  static __inline__ unsigned int  queue_get_count_##name() { queue_get_count(&queue_##name); }; \
  static __inline__ unsigned char queue_is_empty_##name() { queue_is_empty(&queue_##name); }; \
  static __inline__ unsigned char queue_drop_##name() { queue_drop(&queue_##name); }; \
  static __inline__ unsigned char queue_drop_all_##name() { queue_drop_all(&queue_##name); };
  
#endif