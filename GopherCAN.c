/*
 * GopherCAN
 */

#include "GopherCAN.h"
#include "GopherCAN_network.h"

/*************************************************
 * STATIC FUNCTION PROTOTYPES
*************************************************/

static S8 init_filters(CAN_HandleTypeDef* hcan);

static void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);
static void service_can_tx_hardware(CAN_HandleTypeDef* hcan);

static S8 encode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length);
static S8 tx_can_message(CAN_MSG* message);

static S8 service_can_rx_message_std(CAN_MSG* message);
static S8 service_can_rx_message_ext(CAN_MSG* message);
static S8 decode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length);

static S8 parameter_requested(CAN_MSG* message, CAN_ID* id);

static S8 run_can_command(CAN_MSG* message, CAN_ID* id);

static S8 send_error_message(CAN_ID* id, GCAN_ERROR_ID error_id);

static CAN_BUS* get_bus_from_hcan(CAN_HandleTypeDef* hcan);
static void remove_from_front(CAN_BUFFER* buffer);
static void add_message_by_highest_prio(CAN_BUS* bus, CAN_MSG* message);
static void copy_message(CAN_MSG* source, CAN_MSG* dest);

static U32 build_message_id(CAN_ID* id);

/*************************************************
 * GLOBAL VARIABLES
*************************************************/

// callbacks triggered by receiving a particular 11-bit CAN ID
void (*CALLBACKS_STD[NUM_OF_GROUPS]) () = { NULL };

// callbacks triggered by receiving a particular command
void (*CALLBACKS_CMD[NUM_OF_COMMANDS]) (MODULE_ID, U8, U8, U8, U8) = { NULL };

/*************************************************
 * INITIALIZATION
*************************************************/

// init_can
//  Connects an hcan instance to a bus ID, configures ID filters, activates ISRs, starts CAN peripheral
// PARAMS:
//  CAN_HandleTypeDef* hcan: the bxCAN hcan pointer from the STM HAL library
//  BUS_ID bus_id: CAN bus identifier
// RETURNS:
//  error codes specified in GopherCAN.h
S8 init_can(CAN_HandleTypeDef* hcan, BUS_ID bus_id)
{
	GCAN_InitNetwork();

	// attach hcan to designated bus
    CAN_BUS* bus = BUSES[bus_id];
    bus->hcan = hcan;

	if (init_filters(hcan))
	{
		return FILTER_SET_FAILED;
	}

	// activate ISRs
	if (
		HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK
		|| HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK
		|| HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK
	) {
		return IRQ_SET_FAILED;
	}

	// start CAN
	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		return CAN_START_FAILED;
	}

	return CAN_SUCCESS;
}


// init_filters
//  Configures CAN message acceptance filters. Messages matching a filter are copied to SRAM.
//  Messages not matching any filter are discarded. Accepted IDs are selected by macros in GopherCAN_config.h
static S8 init_filters(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef filter;
	U8 banknum = 0;

	// starting banknum for CAN2 in dual CAN mode
	if (hcan->Instance == CAN2) banknum = SLAVE_FIRST_FILTER;

	filter.FilterActivation = CAN_FILTER_ENABLE;
    filter.FilterMode = CAN_FILTERMODE_IDMASK; // use mask mode to filter
    filter.SlaveStartFilterBank = SLAVE_FIRST_FILTER;

#ifdef FILTER_ACCEPT_ALL // match/accept all message IDs
	filter.FilterBank = banknum++;                     // modify bank 0 (of 13)
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;    // use FIFO0
	filter.FilterScale = CAN_FILTERSCALE_32BIT;        // 32 bit mask
	filter.FilterIdLow = 0;                            // accepted IDs (lower 16 bits)
	filter.FilterIdHigh = 0;                           // accepted IDs (upper 16 bits)
	filter.FilterMaskIdLow = 0;                        // bits to compare (lower 16 bits)
	filter.FilterMaskIdHigh = 0;                       // bits to compare (upper 16 bits)

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	return CAN_SUCCESS;
#endif

#ifdef FILTER_ACCEPT_STD // accept STD ID messages (IDE=0)
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterScale = CAN_FILTERSCALE_16BIT;
	filter.FilterIdLow = 0;
	filter.FilterIdHigh = 0;
	filter.FilterMaskIdLow = 0b00001000;
	filter.FilterMaskIdHigh = 0;

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}
#endif

