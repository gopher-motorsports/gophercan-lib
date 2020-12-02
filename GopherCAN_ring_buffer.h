/*
 * GopherCAN_ring_buffer.h
 *
 *  Created on: Oct 27, 2020
 *      Author: Calvin
 */

#ifndef GOPHERCAN_RING_BUFFER_H_
#define GOPHERCAN_RING_BUFFER_H_

#include "GopherCAN_structs.h"

// the ring buffer struct
typedef struct
{
	CAN_MSG* message_buffer;                                           // pointer to the array where the messages are stored
	U8 size;                                                           // the total amount of space available
	U8 head;                                                           // the position of the "first" element
	U8 fill_level;                                                     // the number of elements after the head that are apart of the buffer
} CAN_MSG_RING_BUFFER;


// function prototypes
void init_buffer(CAN_MSG_RING_BUFFER* buffer, CAN_MSG buffer_memory_ptr[], U8 buffer_size);
boolean is_full(CAN_MSG_RING_BUFFER* buffer);
boolean is_empty(CAN_MSG_RING_BUFFER* buffer);
CAN_MSG* get_from_buffer(CAN_MSG_RING_BUFFER* buffer, U8 index);
void remove_from_front(CAN_MSG_RING_BUFFER* buffer);
void add_message_to_back(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message);


#endif

// End of GopherCAN_ring_buffer.h
