/*
 * GopherCAN.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will have some auto generated sections

#include "GopherCAN.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"

// static function prototypes
static S8   tx_can_message(CAN_MSG* message);
static void parameter_requested(CAN_MSG* message, CAN_ID* id);
static void run_can_command(CAN_MSG* message, CAN_ID* id);
static void build_message_id(CAN_MSG* msg, CAN_ID* id);
static void get_message_id(CAN_ID* id, CAN_MSG* message);
static S8   send_error_message(CAN_ID* id, U8 error_id);
static void rx_can_message(void);


// what module this is configured to be
U8 this_module_id;

// all of the custom functions and an array to enable or disable
// each command ID corresponds to an index in the array
CUST_FUNC cust_funcs[NUM_OF_COMMANDS];

// a struct to store the last error type message received
ERROR_MSG last_error;

// the HAL_CAN struct
extern CAN_HandleTypeDef hcan;


// ******** BEGIN AUTO GENERATED ********

// all of the global parameter structs
REQ_PARAM_STRUCT req_param;
CAN_COMMAND_STRUCT can_command;
U16_CAN_STRUCT rpm;
U8_CAN_STRUCT fan_current;

// this is the struct that will be used to refrence based on ID
static void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
	&req_param,    // ID 0
	&can_command,    // ID 1
	&rpm,    // ID 2
	&fan_current    // ID 3
};

// this stores the data_type for each parameter, refrenced by ID
static U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
	REQ_PARAM,
	COMMAND,
	UNSIGNED16,
	UNSIGNED8
};

// ******** END AUTO GENERATED ********


// init_can
// 	This function will set up the CAN registers with the inputed module_id
//	as a filter. All parameters that should be enabled should be set after
//  calling this function
// params:
//  U8 module_id: what module this is (ex. PDM_ID, ACM_ID)
// returns:
//  error codes specified in GopherCAN.h
S8 init_can(U8 module_id)
{
	U8 c;
	CAN_INFO_STRUCT* data_struct;
	CAN_FilterTypeDef filterConfig;
	U32 filt_id_low;
	U32 filt_id_high;
	U32 filt_mask_high;
	U32 filt_mask_low;

	// set the current module
	this_module_id = module_id;

	// disable each parameter until the user manually enables them
	for (c = CAN_COMMAND_ID + 1; c < NUM_OF_PARAMETERS; c++)
	{
		data_struct = (CAN_INFO_STRUCT*)(all_parameter_structs + (c * sizeof(void*)));

		data_struct->last_rx = 0;
		data_struct->update_enabled = FALSE;
		data_struct->pending_response = FALSE;
	}


	// Define the filter values based on this_module_id
	// High and low id are the same because the id exclusively must be the module id
	filt_id_low = this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_id_high = this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_mask_low = DEST_MASK;
	filt_mask_high = DEST_MASK;

	// Set the the parameters on the filter struct
	filterConfig.FilterBank = 0;                                      // Modify bank 0
	filterConfig.FilterActivation = CAN_FILTER_ENABLE;                // enable the filter
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;             // enable FIFO, don't use a stack
	filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;                  // Use mask mode to filter
	filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;                 // 32 bit mask
	filterConfig.FilterIdLow = filt_id_low;                           // Low bound of accepted values
	filterConfig.FilterIdHigh = filt_id_high;                         // High bound of accepted values
	filterConfig.FilterMaskIdLow = filt_mask_low;                     // Which bits matter when filtering (high)
	filterConfig.FilterMaskIdHigh = filt_mask_high;                   // Which bits matter when filtering (low)

	if (HAL_CAN_ConfigFilter(&hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// CAN interrupts are currently disabled because Calvin can't
	// figure out how to make them work. poll_can_rx() is used instead

	/*
	// Setup the rx interrupt function to interrupt on any pending message
	// will supposedly call methods following the format HAL_CAN_xxxCallback()
	HAL_NVIC_SetPriority(CEC_CAN_IRQn, CAN_INTERRUPT_PRIO, 0);
	HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);
	HAL_CAN_IRQHandler(&hcan);
	if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
	{
		return IRQ_SET_FAILED;
	}
	*/

	// start can!
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{
		return CAN_START_FAILED;
	}

	return CAN_SUCCESS;
}


