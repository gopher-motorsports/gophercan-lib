/*
 * GopherCAN_example.c
 *
 *  Created on: Aug 19, 2020
 *      Author: calja
 */

#include "GopherStandards.h"
#include "GopherCAN.h"

extern RPM_STRUCT rpm;
extern FAN_CURRENT_STRUCT fan_current;

U8 can_func_param;
U8 can_func_ret_val;

U8 last_fan_current;


// init
//  examples of things to be called on startup
void init()
{
	// initialize CAN
	if (init_can(ACM_ID))
	{
		// an error has occurred
	}

	// enable updating the RPM from the EDL
	rpm.update_enabled = TRUE;

	// TODO define target for what each module needs (conditional defines)

	// add can_callback_function
	if (add_custom_can_func(INC_VARIABLE, &inc_variable,
			ENABLED, (void*)&can_func_param, (void*)&can_func_ret_val))
	{
		// an error has occurred
	}
}


// backround_loop
//  a lower priority loop
void background_loop()
{
	// update EDL variables. This is subject to change
	if (update_general_target_can())
	{
		// an error has occurred
	}
}


// main_loop
//  a higher priority logic loop
void main_loop()
{
	// Example accessing updating parameters
	if (rpm.data == 0)
	{
		// if the engine is off
	}

	// request a parameter. The associated struct will be updated
	if (fan_current.pending_response == FALSE)
	{
		request_parameter(PRIO_HIGH, PDM_ID, FAN_CURRENT_ID);
	}

	// access that parameter when the update has been received.
	// This logic can be done in many different ways
	if (last_fan_current != fan_current.data)
	{
		last_fan_current = fan_current.data;
	}

	// send a can command to another module
	if (send_can_command(PRIO_LOW, PDM_ID, TURN_FAN_OFF))
	{
		// error sending command
	}
}


// can_callback_function
//  a custom function that will return the input + 1
//  note void pointers are passed in and returned
void* inc_variable(void* parameter)
{
	U8 i, j;

	i = *(U8*)parameter;
	j = i + 1;

	return (void*)&j;
}


