/*! \file queue.c
 *  \brief An generic FIFO queue
 *  \ingroup queue_group
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2011-12-21
 *  \code #include "queue.c" \endcode
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define QUEUE_DEBUG

#include "queue.h"

void queue_init(queue *q, unsigned int queue_size, unsigned int element_size) {
  q->first = 0;
  q->last = queue_size-1;
  q->queue_size = queue_size;
  q->element_size = element_size;
  q->count = 0;
}

unsigned char queue_enqueue(queue* q, void* data) {
  #ifdef QUEUE_DEBUG
    printf("Queue size: %i\n\r",q->queue_size);
    printf("Count: %i\n\r",q->count);
  #endif
  
  if (q->count >= q->queue_size) {
    #ifdef QUEUE_DEBUG
      printf("Warning: queue overflow enqueue\n\r");
    #endif
    return(1);
  }
  else {
    q->last = (q->last+1) % q->queue_size;
    memcpy(q->content+q->element_size*q->last, data, q->element_size);
    q->count++;
    
    return(0);
  }
}

unsigned char queue_dequeue(queue* q, void* res) {
  if (q->count <= 0) {
    #ifdef QUEUE_DEBUG
      printf("Warning: empty queue dequeue.\n");
    #endif
    return(1);
  }
  else {
    memcpy((char *)res, q->content+q->first*q->element_size, q->element_size);
    q->first = (q->first+1) % q->queue_size;
    q->count--;
  }

  return(0);
}

unsigned char queue_get_first(queue *q, void* res) {
  if (q->count <= 0) {
    #ifdef QUEUE_DEBUG
      printf("Warning: empty queue get first\n\r");
    #endif
    return(1);
  }
  else {
    memcpy((char *)res, q->content+q->first*q->element_size, q->element_size);
    
    return(0);
  }
}

unsigned char queue_get_pos(queue *q, void* res, unsigned int pos) {
  if ((q->count <= 0) || (pos >= q->count)) {
    #ifdef QUEUE_DEBUG
      printf("Warning: empty queue or out of range, queue_get_pos()\n\r");
    #endif
    return(1);
  }
  else {
    memcpy((char *)res, q->content+pos*q->element_size, q->element_size);
    return(0);
  }
}

unsigned char queue_is_empty(queue *q) {
  if (q->count == 0)
    return(1);

  return(0);
}

unsigned int queue_get_count(queue *q) {
  if (q != NULL)
    return(q->count);

  return(0);
}

unsigned char queue_drop(queue *q) {  
  if (q->count > 0) {
    q->first = (q->first+1) % q->queue_size;
    q->count--;
    
    return(1);
  }
  else {
    #ifdef QUEUE_DEBUG
      printf("Warning: The queue was already empty - queue_drop()\n\r");
    #endif
      
    return(0);
  }
}

void queue_drop_all(queue *q) {
  q->first = 0;
  q->last = q->queue_size-1;
  q->count = 0;
}