#ifdef FILTER_ACCEPT_EXT_THIS_ID // accept EXT messages with destination = THIS_MODULE_ID
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filter.FilterScale = CAN_FILTERSCALE_32BIT;
	filter.FilterIdLow = GET_ID_LOW(THIS_MODULE_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filter.FilterIdHigh = GET_ID_HIGH(THIS_MODULE_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filter.FilterMaskIdLow = GET_ID_LOW(DEST_MASK);
	filter.FilterMaskIdHigh = GET_ID_HIGH(DEST_MASK);

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}
#endif

#ifdef FILTER_ACCEPT_EXT_ALL_ID // accept EXT messages with destination = ALL_MODULES_ID
	filter.FilterBank = banknum++;
    filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filter.FilterScale = CAN_FILTERSCALE_32BIT;
    filter.FilterIdLow = GET_ID_LOW(ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
    filter.FilterIdHigh = GET_ID_HIGH(ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filter.FilterMaskIdLow = GET_ID_LOW(DEST_MASK);
	filter.FilterMaskIdHigh = GET_ID_HIGH(DEST_MASK);

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filter.FilterBank = banknum++;
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filter) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}
#endif

	return CAN_SUCCESS;
}

/*************************************************
 * EVENT CALLBACKS
 * these functions are marked __weak, they should be redefined in application code
*************************************************/

// called by ISRs when a message is received
__weak void GCAN_onRX(CAN_HandleTypeDef* hcan) {}

// called when an error message (EXT ID) is received
__weak void GCAN_onError(U32 rx_time, U8 source_module, U16 parameter, U8 error_id) {}

/*************************************************
 * ISRs
*************************************************/

// triggered when a TX mailbox transmission is complete, mailbox now empty
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

// triggered when a TX mailbox transmission has been aborted, mailbox now empty
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef* hcan)
{
    service_can_tx_hardware(hcan);
}

// triggered when an RX FIFO has a pending message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	service_can_rx_hardware(hcan, CAN_RX_FIFO0);
	GCAN_onRX(hcan);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	service_can_rx_hardware(hcan, CAN_RX_FIFO1);
	GCAN_onRX(hcan);
}

/*************************************************
 * HARDWARE SERVICE
*************************************************/

// called by ISRs and service_can_tx(), moves messages from buffer to mailbox
static void service_can_tx_hardware(CAN_HandleTypeDef* hcan)
{
	CAN_BUS* bus = get_bus_from_hcan(hcan);

	// add messages to the TX mailboxes until they are full
	while (!IS_EMPTY(bus->tx_buffer) && HAL_CAN_GetTxMailboxesFreeLevel(hcan))
	{
		// get the next CAN message from the TX buffer (FIFO)
		CAN_MSG* message = GET_FROM_BUFFER(bus->tx_buffer, 0);

		// configure the settings/params of the CAN message
		message->header.TransmitGlobalTime = DISABLE;

		// add the message to the sending list
		U32 tx_mailbox_num;
		if (HAL_CAN_AddTxMessage(hcan, &(message->header), message->data, &tx_mailbox_num) != HAL_OK)
		{
			// hardware error (do not move the head as the message did not send, try again later)
			return;
		}

		// move the head now that the first element has been removed
		remove_from_front(bus->tx_buffer);
	}

	return;
}

