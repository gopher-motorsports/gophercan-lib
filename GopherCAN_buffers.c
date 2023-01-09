/*
 * GopherCAN_buffers.c
 *
 *  Created on: Jan 4, 2023
 *      Author: jonathan
 */

#include "GopherCAN_buffers.h"

static void copy_message(CAN_MSG* source, CAN_MSG* dest);

// RX CAN buffer
CAN_MSG rxbuff_mem[RX_BUFFER_SIZE];
CAN_MSG_RING_BUFFER rxbuff = {
    .BUS_ID = ALL_BUSSES,
    .hcan = NULL,
    .message_buffer = rxbuff_mem,
    .size = RX_BUFFER_SIZE,
    .head = 0,
    .fill_level = 0,
    .mutex = NULL
};

// TX buffers for each CAN bus
CAN_MSG txbuff0_mem[TX_BUFFER_SIZE];
CAN_MSG_RING_BUFFER txbuff0 = {
    .BUS_ID = GCAN0,
    .hcan = NULL,
    .message_buffer = txbuff0_mem,
    .size = TX_BUFFER_SIZE,
    .head = 0,
    .fill_level = 0,
    .mutex = NULL
};

#if NUM_OF_BUSSES > 1
CAN_MSG txbuff1_mem[TX_BUFFER_SIZE];
CAN_MSG_RING_BUFFER txbuff1 = {
    .BUS_ID = GCAN1,
    .hcan = NULL,
    .message_buffer = txbuff1_mem,
    .size = TX_BUFFER_SIZE,
    .head = 0,
    .fill_level = 0,
    .mutex = NULL
};
#endif

#if NUM_OF_BUSSES > 2
CAN_MSG txbuff2_mem[TX_BUFFER_SIZE];
CAN_MSG_RING_BUFFER txbuff2 = {
    .BUS_ID = GCAN2,
    .hcan = NULL,
    .message_buffer = txbuff2_mem,
    .size = TX_BUFFER_SIZE,
    .head = 0,
    .fill_level = 0,
    .mutex = NULL
};
#endif


// get TX buffer associated with a CAN handle
// defaults to TX buffer 0
CAN_MSG_RING_BUFFER* choose_tx_buffer_from_hcan(CAN_HandleTypeDef* hcan) {
#if NUM_OF_BUSSES > 2
    if (txbuff2.hcan == hcan) return &txbuff2;
#endif
#if NUM_OF_BUSSES > 1
    if (txbuff1.hcan == hcan) return &txbuff1;
#endif
    return &txbuff0;
}

// get CAN handle associated with buffer
// defaults to TX buffer 0
CAN_HandleTypeDef* choose_hcan_from_tx_buffer(CAN_MSG_RING_BUFFER* buffer) {
#if NUM_OF_BUSSES > 2
    if (&txbuff2 == buffer) return txbuff2.hcan;
#endif
#if NUM_OF_BUSSES > 1
    if (&txbuff1 == buffer) return txbuff1.hcan;
#endif
    return txbuff0.hcan;
}

// get TX buffer on the CAN bus associated with a module ID
// defaults to TX buffer 0
CAN_MSG_RING_BUFFER* choose_tx_buffer_from_dest_module(MODULE_ID module) {
#if NUM_OF_BUSSES > 2
    if (txbuff2.BUS_ID == module_bus_number[module]) return &txbuff2;
#endif
#if NUM_OF_BUSSES > 1
    if (txbuff1.BUS_ID == module_bus_number[module]) return &txbuff1;
#endif
    return &txbuff0;
}

//  will remove the first element of the ring buffer. If the buffer is empty it will do nothing
void remove_from_front(CAN_MSG_RING_BUFFER* buffer)
{
    if (IS_EMPTY(buffer)) return;

    // move the head to the next element
    buffer->head = (buffer->head + 1) % buffer->size;

    // decrement the fill level
    buffer->fill_level--;
}

// add_message_by_highest_prio
//  This function will add message to the buffer based on the ID of the message. Higher
//  priority messages (lower ID) will be towards the front, with lower priority
//  messages (greater ID) will be towards the back. Removing from the front will get
//  the highest priority message.
void add_message_by_highest_prio(CAN_MSG_RING_BUFFER* buffer, CAN_MSG* message)
{
    // protect buffer from RTOS thread switching
    if (buffer->mutex != NULL) {
        if(osMutexAcquire(buffer->mutex, MUTEX_TIMEOUT)) return;
    }
#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
    // protect buffer from interrupts
    HAL_CAN_DeactivateNotification(choose_hcan_from_tx_buffer(buffer), CAN_IT_TX_MAILBOX_EMPTY);
#endif

    if (IS_FULL(buffer)) return;

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

#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
    HAL_CAN_ActivateNotification(choose_hcan_from_tx_buffer(buffer), CAN_IT_TX_MAILBOX_EMPTY);
#endif
    if (buffer->mutex != NULL) {
        osMutexRelease(buffer->mutex);
    }
}

// copy_message
//  function to copy all of the data in source to dest by value, not by reference
static void copy_message(CAN_MSG* source, CAN_MSG* dest)
{
    U8 c;

    dest->header = source->header;

    for (c = 0; c < dest->header.DLC; c++)
    {
        dest->data[c] = source->data[c];
    }
}