// request_parameter
// 	This function will send out a CAN message requesting the parameter
//	given by the parameter ID from the module specified by the module ID
// params:
//  U8 priority:    PRIO_LOW or PRIO_HIGH
//  U8 dest_module: what module to request the parameter from
//  U16 parameter:  what parameter to request
// returns:
//  error codes specified in GopherCAN.h
S8 request_parameter(U8 priority, U8 dest_module, U16 parameter)
{
	CAN_MSG message;
	CAN_ID id;

	if (dest_module < 0 || dest_module >= NUM_OF_MODULES)
	{
		return BAD_MODULE_ID;
	}

	if (parameter <= CAN_COMMAND_ID || parameter >= NUM_OF_PARAMETERS)
	{
		return BAD_PARAMETER_ID;
	}

	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = this_module_id;
	id.error = FALSE;
	id.parameter = REQUEST_VALUE_ID;

	// set the pending response to true for this parameter, will be set to true once
	// the value is recieved from the CAN bus
	((CAN_INFO_STRUCT*)(all_parameter_structs[parameter]))->pending_response = TRUE;

	build_message_id(&message, &id);

	message.dlc = sizeof(parameter);

    message.data[0] = parameter >> BITS_IN_BYTE;
	message.data[1] = parameter & U8_MAX;

	return tx_can_message(&message);
}


// send_can_command
//	This function will send a CAN message with a command specified
//	by command_id to the specified module
// params:
//  U8 priority:          PRIO_LOW or PRIO_HIGH
//  U8 dest_module:       what module to send the command to
//  U8 command_id:        what command the module should run
//  U8 command_parameter: the parameter to run the function with. May not be used depending on the function
// returns:
//  error codes specified in GopherCAN.h
S8 send_can_command(U8 priority, U8 dest_module, U8 command_id, U8 command_parameter)
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
	id.parameter = CAN_COMMAND_ID;

	build_message_id(&message, &id);

	message.dlc = sizeof(command_id) + sizeof(command_parameter);

	message.data[0] = command_id;
	message.data[1] = command_parameter;

	return tx_can_message(&message);
}


// add_custum_can_func
//  add a user function to the array of functions to check if
//  a CAN command message is sent. Note the functions must be of type 'void func(void*, U8)',
//  so structs and casts are needed to get multiple params. The second parameter (U8) will be
//  sent by the module in the CAN command message. This function can also be called to overwrite
//  or modify existing custom commands
// params:
//  U8 command_id:               what command ID is being defined
//  void (*func_ptr)(void*, U8): the pointer to the function that should be run if this command_id is called
//  U8 init_state:               TRUE or FALSE, whether to start with the command enabled
//  void* param_ptr:             pointer to the parameter that should be used. This can point to any
//                                data type (including nullptr) as long as it is casted correctly
// returns:
//  error codes specified in GopherCAN.h
S8 add_custom_can_func(U8 command_id, void (*func_ptr)(void*, U8), U8 init_state, void* param_ptr)
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
//  U8 command_id: what command ID should have its state modified
//  U8 state:      TRUE or FALSE. what state to set this command to
// returns:
//  error codes specified in GopherCAN.h
S8 mod_custom_can_func_state(U8 command_id, U8 state)
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


// tx_can_message
//  Takes in a CAN_MSG struct, modifies registers accordingly
static S8 tx_can_message(CAN_MSG* message)
{
	CAN_TxHeaderTypeDef header;
	U32 tx_mailbox;

	// configure the settings of the CAN message
	header.IDE = CAN_ID_EXT;                                          // 29 bit id
	header.RTR = CAN_RTR_DATA;                                        // sending data
	header.TransmitGlobalTime = DISABLE;                              // do not send a timestamp

	header.ExtId = message->id;
	header.DLC = message->dlc;

	// wait until there is a free mailbox
	while (!HAL_CAN_GetTxMailboxesFreeLevel(&hcan))
	{
		// TODO add some checking for a TX timeout, or possibly put in a separate CAN-handling loop
	}

	// add the message to the sending list
	if (HAL_CAN_AddTxMessage(&hcan, &header, message->data, &tx_mailbox) != HAL_OK)
	{
		return TX_PROBLEM_ADDING;
	}

	return CAN_SUCCESS;
}


// poll_can_rx
//  TODO docs
S8 poll_can_rx(void)
{
	if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0))
	{
		rx_can_message();

		return NEW_MESSAGE;
	}

	return NO_NEW_MESSAGE;
}


