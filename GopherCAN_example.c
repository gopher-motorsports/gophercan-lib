/*
 * GopherCAN_example.c
 *
 *  Created on: Aug 19, 2020
 *      Author: calja
 */

#include "GopherCAN.h"
#include "stm32f0xx_hal.h"


// each parameter used will need to be pulled from GopherCAN.c
extern U16_CAN_STRUCT rpm;
extern U8_CAN_STRUCT fan_current;


// some defines used for examples
#define MIN_ON_RPM 100
#define FAN_CURRENT_UPDATE_TIME 1000

// some global variables for examples
U16 led_to_change;
U32 last_fan_current_rx = 0;

// the CAN callback function used in this example
void change_led_state(void* parameter, U8 remote_param);

// init
//  What needs to happen on startup in order to run GopherCAN
void init()
{
	// initialize CAN
	// NOTE: CAN will also need to be added in CubeMX and code must be generated
	// TODO add a link of a good video showing how to set up CAN (once done successfully)
	if (init_can(ACM_ID))
	{
		// an error has occurred
	}

	// enable updating the RPM and fan_current. Parameters that are not added to this list
	// will not be updated over can, even if they are requested
	rpm.update_enabled = TRUE;
	fan_current.update_enabled = TRUE;

	// adding can_callback_function
	led_to_change = 0; // TODO what gpio pin

	if (add_custom_can_func(SET_LED_STATE, &change_led_state,
		TRUE, (void*)&led_to_change))
	{
		// an error has occurred
	}
}


// backround_loop
//  this loop should request parameters that need to be consistently up to date for
//  other parts of the program
void background_loop()
{
	// always request rpm from the PDM at 1ms intervals
	if (request_parameter(PRIO_HIGH, PDM_ID, RPM_ID))
	{
		// an error has occurred
	}
}


// main_loop
//  a higher priority logic loop
void main_loop()
{
	U8 button_1_state;
	U8 button_2_state;
	U8 foo;

	// Example accessing updating parameters that are requested in another loop
	if (rpm.data <= MIN_ON_RPM)
	{
		// if the engine is off
	}

	// Example of requesting a parameter in runtime. This example
	// uses time, but logic could be used as well
	if (last_fan_current_rx - HAL_GetTick() >= FAN_CURRENT_UPDATE_TIME)
	{
		// don't send another data request if the request is already pending
		// A timeout may also be worth including just in case something goes wrong
		if (fan_current.pending_response == FALSE)
		{
			if (request_parameter(PRIO_HIGH, PDM_ID, FAN_CURRENT_ID))
			{
				// error handling
			}
		}

		// use the fan current for something
		foo = fan_current.data;

		// update the last time the fan current was received. fan_current.last_rx cannot be
		// used directly in this case because the code inside the if statement may not be run
		// if it updates at a bad time
		last_fan_current_rx = fan_current.last_rx;
	}

	// If the button is pressed send a can command to another to change the LED state
	// To on or off depending on the second button

	// TODO get button 1 and 2 state

	if (button_1_state
		&& send_can_command(PRIO_LOW, PDM_ID, SET_LED_STATE, button_2_state))
	{
		// error sending command
	}
}

// can_callback_function example

// change_led_state
//  a custom function that will change the state of the LED specified
//  by parameter to remote_param. In this case parameter is a U16*, but
//  any data type can be pointed to, as long as it is configured and casted
//  correctly
void change_led_state(void* parameter, U8 remote_param)
{
	// this function will set the LED to high or low, depending on remote_param
	// the LED to change is dependent on the parameter stored on this module (*((U16*)parameter))
	U16 gpio_pin = *((U16*)parameter);

	HAL_GPIO_WritePin(GPIOA, gpio_pin, !!remote_param);

	return;
}


// end of GopherCAN_example.c