// called by CAN RX ISRs, moves messages from FIFO to buffer
static void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox)
{
	// get all the pending RX messages from the RX mailbox and store into the RX buffer
	while (!IS_FULL(&RX_BUFF) && HAL_CAN_GetRxFifoFillLevel(hcan, rx_mailbox))
	{
		// set message to the correct pointer from the RX buffer (the "last" message in the buffer)
		CAN_MSG* message = GET_FROM_BUFFER(&RX_BUFF, RX_BUFF.fill);

		// Build the message from the registers on the STM32
		CAN_RxHeaderTypeDef rx_header;
		if (HAL_CAN_GetRxMessage(hcan, rx_mailbox, &rx_header, message->data) != HAL_OK)
		{
			// hardware error (do not move the head as the message did not send, try again later)
			return;
		}

		// modify the rx_buffer data to reflect the new message
		RX_BUFF.fill++;

		// move the header ID, RTR bit, and DLC into the GopherCAN message struct
		message->header.RTR = rx_header.RTR;
		message->header.DLC = rx_header.DLC;
		message->header.ExtId = rx_header.ExtId;
		message->header.StdId = rx_header.StdId;
		message->header.IDE = rx_header.IDE;
		message->rx_time = HAL_GetTick();

#ifdef ENABLE_ROUTING
		if (message->header.IDE == CAN_ID_EXT) { // routing only applies to EXT IDs
			MODULE_ID dest_module = GET_ID_DEST(message->header.ExtId);
			BUS_ID dest_bus = module_bus_number[dest_module];
			CAN_BUS* this_bus = get_bus_from_hcan(hcan);

			if (dest_bus != this_bus->id) {
				// try to retransmit message on the correct bus
				if (dest_module == ALL_MODULES_ID) {
					for (U8 i = 0; i < ALL_BUSSES; i++) {
						CAN_BUS *bus = BUSES[i];
						add_message_by_highest_prio(bus, message);
					}
				} else if (dest_module != THIS_MODULE_ID) {
					CAN_BUS *bus = BUSES[dest_bus];
					add_message_by_highest_prio(bus, message);
				}
			}
		}
#endif
	}
}

/*************************************************
 * BUFFER SERVICE
*************************************************/

// service_can_tx
//  Moves messages from TX buffer to mailbox. Designed to be called frequently (1ms).
void service_can_tx(CAN_HandleTypeDef* hcan)
{
	CAN_BUS* bus = get_bus_from_hcan(hcan);

    // protect buffer from RTOS thread switching and interrupts
    if (bus->tx_buffer->mutex != NULL) {
        if (osMutexWait(bus->tx_buffer->mutex, MUTEX_TIMEOUT))
        	return;
    }
    HAL_CAN_DeactivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY);

    service_can_tx_hardware(hcan);

    HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY);
    if (bus->tx_buffer->mutex != NULL) {
        osMutexRelease(bus->tx_buffer->mutex);
    }

    return;
}

// service_can_rx_buffer
//  Processes messages in the RX buffer. Designed to be called frequently (1ms).
S8 service_can_rx_buffer(void)
{
	while (!IS_EMPTY(&RX_BUFF))
	{
		// get the message at the head of the array
		CAN_MSG* current_message = GET_FROM_BUFFER(&RX_BUFF, 0);

		// WARNING: CAN errors from other modules are not handled in this version. The message is just discarded
		// Use a CAN bus analyzer to see what the message is for debugging
		if (current_message->header.IDE == CAN_ID_STD) {
		    service_can_rx_message_std(current_message);
		} else {
		    service_can_rx_message_ext(current_message);
		}

		// move the head now that the first element has been removed
		remove_from_front(&RX_BUFF);
	}

	return CAN_SUCCESS;
}

/*************************************************
 * CALLBACKS
*************************************************/

// attach_callback_std
//  Configure a function to be called when a particular STD ID is received.
//  Note that std_id must be a configured group ID.
// PARAMS:
//  U16 std_id: 11-bit CAN ID to trigger the callback
//  void (*func)(): function pointer accepting no arguments
void attach_callback_std(U16 std_id, void (*func)()) {
	PARAM_GROUP* group = NULL;
	size_t group_index = 0;

	for (U8 i = 0; i < NUM_OF_GROUPS; i++) {
		if (GROUPS[i].group_id == std_id) {
			group = &GROUPS[i];
			group_index = i;
			break;
		}
	}

	if (group != NULL) {
		CALLBACKS_STD[group_index] = func;
	}
}

// attach_callback_cmd
//  Configure a function to be called when a particular command is received.
// PARAMS:
//  U16 cmd_id: command ID to trigger the callback
//  void (*func)(): function pointer accepting the source module ID and four U8 arguments
void attach_callback_cmd(GCAN_COMMAND_ID cmd_id, void (*func)(MODULE_ID, U8, U8, U8, U8)) {
	CALLBACKS_CMD[cmd_id] = func;
}

/*************************************************
 * TX MESSAGE
*************************************************/

