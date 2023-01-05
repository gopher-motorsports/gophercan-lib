/*
 * GopherCAN_buffers.h
 *
 *  Created on: Jan 4, 2023
 *      Author: jonathan
 */

#ifndef GOPHERCAN_BUFFERS_H_
#define GOPHERCAN_BUFFERS_H_

#include "main.h"
#include "base_types.h"
#include "GopherCAN_config.h"
#include "GopherCAN_network.h"

typedef struct
{
    CAN_TxHeaderTypeDef header;
    U8  data[8];
} CAN_MSG;

typedef struct
{
    U8 BUS_ID;
    CAN_HandleTypeDef* hcan;
    CAN_MSG* message_buffer;    // pointer to the array where the messages are stored
    U8 size;                    // the total amount of space available
    U8 head;                    // the position of the "first" element
    U8 fill_level;              // the number of elements after the head that are apart of the buffer
} CAN_MSG_RING_BUFFER;

#define IS_FULL(buffer) ((buffer)->fill_level >= (buffer)->size)
#define IS_EMPTY(buffer) ((buffer)->fill_level == 0)
#define GET_FROM_BUFFER(buffer, index) ((buffer)->message_buffer + (((buffer)->head + index) % (buffer)->size))

extern CAN_MSG_RING_BUFFER rxbuff;
extern CAN_MSG_RING_BUFFER txbuff0;

#if NUM_OF_BUSSES > 1
extern CAN_MSG_RING_BUFFER txbuff1;
#endif

#if NUM_OF_BUSSES > 2
extern CAN_MSG_RING_BUFFER txbuff2;
#endif

void attach_hcan(U8 bus_id, CAN_HandleTypeDef* hcan);
CAN_MSG_RING_BUFFER* choose_tx_buffer_from_hcan(CAN_HandleTypeDef* hcan);
CAN_HandleTypeDef* choose_hcan_from_tx_buffer(CAN_MSG_RING_BUFFER* buffer);
CAN_MSG_RING_BUFFER* choose_tx_buffer_from_dest_module(MODULE_ID module);
void remove_from_front(CAN_MSG_RING_BUFFER* buffer);
void add_message_by_highest_prio(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message);

#endif /* GOPHERCAN_BUFFERS_H_ */
