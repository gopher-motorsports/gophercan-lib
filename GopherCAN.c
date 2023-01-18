// GopherCAN.c
//  This function houses the functionality of GopherCAN, including handling
//  hardware, interrupts, and buffers. Functions will also automatically
//  return request from other modules and call CAN commands

#include "GopherCAN.h"
#include "GopherCAN_network.h"
#include "GopherCAN_buffers.h"

// static function prototypes
static S8   init_filters(CAN_HandleTypeDef* hcan, BXCAN_TYPE bx_type);
static S8   parameter_requested(CAN_MSG* message, CAN_ID* id);
static S8   run_can_command(CAN_MSG* message, CAN_ID* id);
static U32 build_message_id(CAN_ID* id);
static void get_message_id(CAN_ID* id, CAN_MSG* message);
static S8   send_error_message(CAN_ID* id, U8 error_id);
static S8   tx_can_message(CAN_MSG* message);
static S8   service_can_rx_message_std(CAN_MSG* message);
static S8   service_can_rx_message_ext(CAN_MSG* message);
static S8 encode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length);
static S8 decode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length);
static void rout_can_message(CAN_HandleTypeDef* hcan, CAN_MSG* message);
static void service_can_tx_hardware(CAN_HandleTypeDef* hcan);


// all of the custom functions and an array to enable or disable
// each command ID corresponds to an index in the array
CUST_FUNC cust_funcs[NUM_OF_COMMANDS];

// what module this is configured to be
U8 this_module_id;

// a struct to store the last error type message received
ERROR_MSG last_error;

// stores the last hcan error code
U32 hcan_error = HAL_CAN_ERROR_NONE;


// init_can
//  This function will set up the CAN registers with the inputed module_id
//  as a filter. All parameters that should be enabled should be set after
//  calling this function
// params:
//  U8 bus_id:               CAN bus identifier (GCAN0/1/2)
//  CAN_HandleTypeDef* hcan: the BXcan hcan pointer from the STM HAL library
//  MODULE_ID module_id:     what module this is (ex. PDM_ID, ACM_ID)
//  BXCAN_TYPE bx_type:      master or slave BXcan type. This is usually BXTYPE_MASTER
// returns:
//  error codes specified in GopherCAN.h
S8 init_can(U8 bus_id, CAN_HandleTypeDef* hcan, MODULE_ID module_id, BXCAN_TYPE bx_type)
{
	U8 c;

	// set the current module
	this_module_id = module_id;

	// check for invalid bus ID
	if (!(bus_id == GCAN0 || bus_id == GCAN1 || bus_id == GCAN2)) {
	    return INIT_FAILED;
	}

	// attach hcan and mutex to appropriate buffer
#if NUM_OF_BUSSES > 2
    if (bus_id == GCAN2) {
        txbuff2.hcan = hcan;
        txbuff2.mutex = osMutexNew(&txbuff2_mutex_attr);
    }
#endif
#if NUM_OF_BUSSES > 1
    if (bus_id == GCAN1) {
        txbuff1.hcan = hcan;
        txbuff1.mutex = osMutexNew(&txbuff1_mutex_attr);
    }
#endif
    if (bus_id == GCAN0) {
        txbuff0.hcan = hcan;
        txbuff0.mutex = osMutexNew(&txbuff0_mutex_attr);
    }

	// init HAL_GetTick()
	HAL_SetTickFreq(HAL_TICK_FREQ_DEFAULT);

	// set each function pointer to the do_nothing() function
	for (c = 0; c < NUM_OF_COMMANDS; c++)
	{
		cust_funcs[c].func_ptr = &do_nothing;
		cust_funcs[c].func_enabled = FALSE;
		cust_funcs[c].param_ptr = NULL;

	}

	if (init_filters(hcan, bx_type))
	{
		return FILTER_SET_FAILED;
	}

	// Setup the rx interrupt function to interrupt on any pending message
	// will call methods following the format HAL_CAN_xxxCallback()
	if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK
			|| HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
	{
		return IRQ_SET_FAILED;
	}

	// The F7xx includes interrupts for when a message is complete. Activate them here
#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
	if (HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
	{
		return IRQ_SET_FAILED;
	}
#endif

	// start can!
	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		return CAN_START_FAILED;
	}

	return CAN_SUCCESS;
}