// tx_can_message
//  Adds a CAN message to TX buffers
static S8 tx_can_message(CAN_MSG* message)
{
	// remove any trailing zeros in the CAN message. This is done by starting at the
	// back of the message and decrementing the DLC for each byte in the message that
	// is zero at the back. RX logic will add zero bytes as needed
#ifndef DISABLE_TRIM_ZEROS
	while (message->header.DLC > 0 && message->data[message->header.DLC - 1] == 0)
	{
		message->header.DLC--;
	}
#endif

	// if extended ID, get destination
	// send standard ID data messages to all modules
	MODULE_ID module_id = message->header.IDE == CAN_ID_EXT ?
	        GET_ID_DEST(message->header.ExtId) :
	        ALL_MODULES_ID;

	if (module_id == ALL_MODULES_ID) {
        for (U8 i = 0; i < ALL_BUSSES; i++) {
			CAN_BUS* bus = BUSES[i];
			add_message_by_highest_prio(bus, message);
        }
    } else {
        BUS_ID bus_id = module_bus_number[module_id];
        CAN_BUS* bus = BUSES[bus_id];
        add_message_by_highest_prio(bus, message);
    }

    return CAN_SUCCESS;
}

/*************************************************
 * RX MESSAGE
*************************************************/

// service_can_rx_message_std
// handle standard ID CAN messages (data messages)
// finds the specified group and decodes parameters
static S8 service_can_rx_message_std(CAN_MSG* message)
{
    PARAM_GROUP* group = NULL;
    size_t group_index = 0;

    // find the specified parameter group
    for (U8 i = 0; i < NUM_OF_GROUPS; i++) {
        if (GROUPS[i].group_id == message->header.StdId) {
            group = &GROUPS[i];
            group_index = i;
            break;
        }
    }

    if (group == NULL) return NOT_FOUND_ERR;

    // decode parameters
    S8 err;

    for (U8 i = 0; i < CAN_DATA_BYTES; i++)
    {
        GCAN_PARAM_ID id = group->param_ids[i];
        if (id == EMPTY_ID) continue;

        // check to make sure this is a good id. We are down bad if it is not
        if (id < EMPTY_ID || id >= NUM_OF_PARAMETERS) return BAD_PARAMETER_ID;

        // decode this parameters data from the message
        // update last_rx if there was no error decoding
        CAN_INFO_STRUCT* param = (CAN_INFO_STRUCT*) PARAMETERS[id];

        err = decode_parameter(param, message->data, i, param->ENC_SIZE);
        if (!err) param->last_rx = message->rx_time;
    }

    // trigger callback for this group if one has been attached
    if (CALLBACKS_STD[group_index] != NULL) {
    	(*CALLBACKS_STD[group_index])();
    }

    return CAN_SUCCESS;
}

