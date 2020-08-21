/*
 * GopherCAN.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will have some auto generated sections

#include "GopherCAN.h"

U8 this_modual_id;

// all of the custom functions and an array to enable or disable
// each command ID corresponds to an index in the array
CUST_FUNC cust_funcs[NUM_OF_COMMANDS];


// ******** BEGIN AUTO GENERATED ********

// all of the global parameter structs
volatile REQ_PARAM_STRUCT req_param;
volatile CAN_COMMAND_STRUCT can_command;
volatile U16_CAN_STRUCT rpm;
volatile U8_CAN_STRUCT fan_current;

// this is the struct that will be used to refrence based on ID
static void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
	&req_param,    // ID 0
	&can_command,    // ID 2
	&rpm,    // ID 3
	&fan_current    // ID 4
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

	return SUCCESS;
}


// request_parameter
// 	This function will send out a CAN message requesting the parameter
//	given by the parameter ID from the module specified by the module ID
U8 request_parameter(U8 priority, U8 dest_module, U16 parameter)
{
	U8 ret_val;
	CAN_MSG message;

	if (dest_module < 0 || dest_module >= NUM_OF_MODUALS)
	{
		return BAD_MODULE_ID;
	}

	if (parameter < 0 || parameter >= NUM_OF_PARAMETERS)
	{
		return BAD_PARAMETER_ID;
	}

	// TODO build the message and send it

	return ret_val;
}


// send_can_command
//	This function will send a CAN message with a command specified
//	by command_id to the specified module
U8 send_can_command(U8 priority, U8 dest_module, U8 command_id, U8 command_parameter)
{
	U8 ret_val;
	CAN_MSG message;

	if (dest_module < 0 || dest_module >= NUM_OF_MODUALS)
	{
		return BAD_MODULE_ID;
	}

	if (command_id < 0 || command_id >= NUM_OF_COMMANDS)
	{
		return BAD_COMMAND_ID;
	}

	// TODO build the message and send it

	return ret_val;
}


// add_custum_can_func
//  add a user function to the array of functions to check if
//  a CAN command message is sent. Note the functions must be of type 'void* func(void*, U8)',
//  so structs and casts are needed to get multiple params. The second parameter (U8) will be
//  sent by the module in the CAN command message
U8 add_custom_can_func(U8 command_id, void* (*func_ptr)(void*), U8 init_state, void* param_ptr, void* return_val_ptr)
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
U8 mod_custom_can_func_ptr(U8 command_id, void* (*func_ptr)(void*), void* param_ptr, void* return_val_ptr)
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
	// probably some auto-gen stuff in here
	// Use HAL_GetTick() to set last_rx
}