// init_filters
//  function called within init() that sets up all of the filters
static S8 init_filters(CAN_HandleTypeDef* hcan, BXCAN_TYPE bx_type)
{
	CAN_FilterTypeDef filterConfig;
	U8 banknum = 0;

	if (bx_type == BXTYPE_SLAVE)
	{
		banknum = SLAVE_FIRST_FILTER;
	}

	filterConfig.FilterActivation = CAN_FILTER_ENABLE;
    filterConfig.FilterMode = CAN_FILTERMODE_IDMASK; // use mask mode to filter

#if defined(CAN_ROUTER) || defined(NO_FILTER)
	// accept all messages on the CAN router
	filterConfig.FilterBank = banknum++;                     // modify bank 0 (of 13)
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;    // use FIFO0
	filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;        // 32 bit mask
	filterConfig.FilterIdLow = 0;                            // accepted IDs (lower 16 bits)
	filterConfig.FilterIdHigh = 0;                           // accepted IDs (upper 16 bits)
	filterConfig.FilterMaskIdLow = 0;                        // bits to compare (lower 16 bits)
	filterConfig.FilterMaskIdHigh = 0;                       // bits to compare (upper 16 bits)

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}
#else

#ifndef IGNORE_DATA
	// accept STD ID messages (IDE=0)
	filterConfig.FilterBank = banknum++;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterConfig.FilterScale = CAN_FILTERSCALE_16BIT; // 16 bit scale includes IDE bit
	filterConfig.FilterIdLow = 0;
	filterConfig.FilterIdHigh = 0;
	filterConfig.FilterMaskIdLow = 0b00001000;
	filterConfig.FilterMaskIdHigh = 0;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filterConfig.FilterBank = banknum++;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}
#endif

	U32 filt_id_low;
    U32 filt_id_high;
    U32 filt_mask_high;
    U32 filt_mask_low;

	// accept EXT messages with destination = this_module_id
	filt_id_high = GET_ID_HIGH(this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filt_id_low = GET_ID_LOW(this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filt_mask_high = GET_ID_HIGH(DEST_MASK);
    filt_mask_low = GET_ID_LOW(DEST_MASK);

	filterConfig.FilterBank = banknum++;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	filterConfig.FilterIdLow = filt_id_low;
	filterConfig.FilterIdHigh = filt_id_high;
	filterConfig.FilterMaskIdLow = filt_mask_low;
	filterConfig.FilterMaskIdHigh = filt_mask_high;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filterConfig.FilterBank = banknum++;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// accept EXT messages with destination = ALL_MODULES_ID
	filt_id_high = GET_ID_HIGH(ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filt_id_low = GET_ID_LOW(ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE));
	filt_mask_high = GET_ID_HIGH(DEST_MASK);
	filt_mask_low = GET_ID_LOW(DEST_MASK);

	filterConfig.FilterBank = banknum++;
    filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    filterConfig.FilterIdLow = filt_id_low;
    filterConfig.FilterIdHigh = filt_id_high;
	filterConfig.FilterMaskIdLow = filt_mask_low;
	filterConfig.FilterMaskIdHigh = filt_mask_high;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// apply same filter to FIFO1
	filterConfig.FilterBank = banknum++;
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

#endif // CAN_ROUTER

	return CAN_SUCCESS;
}


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
	id.source_module = this_module_id;
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


// send_can_command
//	This function will send a CAN message with a command specified
//	by command_id to the specified module
// params:
//  PRIORITY priority:          PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:      what module to send the command to
//  GCAN_COMMAND_ID command_id: what command the module should run
//  U8 command_param_0:         parameter 0 to run the function with. May not be used depending on the function
//  U8 command_param_1:         parameter 1
//  U8 command_param_2:         parameter 2
//  U8 command_param_3:         parameter 3
// returns:
//  error codes specified in GopherCAN.h
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
	U8 command_param_0, U8 command_param_1, U8 command_param_2, U8 command_param_3)
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
	id.source_module = this_module_id;
	id.error = FALSE;
	id.parameter = EMPTY_ID;

	message.header.ExtId = build_message_id(&id);
	message.header.IDE = CAN_ID_EXT;
	message.header.RTR = DATA_MESSAGE;
	message.header.DLC = COMMAND_SIZE;

	message.data[0] = command_id;
	message.data[1] = command_param_0;
	message.data[2] = command_param_1;
	message.data[3] = command_param_2;
	message.data[4] = command_param_3;

	return tx_can_message(&message);
}


