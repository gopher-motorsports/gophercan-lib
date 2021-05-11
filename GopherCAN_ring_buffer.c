/*
 * GopherCAN.c
 *
 *  Created on: Oct 27, 2020
 *      Author: Calvin
 */


#include "GopherCAN_ring_buffer.h"


// static function prototypes
static void copy_message(CAN_MSG* source, CAN_MSG* dest);


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


// add_message_to_back
//  This function will add message to the first open slot in the ring buffer. Note no
//  error checking is done in this function, so it will need to be done somewhere else
//  before calling this function
void add_message_to_back(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message)
{
	CAN_MSG* buffer_message;

	// set the message in the next open element in the buffer to message_to_add (by value, not by reference)
	buffer_message = get_from_buffer(buffer, buffer->fill_level);

	// copy the message to the buffer memory
	copy_message(message, buffer_message);

	// adjust the fill_level to reflect the new message added
	buffer->fill_level++;
}


// add_message_by_highest_prio
//  This function will add message to the buffer based on the ID of the message. Higher
//  priority messages (lower ID) will be towards the front, with lower priority
//  messages (greater ID) will be towards the back. Removing from the front will get
//  the highest priority message. This function assumes the buffer is not full
void add_message_by_highest_prio(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message)
{
	CAN_MSG* buffer_message;
	U8 c;

	// start from the back of the buffer, moving each message towards the back
	// by one and put the new message in the correct spot by ID
	buffer->fill_level++;
	for (c = buffer->fill_level - 1; c >= 0; c--)
	{
		buffer_message = get_from_buffer(buffer, c);
		if (message->id >= buffer_message->id)
		{
			// we have found the correct place for the new message
			buffer_message = get_from_buffer(buffer, c + 1);
			break;
		}

		// move this message back by 1 and try again
		copy_message(buffer_message, get_from_buffer(buffer, c + 1));
	}

	// put the message into the buffer at this position
	copy_message(message, buffer_message);
}


// copy_message
//  function to copy all of the data in source to dest by value, not by refernce
static void copy_message(CAN_MSG* source, CAN_MSG* dest)
{
	U8 c;

	dest->id = source->id;
	dest->dlc = source->dlc;
	dest->rtr_bit = source->rtr_bit;

	for (c = 0; c < dest->dlc; c++)
	{
		dest->data[c] = source->data[c];
	}
}


// End of GopherCAN_ring_buffer.c
