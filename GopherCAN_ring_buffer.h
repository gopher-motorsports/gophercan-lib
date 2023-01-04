/*
 * GopherCAN_ring_buffer.h
 *
 *  Created on: Oct 27, 2020
 *      Author: Calvin
 */

#ifndef GOPHERCAN_RING_BUFFER_H_
#define GOPHERCAN_RING_BUFFER_H_

#include "base_types.h"
#include "GopherCAN_config.h"

#if TARGET == F0XX
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#endif

#if TARGET == F4XX
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#endif

#if TARGET == F7XX
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#endif

typedef struct
{
    CAN_TxHeaderTypeDef header;
    U8  data[8];        // not all of these will matter depending on dlc
} CAN_MSG;

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
void remove_from_front(CAN_MSG_RING_BUFFER* buffer);
void add_message_to_back(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message);
void add_message_by_highest_prio(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message);

// some handy macro functions
#define IS_FULL(buffer) ((buffer)->fill_level >= (buffer)->size)
#define IS_EMPTY(buffer) ((buffer)->fill_level == 0)
#define GET_FROM_BUFFER(buffer, index) ((buffer)->message_buffer + (((buffer)->head + index) % (buffer)->size))

#endif

// End of GopherCAN_ring_buffer.h
