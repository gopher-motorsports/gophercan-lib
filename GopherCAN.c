/*
 * GopherCAN.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will have some auto generated sections

#include "GopherCAN.h"

// static function prototypes
static U8   tx_can_message(CAN_MSG* message);
static void rx_can_message();
static void build_message_id(CAN_MSG* msg, CAN_ID* id);
static void get_message_id(CAN_ID* id, CAN_MSG* message);
static U8   send_error_message(CAN_ID* id, U8 error_id);


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

	return tx_can_message(&message);
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

	return tx_can_message(&message);
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
static U8 tx_can_message(CAN_MSG* message)
{
	// TODO

	return NOT_IMPLEMENTED;
}


// rx_can_message
//  CAN message bus interrupt function this will update all
//  the global variables or trigger the CAN functions if needed
static void rx_can_message()
{
	CAN_MSG message;
	CAN_ID id;
	U64 recieved_data = 0;
	U8 c;
	void* data_struct = 0;

	// TODO build the message from the registers on the STM32

	get_message_id(&id, &message);

	// TODO what to do if there is an error in the message

	// error checking on the parameter requested
	if (id.parameter < 0 || id.parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(&id, ID_NOT_FOUND);

		return;
	}
	
	// get the associated data struct and set last_rx
	data_struct = all_parameter_structs[id.parameter];
	*((U32*)data_struct) = HAL_GetTicks();

	// build the data U64 (big endian)
	for (c = (message.dlc - 1); c >= 0; c--)
	{
		recieved_data |= message.data[c] << c;
	}

	// request parameter and commands are handled differently
	if (parameter_data_types[id.parameter] == REQ_PARAM)
	{
		U16 parameter_requested;
		CAN_ID return_id;
		CAN_MSG return_message;
		U64 return_data;

		if (message.dlc != REQ_PARAM_SIZE)
		{
			send_error_message(&id, SIZE_ERROR);

			return;
		}

		// find what the parameter is from the data
		parameter_requested = (message.data[0] << BITS_IN_BYTE) | message.data[1];
		req_param.parameter_id = parameter_requested;

		if (parameter_requested < RPM_ID || parameter_requested >= NUM_OF_PARAMETERS)
		{
			send_error_message(&id, ID_NOT_FOUND);
			
			return;
		}

		// build the return message ID
		return_id.priority = id.priority;
		return_id.dest_module = id.source_module;
		return_id.source_module = this_modual_id;
		return_id.error = FALSE;
		return_id.parameter = parameter_requested;

		build_message_id(&return_message, &return_id);

		// get the value of the data on this module and build the CAN message
		if (parameter_data_types[parameter_requested] == UNSIGNED8
			|| parameter_data_types[parameter_requested] == SIGNED8)
		{
			return_data = *((U8*)(all_parameter_structs[parameter_requested] + DATA_POS));
			return_message.dlc = sizeof(U8);
		}

		else if (parameter_data_types[parameter_requested] == UNSIGNED16
			|| parameter_data_types[parameter_requested] == SIGNED16)
		{
			return_data = *((U16*)(all_parameter_structs[parameter_requested] + DATA_POS));
			return_message.dlc = sizeof(U16);
		}

		else if (parameter_data_types[parameter_requested] == UNSIGNED32
			|| parameter_data_types[parameter_requested] == SIGNED32)
		{
			return_data = *((U32*)(all_parameter_structs[parameter_requested] + DATA_POS));
			return_message.dlc = sizeof(U32);
		}

		else if (parameter_data_types[parameter_requested] == UNSIGNED64
			|| parameter_data_types[parameter_requested] == SIGNED64)
		{
			return_data = *((U64*)(all_parameter_structs[parameter_requested] + DATA_POS));
			return_message.dlc = sizeof(U64);
		}

		// build the data in the message (big endian)
		for (c = return_message.dlc - 1; c >= 0; c--)
		{
			return_message.data[c] = (U8)(return_data >> c);
		}

		// send the built CAN message
		tx_can_message(&return_message);

		return;
	}

	if (parameter_data_types[id.parameter] == COMMAND)
	{
		// TODO

		return;
	}

	// Check the update_enabled flag
	if (!((U8*)(data_struct + UPDATE_ENABLED_POS)))
	{
		send_error_message(&id, PARAM_NOT_ENABLED);
		
		return;
	}

	// Switch the pending_response flag
	*((U8*)(data_struct + PENDING_RESPONSE_POS)) = FALSE;

	// this switch will handle all of the different possible data types
	// that can be sent over CAN
	switch (parameter_data_types[id.parameter])
	{
	case UNSIGNED8:
		*((U8*)(data_struct + DATA_POS)) = (U8)recieved_data;
		return;

	case UNSIGNED16:
		*((U16*)(data_struct + DATA_POS)) = (U16)recieved_data;
		return;

	case UNSIGNED32:
		*((U32*)(data_struct + DATA_POS)) = (U32)recieved_data;
		return;

	case UNSIGNED64:
		*((U64*)(data_struct + DATA_POS)) = (U64)recieved_data;
		return;

	case SIGNED8:
		*((S8*)(data_struct + DATA_POS)) = (S8)recieved_data;
		break;

	case SIGNED16:
		*((S16*)(data_struct + DATA_POS)) = (S16)recieved_data;
		break;

	case SIGNED32:
		*((S32*)(data_struct + DATA_POS)) = (S32)recieved_data;
		break;

	case SIGNED64:
		*((S64*)(data_struct + DATA_POS)) = (S64)recieved_data;
		break;

	case FLOATING:
		*((float*)(data_struct + DATA_POS)) = (float)recieved_data;
		break;

	default:
		*((U8*)(data_struct + DATA_POS)) = (U8)recieved_data;
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


// send_error_message
//  Sends a return message to the original sender with the ID specified
static U8 send_error_message(CAN_ID* id, U8 error_id)
{
	CAN_MSG message;
	CAN_ID id;

	// TODO

	return tx_can_message(&message);
}

// end GopherCAN.c