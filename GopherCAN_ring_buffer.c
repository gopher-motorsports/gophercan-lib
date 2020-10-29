/*
 * GopherCAN.c
 *
 *  Created on: Oct 27, 2020
 *      Author: Calvin
 */


#include "GopherCAN_ring_buffer.h"


// init_buffer
//  initialize the buffer with the values passed in as parameters
void init_buffer(CAN_MSG_RING_BUFFER* buffer, CAN_MSG buffer_memory_ptr[], U8 buffer_size)
{
	buffer->head = 0;
	buffer->fill_level = 0;
	buffer->size = buffer_size;
	buffer->message_buffer = buffer_memory_ptr;
}


// is_full
//  return true if the buffer is full, false otherwise
boolean is_full(CAN_MSG_RING_BUFFER* buffer)
{
	if (buffer->fill_level >= buffer->size)
	{
		return TRUE;
	}

	return FALSE;
}


// is_empty
//  return true if the buffer has no elements, false otherwise
boolean is_empty(CAN_MSG_RING_BUFFER* buffer)
{
	if (buffer->fill_level > 0)
	{
		return FALSE;
	}

	return TRUE;
}


// get_from_buffer
//  gets the message at index as if it was a normal array
CAN_MSG* get_from_buffer(CAN_MSG_RING_BUFFER* buffer, U8 index)
{
	return buffer->message_buffer + ((buffer->head + index) % buffer->size);
}


// remove_from_front
//  will remove the first element of the ring buffer. If the buffer is empty it will do nothing
void remove_from_front(CAN_MSG_RING_BUFFER* buffer)
{
	// don't do anything if the buffer is empty
	if (is_empty(buffer))
	{
		return;
	}

	// move the head to the next element
	buffer->head = (buffer->head + 1) % buffer->size;

	// decrement the fill level
	buffer->fill_level--;
}


// End of GopherCAN_ring_buffer.c
