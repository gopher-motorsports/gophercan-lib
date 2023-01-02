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


// remove_from_front
//  will remove the first element of the ring buffer. If the buffer is empty it will do nothing
void remove_from_front(CAN_MSG_RING_BUFFER* buffer)
{
	// don't do anything if the buffer is empty
	if (IS_EMPTY(buffer))
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
	buffer_message = GET_FROM_BUFFER(buffer, buffer->fill_level);

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
	CAN_MSG* buffer_message = GET_FROM_BUFFER(buffer, 0);
	S16 c;

	// start from the back of the buffer, moving each message towards the back
	// by one and put the new message in the correct spot by ID. If the buffer
	// was empty when the message first went through here, it will put the new
	// message in position 0
	buffer->fill_level++;
	for (c = buffer->fill_level - 2; c >= 0; c--)
	{
        buffer_message = GET_FROM_BUFFER(buffer, c);

        if (
            (message->header.IDE == CAN_ID_STD &&
            buffer_message->header.IDE == CAN_ID_EXT)
            ||
            (message->header.IDE == CAN_ID_EXT &&
            buffer_message->header.IDE == CAN_ID_EXT &&
            message->header.ExtId >= buffer_message->header.ExtId)
            ||
            (message->header.IDE == CAN_ID_STD &&
            buffer_message->header.IDE == CAN_ID_STD &&
            message->header.StdId >= buffer_message->header.StdId)
        ) {
            // new message is lower priority, insert behind this buffer message
            buffer_message = GET_FROM_BUFFER(buffer, c + 1);
            break;
        }

		// move this message back by 1 and try again
		copy_message(buffer_message, GET_FROM_BUFFER(buffer, c + 1));
	}

	// put the message into the buffer at this position
	copy_message(message, buffer_message);
}


// copy_message
//  function to copy all of the data in source to dest by value, not by refernce
static void copy_message(CAN_MSG* source, CAN_MSG* dest)
{
	U8 c;

	dest->header = source->header;

	for (c = 0; c < dest->header.DLC; c++)
	{
		dest->data[c] = source->data[c];
	}
}


// End of GopherCAN_ring_buffer.c