// send_parameter
// encodes and sends the specified parameter's group in a standard 11-bit CAN frame
// params:
// CAN_INFO_STRUCT* param: parameter to send (along with its group)
// returns:
// error codes specified in GopherCAN.h
S8 send_parameter(CAN_INFO_STRUCT* param)
{
    PARAM_GROUP* group = NULL;

    // find the specified parameter group
    for (U8 i = 0; i < NUM_OF_GROUPS; i++) {
        if (GROUPS[i].id == param->GROUP_ID) {
            group = &GROUPS[i];
            break;
        }
    }

    if (group == NULL) return NOT_FOUND_ERR;

    // build parameter group message
    CAN_MSG message;

    message.header.StdId = param->GROUP_ID;
    message.header.IDE = CAN_ID_STD;
    message.header.RTR = DATA_MESSAGE;
    message.header.DLC = 8;

    // encode parameters
    U8 param_start = 0;
    U8 param_length = 1;
    S8 err;
    for (U8 i = 1; i < CAN_DATA_BYTES; i++) {
        if (group->slots[i] == group->slots[i-1]) {
            // count consecutive slots belonging to the same parameter
            param_length++;
        } else {
            // start of a new parameter, encode previous and write to data
            GCAN_PARAM_ID id = group->slots[i-1];
            if (id < EMPTY_ID || id >= NUM_OF_PARAMETERS) {
                return BAD_PARAMETER_ID;
            } else {
                CAN_INFO_STRUCT* param = (CAN_INFO_STRUCT*) PARAMETERS[id];
                err = encode_parameter(param, message.data, param_start, param_length);
                if (err) return err;
                else {
                    param->last_tx = HAL_GetTick();
                }
            }

            param_start = i;
            param_length = 1;
        }
    }

    return tx_can_message(&message);
}

// encode_parameter
// encodes a parameter as an unsigned int with scale & offset
// adds encoded param to the CAN message data field
static S8 encode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length)
{
    U64 value = 0;
    FLOAT_CONVERTER float_con;

    // represent value as U64
    switch (param->TYPE) {
        case UNSIGNED8:
        case SIGNED8:
            value |= ((U8_CAN_STRUCT*) param)->data;
            break;
        case UNSIGNED16:
        case SIGNED16:
            value |= ((U16_CAN_STRUCT*) param)->data;
            break;
        case UNSIGNED32:
        case SIGNED32:
            value |= ((U32_CAN_STRUCT*) param)->data;
            break;
        case UNSIGNED64:
        case SIGNED64:
            value |= ((U64_CAN_STRUCT*) param)->data;
            break;
        case FLOATING:
        	// TODO floating point I think is wrong, it should be fixed point
        	// on the bus
            float_con.f = ((FLOAT_CAN_STRUCT*) param)->data;
            value |= float_con.u32;
            break;
        default:
            return ENCODING_ERR;
    }

    // apply scale and offset to reduce bits required
    // TODO add parentheses to make it more clear what is first
    // TODO scale might be backwards
    value = value * param->SCALE - param->OFFSET;

    // move bytes into data field
    for (U8 i = 0; i < length; i++) {
        if (param->ENC == LSB) {
            data[start + i] = (U8)(value >> (i * BITS_IN_BYTE));
        } else if (param->ENC == MSB) {
            data[start + i] = (U8)(value >> ((param->SIZE - 1 - i) * BITS_IN_BYTE));
        } else return ENCODING_ERR;
    }

    return CAN_SUCCESS;
}