// service_can_rx_message_ext
// handle extended ID CAN messages (commands/errors)
static S8 service_can_rx_message_ext(CAN_MSG* message)
{
	CAN_ID id;
	id.priority = GET_ID_PRIO(message->header.ExtId);
	id.dest_module = GET_ID_DEST(message->header.ExtId);
	id.source_module = GET_ID_SOURCE(message->header.ExtId);
	id.error = GET_ID_ERROR(message->header.ExtId);
	id.parameter = GET_ID_PARAM(message->header.ExtId);

	// A double check to make sure this message is actually for this module (most useful in the CAN router)
	if (id.dest_module != THIS_MODULE_ID && id.dest_module != ALL_MODULES_ID)
	{
		// This is not for this module. Do not process this message
		return WRONG_DEST_ERR;
	}

	// if the message received has the error flag high, put the details into the last_error struct, then return
	if (id.error)
	{
		GCAN_onError(message->rx_time, id.source_module, id.parameter, message->header.DLC > 0 ? message->data[0] : 0);

		// return success because the problem is not with the RX
		return CAN_SUCCESS;
	}

	// error checking on the parameter requested
	if (id.parameter < EMPTY_ID || id.parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(&id, ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}

    // run command: run the command specified by the CAN message on this module
	if (id.parameter == EMPTY_ID)
	{
		return run_can_command(message, &id);
	}

	// request parameter: return a CAN message with the data taken from this module
	if (message->header.RTR)
	{
		return parameter_requested(message, &id);
	}

	// EXT ID but not a command/request/error - unknown message
	return NOT_IMPLEMENTED;
}

/*************************************************
 * DATA MESSAGE (STD 11-bit ID)
*************************************************/

// send_parameter
//  Sends the group containing a parameter.
S8 send_parameter(GCAN_PARAM_ID param_id)
{
	return send_group(((CAN_INFO_STRUCT*)PARAMETERS[param_id])->GROUP_ID);
}

// send_group
//  Encodes and transmits a group of parameters.
// PARAMS:
//  U16 group_id: the CAN ID of the group to be sent
// RETURNS:
//  error codes specified in GopherCAN.h
S8 send_group(U16 group_id)
{
    PARAM_GROUP* group = NULL;

    // find the specified parameter group
    for (U8 i = 0; i < NUM_OF_GROUPS; i++)
    {
        if (GROUPS[i].group_id == group_id)
        {
            group = &GROUPS[i];
            break;
        }
    }

    if (group == NULL) return NOT_FOUND_ERR;

    // build parameter group message. Setting the data to 0 means that training zeros
    // will be properly removed when sending the message, cutting down the DLC with it
    CAN_MSG message = {
        .header = {
            .StdId = group_id,
            .IDE = CAN_ID_STD,
            .RTR = DATA_MESSAGE,
            .DLC = CAN_DATA_BYTES
        },
        .data = {0}
    };

    // run through all of the bytes in the group, putting the correct data in them based
    // on the parameters that are in this group
    S8 err;
    for (U8 i = 0; i < CAN_DATA_BYTES; i++)
    {
        GCAN_PARAM_ID id = group->param_ids[i];
        if (id == EMPTY_ID) continue;

        // check to make sure this is a good id. We are down bad if it is not
        if (id < EMPTY_ID || id >= NUM_OF_PARAMETERS) return BAD_PARAMETER_ID;

        // add this parameter's data to the message
        CAN_INFO_STRUCT* parameter = (CAN_INFO_STRUCT*) PARAMETERS[id];
        err = encode_parameter(parameter, message.data, i, parameter->ENC_SIZE);
        if (err) return err;
    }

    // send the message
    err = tx_can_message(&message);
    if (err) return err;

    // if successful send, update the last_tx for all of the sent parameters
    for (U8 i = 0; i < CAN_DATA_BYTES; i++)
    {
        GCAN_PARAM_ID id = group->param_ids[i];
        if (id != EMPTY_ID) ((CAN_INFO_STRUCT*)PARAMETERS[id])->last_tx = HAL_GetTick();
    }

    return CAN_SUCCESS;
}


// encode_parameter
// encodes a parameter as an unsigned int with scale & offset
// adds encoded param to the CAN message data field
static S8 encode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length)
{
    U64 value = 0;

    // apply quantization and store in U64
    // use scale = 1 if necessary to avoid divide by 0 due to truncation
    switch (param->TYPE) {
        case UNSIGNED8:
            value = (((U8_CAN_STRUCT*)param)->data - (U8)param->OFFSET)
                / ((U8)param->SCALE + ((U8)param->SCALE == 0));
            break;
        case SIGNED8:
            value = (((S8_CAN_STRUCT*)param)->data - (S8)param->OFFSET)
                / ((S8)param->SCALE + ((S8)param->SCALE == 0));
            break;
        case UNSIGNED16:
            value = (((U16_CAN_STRUCT*)param)->data - (U16)param->OFFSET)
                / ((U16)param->SCALE + ((U16)param->SCALE == 0));
            break;
        case SIGNED16:
            value = (((S16_CAN_STRUCT*)param)->data - (S16)param->OFFSET)
                / ((S16)param->SCALE + ((S16)param->SCALE == 0));
            break;
        case UNSIGNED32:
            value = (((U32_CAN_STRUCT*)param)->data - (U32)(param->OFFSET))
                / ((U32)param->SCALE + ((U32)param->SCALE == 0));
            break;
        case SIGNED32:
            value = (((S32_CAN_STRUCT*)param)->data - (S32)(param->OFFSET))
                / ((S32)param->SCALE + ((S32)param->SCALE == 0));
            break;
        case UNSIGNED64:
            value = (((U64_CAN_STRUCT*)param)->data - (U64)param->OFFSET)
                / ((U64)param->SCALE + ((U64)param->SCALE == 0));
            break;
        case SIGNED64:
            value = (((S64_CAN_STRUCT*)param)->data - (S64)param->OFFSET)
                / ((S64)param->SCALE + ((S64)param->SCALE == 0));
            break;
        case FLOATING:
            // send floats as signed values
            value = (S64)( (((FLOAT_CAN_STRUCT*)param)->data - param->OFFSET) / param->SCALE );
            break;
        default:
            return ENCODING_ERR;
    }

    // move bytes into data field
    for (U8 i = 0; i < length; i++) {
        if (param->ENC == LSB) {
            data[start + i] = value >> (i * BITS_IN_BYTE);
        } else if (param->ENC == MSB) {
            data[start + i] = value >> ((length - 1 - i) * BITS_IN_BYTE);
        } else return ENCODING_ERR;
    }

    return CAN_SUCCESS;
}

