/*
 * GopherCAN.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will have some auto generated sections

#include "GopherCAN.h"

// static function prototypes
static U8 tx_can_message(CAN_MSG message);
static void rx_can_message();
static void build_message_id(CAN_MSG* msg, CAN_ID* id);
static void get_message_id(CAN_ID* id, CAN_MSG* message);


// fields
U8 this_modual_id;

// all of the custom functions and an array to enable or disable
// each command ID corresponds to an index in the array
CUST_FUNC cust_funcs[NUM_OF_COMMANDS];


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
//	as a filter
U8 init_can(U8 module_id)
{
	this_modual_id = module_id;

	// TODO init each parameter struct to diabled

	// TODO set the the registers and filters

	// TODO setup the interrupt function

	// TODO init HAL_TICK

	return NOT_IMPLEMENTED;
}


// request_parameter
// 	This function will send out a CAN message requesting the parameter
//	given by the parameter ID from the module specified by the module ID
U8 request_parameter(U8 priority, U8 dest_module, U16 parameter)
{
	CAN_MSG message;
	CAN_ID id;

	if (dest_module < 0 || dest_module >= NUM_OF_MODUALS)
	{
		return BAD_MODULE_ID;
	}

	if (parameter < 0 || parameter >= NUM_OF_PARAMETERS)
	{
		return BAD_PARAMETER_ID;
	}

	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = this_modual_id;
	id.error = FALSE;
	id.parameter = REQUEST_VALUE_ID;

	build_message_id(&message, &id);

	message.dlc = sizeof(parameter);

    message.data[0] = parameter >> BITS_IN_BYTE;
	message.data[1] = parameter & U8_MAX;

	return tx_can_message(message);
}


// send_can_command
//	This function will send a CAN message with a command specified
//	by command_id to the specified module
U8 send_can_command(U8 priority, U8 dest_module, U8 command_id, U8 command_parameter)
{
	CAN_MSG message;
	CAN_ID id;

	if (dest_module < 0 || dest_module >= NUM_OF_MODUALS)
	{
		return BAD_MODULE_ID;
	}

	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = this_modual_id;
	id.error = FALSE;
	id.parameter = CAN_COMMAND_ID;

	build_message_id(&message, &id);

	message.dlc = sizeof(command_id) + sizeof(command_parameter);

	message.data[0] = command_id;
	message.data[1] = command_parameter;

	return tx_can_message(message);
}


// add_custum_can_func
//  add a user function to the array of functions to check if
//  a CAN command message is sent. Note the functions must be of type 'void* func(void*, U8)',
//  so structs and casts are needed to get multiple params. The second parameter (U8) will be
//  sent by the module in the CAN command message
U8 add_custom_can_func(U8 command_id, void* (*func_ptr)(void*, U8), U8 init_state, void* param_ptr, void* return_val_ptr)
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
	new_cust_func->return_val_ptr = return_val_ptr;

	return SUCCESS;
}


// mod_custom_can_func_ptr
//  change the function pointer, parameter, and return value pointer
//  for the specified custom CAN function
U8 mod_custom_can_func_ptr(U8 command_id, void* (*func_ptr)(void*, U8), void* param_ptr, void* return_val_ptr)
{
	CUST_FUNC* this_cust_func;

	// make sure the ID is valid
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	this_cust_func = &(cust_funcs[command_id]);
	this_cust_func->func_ptr       = func_ptr;
	this_cust_func->param_ptr      = param_ptr;
	this_cust_func->return_val_ptr = return_val_ptr;

	return SUCCESS;
}


// mod_custom_can_func_state
//  change the state (enabled or disabled) of the specified custom CAN function
U8 mod_custom_can_func_state(U8 command_id, U8 state)
{
	CUST_FUNC* this_cust_func;

	// make sure the ID is valid
	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	this_cust_func = &(cust_funcs[command_id]);
	this_cust_func->func_enabled = !!state;

	return SUCCESS;
}


// tx_can_message
//  Takes in a CAN_MSG struct, modifies registers accordingly
static U8 tx_can_message(CAN_MSG message)
{
	// TODO

	return NOT_IMPLEMENTED;
}


// rx_can_message
//  TODO DOCS
static void rx_can_message()
{
	// TODO CAN message bus interrupt function
	// this will update all the global variables
	// or trigger the CAN functions if needed.
	// Use HAL_GetTick() to set last_rx

	CAN_MSG message;
	CAN_ID id;

	// TODO build the message from the registers on the STM32

	get_message_id(&id, &message);

	// error checking on the parameter requested
	if (id.parameter < 0 || id.parameter >= NUM_OF_PARAMETERS)
	{
		return;
	}

	// this switch will handle all of the different possible data types
	// that can be sent over CAN
	switch (parameter_data_types[id.parameter])
	{
	case REQ_PARAM:
		// TODO send the parameter requested
		break;

	case COMMAND:
		// TODO run the requested command
		break;

	case UNSIGNED8:
		// TODO
		break;

	case UNSIGNED16:
		// TODO
		break;

	case UNSIGNED32:
		// TODO
		break;

	case UNSIGNED64:
		// TODO
		break;

	case SIGNED8:
		// TODO
		break;

	case SIGNED16:
		// TODO
		break;

	case SIGNED32:
		// TODO
		break;

	case SIGNED64:
		// TODO
		break;

	case FLOATING:
		// TODO
		break;

	default:
		// TODO
		break;
	}
}


// build_can_id
//  this function will fill in the id of msg when called.
//  No error checking is preformed in this function besides masking
static void build_message_id(CAN_MSG* msg, CAN_ID* id)
{
	U32 temp;

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
	id->parameter = temp;

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