// decode_parameter
// extract and decode a parameter from CAN data field
static S8 decode_parameter(CAN_INFO_STRUCT* param, U8* data, U8 start, U8 length)
{
    U64 value = 0;
    FLOAT_CONVERTER float_con;

    // extract bytes into U64
    for (U8 i = 0; i < length; i++) {
        if (param->ENC == LSB) {
            value |= data[start + i] << (i * BITS_IN_BYTE);
        } else if (param->ENC == MSB) {
            value |= data[start + i] << ((param->SIZE - 1 - i) * BITS_IN_BYTE);
        } else return DECODING_ERR;
    }

    // undo scale and offset
    // TODO check this as well for encode/decode
    value = (value + param->OFFSET) / param->SCALE;

    // restore original type
    switch (param->TYPE) {
        case UNSIGNED8:
            ((U8_CAN_STRUCT*) param)->data = (U8) value;
            break;
        case UNSIGNED16:
            ((U16_CAN_STRUCT*) param)->data = (U16) value;
            break;
        case UNSIGNED32:
            ((U32_CAN_STRUCT*) param)->data = (U32) value;
            break;
        case UNSIGNED64:
            ((U64_CAN_STRUCT*) param)->data = (U64) value;
            break;
        case SIGNED8:
            ((S8_CAN_STRUCT*) param)->data = (S8) value;
            break;
        case SIGNED16:
            ((S16_CAN_STRUCT*) param)->data = (S16) value;
            break;
        case SIGNED32:
            ((S32_CAN_STRUCT*) param)->data = (S32) value;
            break;
        case SIGNED64:
            ((S64_CAN_STRUCT*) param)->data = (S64) value;
            break;
        case FLOATING:
            float_con.u32 = (U32) value;
            ((FLOAT_CAN_STRUCT*) param)->data = float_con.f;
            break;
        default:
            return DECODING_ERR;
    }

    return CAN_SUCCESS;
}


// add_custom_can_func
//  add a user function to the array of functions to check if
//  a CAN command message is sent. Note the functions must be of type 'void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8)',
//  so structs and casts are needed to get multiple params. The third-sixth parameter (U8, U8, U8, U8) will be
//  sent by the module in the CAN command message. This function can also be called to overwrite
//  or modify existing custom commands
// params:
//  GCAN_COMMAND_ID command_id:                         what command ID is being defined
//  void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8): the pointer to the function that should be run if this command_id is called
//  U8 init_state:                                      TRUE or FALSE, whether to start with the command enabled
//  void* param_ptr:                                    pointer to the parameter that should be used. This can point to any
//                                                       data type (including NULL) as long as it is casted correctly
// returns:
//  error codes specified in GopherCAN.h
S8 add_custom_can_func(GCAN_COMMAND_ID command_id, void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8),
	U8 init_state, void* param_ptr)
{
	CUST_FUNC* new_cust_func;

	// make sure the ID is valid
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	new_cust_func = &(cust_funcs[command_id]);

	// set all of the values of the struct accordingly
	new_cust_func->func_ptr       = func_ptr;
	new_cust_func->func_enabled   = !!init_state;
	new_cust_func->param_ptr      = param_ptr;

	return CAN_SUCCESS;
}


// mod_custom_can_func_state
//  change the state (enabled or disabled) of the specified custom CAN function
// params:
//  GCAN_COMMAND_ID command_id: what command ID should have its state modified
//  U8 state:                   TRUE or FALSE. what state to set this command to
// returns:
//  error codes specified in GopherCAN.h
S8 mod_custom_can_func_state(GCAN_COMMAND_ID command_id, U8 state)
{
	CUST_FUNC* this_cust_func;

	// make sure the ID is valid
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	this_cust_func = &(cust_funcs[command_id]);
	this_cust_func->func_enabled = !!state;

	return CAN_SUCCESS;
}


// service_can_tx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN commands.
//  then will fill as many tx mailboxes as possible from the tx_message_buffer
//
//  designed to be called at high priority on 1ms loop
static void service_can_tx_hardware(CAN_HandleTypeDef* hcan)
{
	CAN_MSG* message;
	CAN_MSG_RING_BUFFER* buffer;
    U32 tx_mailbox_num;

	// With multiple busses, choose the correct bus buffer to be working with
	buffer = choose_tx_buffer_from_hcan(hcan);

	// add messages to the the TX mailboxes until they are full
	while (!IS_EMPTY(buffer) && HAL_CAN_GetTxMailboxesFreeLevel(hcan))
	{
		// get the next CAN message from the TX buffer (FIFO)
		message = GET_FROM_BUFFER(buffer, 0);

		// configure the settings/params of the CAN message
		message->header.TransmitGlobalTime = DISABLE;

		// add the message to the sending list
		if (HAL_CAN_AddTxMessage(hcan, &(message->header), message->data, &tx_mailbox_num) != HAL_OK)
		{
			// this will always be HAL_ERROR. Check hcan->ErrorCode
			// hardware error (do not move the head as the message did not send, try again later)

			hcan_error = hcan->ErrorCode;
			return;
		}

		// move the head now that the first element has been removed
		remove_from_front(buffer);
	}

	return;
}