// decode_parameter
// extract and decode a parameter from CAN data field
static S8 decode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length)
{
    U64 value = 0;
    float value_fl = 0;

    // reconstruct U64
    for (U8 i = 0; i < length; i++) {
        if (param->ENC == LSB) {
            value |= data[start + i] << (i * BITS_IN_BYTE);
        } else if (param->ENC == MSB) {
            value |= data[start + i] << ((length - 1 - i) * BITS_IN_BYTE);
        } else return DECODING_ERR;
    }

    // restore original type
    switch (param->TYPE) {
        case UNSIGNED8:
            ((U8_CAN_STRUCT*)param)->data = (value * (U8)param->SCALE) + (U8)param->OFFSET;
            break;
        case SIGNED8:
            ((S8_CAN_STRUCT*)param)->data = (value * (S8)param->SCALE) + (S8)param->OFFSET;
            break;
        case UNSIGNED16:
            ((U16_CAN_STRUCT*)param)->data = (value * (U16)param->SCALE) + (U16)param->OFFSET;
            break;
        case SIGNED16:
            ((S16_CAN_STRUCT*)param)->data = (value * (S16)param->SCALE) + (S16)param->OFFSET;
            break;
        case UNSIGNED32:
            ((U32_CAN_STRUCT*)param)->data = (value * (U32)param->SCALE) + (U32)param->OFFSET;
            break;
        case SIGNED32:
            ((S32_CAN_STRUCT*)param)->data = (value * (S32)param->SCALE) + (S32)param->OFFSET;
            break;
        case UNSIGNED64:
            ((U64_CAN_STRUCT*)param)->data = (value * (U64)param->SCALE) + (U64)param->OFFSET;
            break;
        case SIGNED64:
            ((S64_CAN_STRUCT*)param)->data = (value * (S64)param->SCALE) + (S64)param->OFFSET;
            break;
        case FLOATING:
            // floats are signed values in data frame
#ifdef PLM_JANK
        	// this is to make sure the IMU logs correctly
        	if (param->GROUP_ID == 0x174 || param->GROUP_ID == 0x178 || param->GROUP_ID == 0x17C)
        	{
        		value_fl = (U16)value;
        	}
        	else
        	{
				if (length == 1) value_fl = (S8)value;
				else if (length == 2) value_fl = (S16)value;
				else if (length == 4) value_fl = (S32)value;
				else if (length == 8) value_fl = (S64)value;
				else value_fl = value;
        	}
#else
            if (length == 1) value_fl = (S8)value;
            else if (length == 2) value_fl = (S16)value;
            else if (length == 4) value_fl = (S32)value;
            else if (length == 8) value_fl = (S64)value;
            else value_fl = value;
#endif
            ((FLOAT_CAN_STRUCT*)param)->data = (value_fl * param->SCALE) + param->OFFSET;
            break;
        default:
            return DECODING_ERR;
    }

    return CAN_SUCCESS;
}

/*************************************************
 * PARAMETER REQUESTS (EXT 29-bit ID)
*************************************************/