// HAL_CAN_RxCallback
//  CAN message bus interrupt function this will update all
//  the global variables or trigger the CAN functions if needed
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
static void rx_can_message(void)
{
	CAN_MSG message;
	CAN_ID id;
	CAN_RxHeaderTypeDef header;
	U64 recieved_data = 0;
	S8 c;
	CAN_INFO_STRUCT* data_struct = 0;
	FLOAT_CONVERTER float_con;

	// Build the message from the registers on the STM32
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &header, message.data) != HAL_OK)
	{
		// ERROR
		return;
	}

	// convert the data into the GopherCAN id and message
	message.id = header.ExtId;
	message.dlc = header.DLC;

	get_message_id(&id, &message);

	// TODO HAL hardware error handling (datasheet)

	// if the message received has the error flag high, put the details into the last_error struct, then return
	if (id.error)
	{
		// this could possibly be changed into a ring buffer
		last_error.last_rx = HAL_GetTick();
		last_error.source_module = id.source_module;
		last_error.parameter = id.parameter;
		if (message.dlc > 0)
		{
			last_error.error_id = message.data[0];
		}

		return;
	}

	// error checking on the parameter requested
	if (id.parameter < 0 || id.parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(&id, ID_NOT_FOUND);

		return;
	}
	
	// get the associated data struct and set last_rx
	data_struct = (CAN_INFO_STRUCT*)(all_parameter_structs[id.parameter]);
	data_struct->last_rx = HAL_GetTick();

	// build the data U64 (big endian)
	for (c = (message.dlc - 1); c >= 0; c--)
	{
		recieved_data |= message.data[c] << (c * BITS_IN_BYTE);
	}

	// request parameter: return a CAN message with the data taken from this module
	if (parameter_data_types[id.parameter] == REQ_PARAM)
	{
		parameter_requested(&message, &id);
		return;
	}

	// run command: run the command specified by the CAN message on this module
	if (parameter_data_types[id.parameter] == COMMAND)
	{
		run_can_command(&message, &id);
		return;
	}

	// this code should only be reached if the message is a data message

	// Check the update_enabled flag
	if (!(data_struct->update_enabled))
	{
		send_error_message(&id, PARAM_NOT_ENABLED);
		return;
	}

	// Switch the pending_response flag
	data_struct->pending_response = FALSE;

	// this switch will handle all of the different possible data types
	// that can be sent over CAN
	switch (parameter_data_types[id.parameter])
	{
	case UNSIGNED8:
		((U8_CAN_STRUCT*)(data_struct))->data = (U8)recieved_data;
		return;

	case UNSIGNED16:
		((U16_CAN_STRUCT*)(data_struct))->data = (U16)recieved_data;
		return;

	case UNSIGNED32:
		((U32_CAN_STRUCT*)(data_struct))->data = (U32)recieved_data;
		return;

	case UNSIGNED64:
		((U64_CAN_STRUCT*)(data_struct))->data = (U64)recieved_data;
		return;

	case SIGNED8:
		((S8_CAN_STRUCT*)(data_struct))->data = (S8)recieved_data;
		break;

	case SIGNED16:
		((S16_CAN_STRUCT*)(data_struct))->data = (S16)recieved_data;
		break;

	case SIGNED32:
		((S32_CAN_STRUCT*)(data_struct))->data = (S32)recieved_data;
		break;

	case SIGNED64:
		((S64_CAN_STRUCT*)(data_struct))->data = (S64)recieved_data;
		break;

	case FLOATING:
		// Union to get the bitwise data of the float
		float_con.u32 = (U32)recieved_data;

		((FLOAT_CAN_STRUCT*)(data_struct))->data = float_con.f;
		break;

	default:
		send_error_message(&id, DATATYPE_NOT_FOUND);
		break;
	}
}