// service_can_tx
// Calls service_can_tx_hardware
// Acquires mutexes and temporarily disables interrupts
//  designed to be called at high priority on 1ms loop
void service_can_tx(CAN_HandleTypeDef* hcan)
{
    CAN_MSG_RING_BUFFER* buffer;

    // find buffer connected to this bus
    buffer = choose_tx_buffer_from_hcan(hcan);

    // protect buffer from RTOS thread switching
    if (buffer->mutex != NULL) {
        if(osMutexAcquire(buffer->mutex, MUTEX_TIMEOUT)) return;
    }
#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
    // protect buffer from interrupts
    HAL_CAN_DeactivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY);
#endif

    service_can_tx_hardware(hcan);

#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
    HAL_CAN_ActivateNotification(hcan, CAN_IT_TX_MAILBOX_EMPTY);
#endif
    if (buffer->mutex != NULL) {
        osMutexRelease(buffer->mutex);
    }

    return;
}


// service_can_rx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN functions.
//  Will take all messages from rx_mailbox (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//  and put them into the rx_buffer
// params:
// CAN_HandleTypeDef* hcan: the BXcan hcan pointer from the STM HAL library
//  U32 rx_mailbox:         the mailbox to service (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//                           Make sure this is valid, no error checking is done
//
//  designed to be called as an ISR whenever there is an RX message pending
void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox)
{
	CAN_RxHeaderTypeDef rx_header;
	CAN_MSG* message;

	// get all the pending RX messages from the RX mailbox and store into the RX buffer
	while (!IS_FULL(&rxbuff) && HAL_CAN_GetRxFifoFillLevel(hcan, rx_mailbox))
	{
		// set message to the correct pointer from the RX buffer (the "last" message in the buffer)
		message = GET_FROM_BUFFER(&rxbuff, rxbuff.fill_level);

		// Build the message from the registers on the STM32
		if (HAL_CAN_GetRxMessage(hcan, rx_mailbox, &rx_header, message->data) != HAL_OK)
		{
			// this will always return HAL_ERROR. Check hcan->ErrorCode
			// hardware error (do not move the head as the message did not send, try again later)

			hcan_error = hcan->ErrorCode;
			return;
		}

		// modify the rx_buffer data to reflect the new message
		rxbuff.fill_level++;

		// move the header ID, RTR bit, and DLC into the GopherCAN message struct
		message->header.RTR = rx_header.RTR;
		message->header.DLC = rx_header.DLC;
		message->header.ExtId = rx_header.ExtId;
		message->header.StdId = rx_header.StdId;
		message->header.IDE = rx_header.IDE;

#ifdef CAN_ROUTER
		// router specific functionality that directly adds messages that need to be routed
		//  directly to the correct TX buffer (if needed, that decision is made within the function)
		rout_can_message(hcan, message);
#endif // CAN_ROUTER
	}
}


// service_can_rx_buffer
//  this method will take all of the messages in rx_message_buffer and run them through
//  service_can_rx_message to return parameter requests, run CAN commands, and update
//  parameters.
//
//  WARNING: currently this function will not handle a full rx_message_buffer when returning
//   parameter requests. The request will not be completed and the other module will have to
//   send a new request
//
//  call in a 1 ms or faster loop
S8 service_can_rx_buffer(void)
{
	CAN_MSG* current_message;

	// run through each message in the RX buffer and service it with service_can_rx_message() (FIFO)
	while (!IS_EMPTY(&rxbuff))
	{
		// get the message at the head of the array
		current_message = GET_FROM_BUFFER(&rxbuff, 0);

		// WARNING: CAN errors from other modules are not handled in this version. The message is just discarded
		// Use a CAN bus analyzer to see what the message is for debugging
		if (current_message->header.IDE == CAN_ID_STD) {
		    service_can_rx_message_std(current_message);
		} else {
		    service_can_rx_message_ext(current_message);
		}

		// move the head now that the first element has been removed
		remove_from_front(&rxbuff);
	}

	return CAN_SUCCESS;
}