// request_parameter
// 	This function will send out a CAN message requesting the parameter
//	given by the parameter ID from the module specified by the module ID
// params:
//  PRIORITY priority:        PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:    what module to request the parameter from
//  GCAN_PARAM_ID parameter:  what parameter to request
// returns:
//  error codes specified in GopherCAN.h
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter)
{
	CAN_MSG message;
	CAN_ID id;

	if (dest_module < 0 || dest_module >= NUM_OF_MODULES)
	{
		return BAD_MODULE_ID;
	}

	if (parameter <= EMPTY_ID || parameter >= NUM_OF_PARAMETERS)
	{
		return BAD_PARAMETER_ID;
	}

	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = THIS_MODULE_ID;
	id.error = FALSE;
	id.parameter = parameter;

	message.header.ExtId = build_message_id(&id);
	message.header.IDE = CAN_ID_EXT;
	message.header.RTR = REQUEST_DATA;
	message.header.DLC = REQ_PARAM_SIZE;

	// set the pending response to true for this parameter, will be set to true once
	// the value is received from the CAN bus
	((CAN_INFO_STRUCT*)(PARAMETERS[parameter]))->pending_response = TRUE;

	return tx_can_message(&message);
}

// parameter_requested
//  return a CAN message with the data taken from this module
static S8 parameter_requested(CAN_MSG* message, CAN_ID* id)
{
	if (message->header.DLC != REQ_PARAM_SIZE)
	{
		send_error_message(id, SIZE_ERROR);

		return SIZE_ERR;
	}

	// the requested parameter is stored in id->parameter of the received CAN message
	if (id->parameter <= EMPTY_ID || id->parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(id, ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}

	// send the parameter data to the module that requested
	return send_parameter(id->parameter);
}

/*************************************************
 * COMMANDS (EXT 29-bit ID)
*************************************************/

// send_can_command
//	This function will send a CAN message with a command specified by command_id to the specified module
// params:
//  PRIORITY priority:          PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:      what module to send the command to
//  GCAN_COMMAND_ID command_id: what command the module should run
//  U8 a0:         argument 0 to run the function with. May not be used depending on the function
//  U8 a1:         argument 1
//  U8 a2:         argument 2
//  U8 a3:         argument 3
// returns:
//  error codes specified in GopherCAN.h
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
					U8 a0, U8 a1, U8 a2, U8 a3)
{
	CAN_MSG message;
	CAN_ID id;

	if (dest_module < 0 || dest_module >= NUM_OF_MODULES)
	{
		return BAD_MODULE_ID;
	}

	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = THIS_MODULE_ID;
	id.error = FALSE;
	id.parameter = EMPTY_ID;

	message.header.ExtId = build_message_id(&id);
	message.header.IDE = CAN_ID_EXT;
	message.header.RTR = DATA_MESSAGE;
	message.header.DLC = COMMAND_SIZE;

	message.data[0] = command_id;
	message.data[1] = a0;
	message.data[2] = a1;
	message.data[3] = a2;
	message.data[4] = a3;

	return tx_can_message(&message);
}

// run_can_command
//  run the command specified by the CAN message on this module
static S8 run_can_command(CAN_MSG* message, CAN_ID* id)
{
	// DLC error checking
	if (message->header.DLC > COMMAND_SIZE)
	{
		// there is either no DLC (no function ID) or too many parameters sent
		send_error_message(id, SIZE_ERROR);
		return SIZE_ERR;
	}

	// fill in the extra DLC of the message with zeros
	for (U8 c = message->header.DLC; c < COMMAND_SIZE; c++)
	{
		message->data[c] = 0;
	}

	// error checking on the command ID
	GCAN_COMMAND_ID command_id = message->data[0];
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		send_error_message(id, COMMAND_ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}

	// trigger command callback if one has been attached
	if (CALLBACKS_CMD[command_id] != NULL) {
		(*CALLBACKS_CMD[command_id])(
				id->source_module,
				message->data[1], message->data[1], message->data[3], message->data[4]);
	}

	return CAN_SUCCESS;
}

/*************************************************
 * ERROR MESSAGES (EXT 29-bit ID)
*************************************************/