// parameter_requested
//  return a CAN message with the data taken from this module
static void parameter_requested(CAN_MSG* message, CAN_ID* id)
{
	U16 parameter_requested;
	CAN_ID return_id;
	CAN_MSG return_message;
	U64 return_data = 0;
	S8 c;
	FLOAT_CONVERTER float_con;

	if (message->dlc != REQ_PARAM_SIZE)
	{
		send_error_message(id, SIZE_ERROR);

		return;
	}

	// find what the parameter is from the data
	parameter_requested = (message->data[0] << BITS_IN_BYTE) | message->data[1];
	req_param.parameter_id = parameter_requested;

	if (parameter_requested <= CAN_COMMAND_ID || parameter_requested >= NUM_OF_PARAMETERS)
	{
		send_error_message(id, ID_NOT_FOUND);

		return;
	}

	// build the return message ID
	return_id.priority = id->priority;
	return_id.dest_module = id->source_module;
	return_id.source_module = this_module_id;
	return_id.error = FALSE;
	return_id.parameter = parameter_requested;

	build_message_id(&return_message, &return_id);

	// get the value of the data on this module and build the CAN message
	if (parameter_data_types[parameter_requested] == UNSIGNED8
		|| parameter_data_types[parameter_requested] == SIGNED8)
	{
		return_data |= ((U8_CAN_STRUCT*)(all_parameter_structs[parameter_requested]))->data;
		return_message.dlc = sizeof(U8);
	}

	else if (parameter_data_types[parameter_requested] == UNSIGNED16
		|| parameter_data_types[parameter_requested] == SIGNED16)
	{
		return_data |= ((U16_CAN_STRUCT*)(all_parameter_structs[parameter_requested]))->data;
		return_message.dlc = sizeof(U16);
	}

	else if (parameter_data_types[parameter_requested] == UNSIGNED32
		|| parameter_data_types[parameter_requested] == SIGNED32)
	{
		return_data |= ((U32_CAN_STRUCT*)(all_parameter_structs[parameter_requested]))->data;
		return_message.dlc = sizeof(U32);
	}

	else if (parameter_data_types[parameter_requested] == UNSIGNED64
		|| parameter_data_types[parameter_requested] == SIGNED64)
	{
		return_data |= ((U64_CAN_STRUCT*)(all_parameter_structs[parameter_requested]))->data;
		return_message.dlc = sizeof(U64);
	}

	else if (parameter_data_types[parameter_requested] == FLOATING)
	{
		// Union to get the bitwise data of the float
		float_con.f = ((FLOAT_CAN_STRUCT*)(all_parameter_structs[parameter_requested]))->data;

		return_data |= float_con.u32;
		return_message.dlc = sizeof(float);
	}

	// build the data in the message (big endian)
	for (c = return_message.dlc - 1; c >= 0; c--)
	{
		return_message.data[c] = (U8)(return_data >> (c * BITS_IN_BYTE));
	}

	// send the built CAN message
	tx_can_message(&return_message);
	return;
}


// run_can_command
//  run the command specified by the CAN message on this module
static void run_can_command(CAN_MSG* message, CAN_ID* id)
{
	U8 command_id;
	CUST_FUNC* this_function;

	// DLC error checking
	if (message->dlc != COMMAND_SIZE)
	{
		send_error_message(id, SIZE_ERROR);

		return;
	}

	// error checking on the command ID
	command_id = message->data[COMMAND_ID_POS];
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		send_error_message(id, COMMAND_ID_NOT_FOUND);

		return;
	}

	this_function = &(cust_funcs[command_id]);

	// check if the function is enabled
	if (!this_function->func_enabled)
	{
		send_error_message(id, COMMAND_NOT_ENABLED);

		return;
	}

	// TODO default "do nothing" for each command just in case something goes very wrong
	// run the function
	(*(this_function->func_ptr))(this_function->param_ptr, message->data[COMMAND_PARAMETER_POS]);

	return;
}


// build_can_id
//  this function will fill in the id of msg when called.
//  No error checking is preformed in this function besides masking
static void build_message_id(CAN_MSG* msg, CAN_ID* id)
{
	U32 temp;

	msg->id = 0;

	// priority bit
	temp = !!id->priority;
	temp <<= (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE);
	temp &= PRIORITY_MASK;
	msg->id |= temp;

	// destination bits
	temp = id->dest_module;
	temp <<= (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	temp &= DEST_MASK;
	msg->id |= temp;

    // source bits
	temp = id->source_module;
	temp <<= (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE);
	temp &= SOURCE_MASK;
	msg->id |= temp;

	// error bit
	temp = id->error;
	temp <<= (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE);
	temp &= ERROR_MASK;
	msg->id |= temp;

	// parameter bits
	temp = id->parameter;
	temp <<= (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE);
	temp &= PARAM_MASK;
	msg->id |= temp;
}


// get_message_id
//  this function will take in a CAN message and convert it to
//  a CAN id struct. No error checking is performed
static void get_message_id(CAN_ID* id, CAN_MSG* message)
{
	U32 temp;

	// priority bit
	temp = message->id & PRIORITY_MASK;
	temp >>= (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE);
	id->priority = temp;

	// destination bits
	temp = message->id & DEST_MASK;
	temp >>= (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	id->dest_module = temp;

	// source bits
	temp = message->id & SOURCE_MASK;
	temp >>= (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE);
	id->source_module = temp;

	// error bit
	temp = message->id & ERROR_MASK;
	temp >>= (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE);
	id->error = temp;

	// parameter bits
	temp = message->id & PARAM_MASK;
	temp >>= (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE);
	id->parameter = temp;
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

	build_message_id(&message, &tx_id);

	// set the DLC and data
	message.dlc = sizeof(error_id);
	message.data[0] = error_id;

	// send the CAN message
	return tx_can_message(&message);
}

// end GopherCAN.c