// tx_can_message
//  Takes in a CAN_MSG struct, adds it to the TX buffer
static S8 tx_can_message(CAN_MSG* message)
{
	// remove any trailing zeros in the CAN message. This is done by starting at the
	// back of the message and decrementing the DLC for each byte in the message that
	// is zero at the back. RX logic will add zero bytes as needed
	while (message->header.DLC > 0 && message->data[message->header.DLC - 1] == 0)
	{
		message->header.DLC--;
	}

	// if extended ID, get destination
	// send standard ID data messages to all modules
	MODULE_ID module_id = message->header.IDE == CAN_ID_EXT ?
	        GET_ID_DEST(message->header.ExtId) :
	        ALL_MODULES_ID;

	if (module_id == ALL_MODULES_ID) {
#if NUM_OF_BUSSES > 2
        add_message_by_highest_prio(&txbuff2, message);
#endif
#if NUM_OF_BUSSES > 1
        add_message_by_highest_prio(&txbuff1, message);
#endif
        add_message_by_highest_prio(&txbuff0, message);
    } else {
        CAN_MSG_RING_BUFFER* buffer = choose_tx_buffer_from_dest_module(module_id);
        add_message_by_highest_prio(buffer, message);
    }

    return CAN_SUCCESS;
}

// service_can_rx_message_std
// handle standard ID CAN messages (data messages)
// finds the specified group and decodes parameters
static S8 service_can_rx_message_std(CAN_MSG* message)
{
    PARAM_GROUP* group = NULL;

    // find the specified parameter group
    for (U8 i = 0; i < NUM_OF_GROUPS; i++) {
        if (GROUPS[i].id == message->header.StdId) {
            group = &GROUPS[i];
            break;
        }
    }

    if (group == NULL) return NOT_FOUND_ERR;

    // decode parameters
    U8 param_start = 0;
    U8 param_length = 1;
    S8 err;
    for (U8 i = 1; i < CAN_DATA_BYTES; i++) {
        if (group->slots[i] == group->slots[i-1]) {
            // count consecutive slots belonging to the same parameter
            param_length++;
        } else {
            // start of a new parameter, decode previous param
            GCAN_PARAM_ID id = group->slots[i-1];
            if (id < EMPTY_ID || id >= NUM_OF_PARAMETERS) {
                return BAD_PARAMETER_ID;
            } else {
                CAN_INFO_STRUCT* param = (CAN_INFO_STRUCT*) PARAMETERS[id];
                param->last_rx = HAL_GetTick();
                err = decode_parameter(param, message->data, param_start, param_length);
                if (err) return err;
                else {
                    param->pending_response = FALSE;
                }
            }

            param_start = i;
            param_length = 1;
        }
    }

    return CAN_SUCCESS;
}