// send_error_message
//  Sends a return message to the original sender with the ID specified
static S8 send_error_message(CAN_ID* rx_id, GCAN_ERROR_ID error_id)
{
	CAN_MSG message;
	CAN_ID tx_id;

	// create the CAN ID for the error message
	tx_id.priority = rx_id->priority;
	tx_id.dest_module = rx_id->source_module;
	tx_id.source_module = THIS_MODULE_ID;
	tx_id.error = TRUE;
	tx_id.parameter = rx_id->parameter;

	message.header.ExtId = build_message_id(&tx_id);
	message.header.IDE = CAN_ID_EXT;
	message.header.RTR = DATA_MESSAGE;
	message.header.DLC = sizeof(error_id);
	message.data[0] = error_id;

	// send the CAN message
	return tx_can_message(&message);
}

/*************************************************
 * MESSAGE BUFFERS
*************************************************/

static CAN_BUS* get_bus_from_hcan(CAN_HandleTypeDef* hcan)
{
	for (U8 i = 0; i < ALL_BUSSES; i++) {
		CAN_BUS* bus = BUSES[i];
		if (bus->hcan == hcan) {
			return bus;
		}
	}
	return NULL;
}

//  will remove the first element of the ring buffer. If the buffer is empty it will do nothing
static void remove_from_front(CAN_BUFFER* buffer)
{
    if (IS_EMPTY(buffer)) return;

    // move the head to the next element
    buffer->head = (buffer->head + 1) % buffer->size;

    // decrement the fill level
    buffer->fill--;
}

// add_message_by_highest_prio
//  This function will add message to the buffer based on the ID of the message. Higher
//  priority messages (lower ID) will be towards the front, with lower priority
//  messages (greater ID) will be towards the back. Removing from the front will get
//  the highest priority message.
static void add_message_by_highest_prio(CAN_BUS* bus, CAN_MSG* message)
{
    // protect buffer from RTOS thread switching and interrupts
    if (bus->tx_buffer->mutex != NULL) {
        if(osMutexWait(bus->tx_buffer->mutex, MUTEX_TIMEOUT)) return;
    }
    HAL_CAN_DeactivateNotification(bus->hcan, CAN_IT_TX_MAILBOX_EMPTY);

    if (IS_FULL(bus->tx_buffer))
    {
        if (bus->tx_buffer->mutex != NULL) {
            osMutexRelease(bus->tx_buffer->mutex);
        }
        return;
    }

    CAN_MSG* buffer_message = GET_FROM_BUFFER(bus->tx_buffer, 0);
    S16 c;

    // start from the back of the buffer, moving each message towards the back
    // by one and put the new message in the correct spot by ID. If the buffer
    // was empty when the message first went through here, it will put the new
    // message in position 0
    bus->tx_buffer->fill++;
    for (c = bus->tx_buffer->fill - 2; c >= 0; c--)
    {
        buffer_message = GET_FROM_BUFFER(bus->tx_buffer, c);

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
            buffer_message = GET_FROM_BUFFER(bus->tx_buffer, c + 1);
            break;
        }

        // move this message back by 1 and try again
        copy_message(buffer_message, GET_FROM_BUFFER(bus->tx_buffer, c + 1));
    }

    // put the message into the buffer at this position
    copy_message(message, buffer_message);

    HAL_CAN_ActivateNotification(bus->hcan, CAN_IT_TX_MAILBOX_EMPTY);
    if (bus->tx_buffer->mutex != NULL) {
        osMutexRelease(bus->tx_buffer->mutex);
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

/*************************************************
 * OTHER FUNCTIONS
*************************************************/

// build_can_id
//  this function will fill in the id of msg when called.
//  No error checking is preformed in this function besides masking
static U32 build_message_id(CAN_ID* id)
{
	U32 temp;
	U32 msg_id = 0;

	// priority bit
	temp = !!id->priority;
	temp <<= (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE);
	temp &= PRIORITY_MASK;
	msg_id |= temp;

	// destination bits
	temp = id->dest_module;
	temp <<= (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	temp &= DEST_MASK;
	msg_id |= temp;

    // source bits
	temp = id->source_module;
	temp <<= (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE);
	temp &= SOURCE_MASK;
	msg_id |= temp;

	// error bit
	temp = id->error;
	temp <<= (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE);
	temp &= ERROR_MASK;
	msg_id |= temp;

	// parameter bits
	temp = id->parameter;
	temp <<= (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE);
	temp &= PARAM_MASK;
	msg_id |= temp;

	return msg_id;
}