// service_can_rx_message_ext
// handle extended ID CAN messages (commands/errors)
static S8 service_can_rx_message_ext(CAN_MSG* message)
{
	CAN_ID id;
	get_message_id(&id, message);

	// A double check to make sure this message is actually for this module (most useful in the CAN router)
	if (id.dest_module != this_module_id && id.dest_module != ALL_MODULES_ID)
	{
		// This is not for this module. Do not process this message
		return WRONG_DEST_ERR;
	}

	// if the message received has the error flag high, put the details into the last_error struct, then return
	if (id.error)
	{
		// this could possibly be changed into a ring buffer
		last_error.last_rx = HAL_GetTick();
		last_error.source_module = id.source_module;
		last_error.parameter = id.parameter;
		if (message->header.DLC > 0)
		{
			last_error.error_id = message->data[0];
		}

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
	CAN_INFO_STRUCT* param = (CAN_INFO_STRUCT*) PARAMETERS[id->parameter];
	return send_parameter(param);
}


// run_can_command
//  run the command specified by the CAN message on this module
static S8 run_can_command(CAN_MSG* message, CAN_ID* id)
{
	GCAN_COMMAND_ID command_id;
	CUST_FUNC* this_function;
	U8 c;

	// DLC error checking
	if (message->header.DLC > COMMAND_SIZE)
	{
		// there is either no DLC (no function ID) or too many parameters sent
		send_error_message(id, SIZE_ERROR);
		return SIZE_ERR;
	}

	// fill in the extra DLC of the message with zeros
	for (c = message->header.DLC; c < COMMAND_SIZE; c++)
	{
		message->data[c] = 0;
	}

	// error checking on the command ID
	command_id = message->data[COMMAND_ID_POS];
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		send_error_message(id, COMMAND_ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}

	this_function = &(cust_funcs[command_id]);

	// check if the function is enabled
	if (!this_function->func_enabled)
	{
		send_error_message(id, COMMAND_NOT_ENABLED);

		return NOT_ENABLED_ERR;
	}

	// run the function
	(*(this_function->func_ptr))(id->source_module, this_function->param_ptr,
		message->data[COMMAND_PARAM_0], message->data[COMMAND_PARAM_1],
		message->data[COMMAND_PARAM_2], message->data[COMMAND_PARAM_3]);

	return CAN_SUCCESS;
}


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


// get_message_id
//  this function will take in a CAN message and convert it to
//  a CAN id struct. No error checking is performed
static void get_message_id(CAN_ID* id, CAN_MSG* message)
{
	id->priority = GET_ID_PRIO(message->header.ExtId);
	id->dest_module = GET_ID_DEST(message->header.ExtId);
	id->source_module = GET_ID_SOURCE(message->header.ExtId);
	id->error = GET_ID_ERROR(message->header.ExtId);
	id->parameter = GET_ID_PARAM(message->header.ExtId);
}


// send_error_message
//  Sends a return message to the original sender with the ID specified
static S8 send_error_message(CAN_ID* rx_id, U8 error_id)
{
	CAN_MSG message;
	CAN_ID tx_id;

	// create the CAN ID for the error message
	tx_id.priority = rx_id->priority;
	tx_id.dest_module = rx_id->source_module;
	tx_id.source_module = this_module_id;
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


// rout_can_message
//  Function to be called in service_can_rx_hardware() that will take messages that are
//  destined for modules on another bus and put that message into the correct TX buffer
static void rout_can_message(CAN_HandleTypeDef* hcan, CAN_MSG* message)
{
	MODULE_ID dest_module = GET_ID_DEST(message->header.ExtId);

	// TX buffer on the CAN bus this message was received on
	CAN_MSG_RING_BUFFER* orig_tx_buffer = choose_tx_buffer_from_hcan(hcan);

	// TX buffer the message is intended for
	CAN_MSG_RING_BUFFER* dest_tx_buffer = choose_tx_buffer_from_dest_module(dest_module);

	// message is intended for all modules, retransmit on a new bus
	if (dest_module == ALL_MODULES_ID) {
#if NUM_OF_BUSSES > 2
		if (&txbuff2 != orig_tx_buffer) add_message_by_highest_prio(&txbuff2, message);
#endif
#if NUM_OF_BUSSES > 1
		if (&txbuff1 != orig_tx_buffer) add_message_by_highest_prio(&txbuff1, message);
#endif
		if (&txbuff0 != orig_tx_buffer) add_message_by_highest_prio(&txbuff0, message);
		return;
	}

	// message is intended for this module, don't retransmit
	if (dest_module == this_module_id) return;

	// message was already on the correct bus, don't retransmit
	if (dest_tx_buffer == orig_tx_buffer) return;

	// remove message from RX buffer and retransmit
	remove_from_front(&rxbuff);
	add_message_by_highest_prio(dest_tx_buffer, message);
}


// do_nothing
//  this exists to give a default function pointer to all of the CAN commands
//  to avoid errors from bad function pointers
void do_nothing(U8 sending_module, void* param,
	U8 remote_param0, U8 remote_param1, U8 remote_param2, U8 remote_param3)
{
	// this function has successfully done nothing
}


// HAL_CAN_RxFifo0MsgPendingCallback
//  ISR called when CAN_RX_FIFO0/FIFO1 has a pending message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    service_can_rx_hardware(hcan, CAN_RX_FIFO0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
    service_can_rx_hardware(hcan, CAN_RX_FIFO1);
}


// the F7xx has ISRs for available TX mailboxes having an opening. All callbacks should service the TX hardware
#if defined __STM32F4xx_HAL_H || defined __STM32F7xx_HAL_H
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
#endif

// end of GopherCAN.c
