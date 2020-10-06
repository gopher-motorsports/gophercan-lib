/*
 * GopherCAN_example.c
 *
 *  Created on: Aug 19, 2020
 *      Author: calja
 */

#include "GopherCAN.h"
#include "stm32f0xx_hal.h"
#include "GopherCAN_example.h"


// each parameter used will need to be pulled from GopherCAN.c
extern U16_CAN_STRUCT rpm;
extern U8_CAN_STRUCT fan_current;

extern CAN_HandleTypeDef hcan;


// use this section to choose what module this should be (for testing 2 dev boards)
#define THIS_ACM
//#define THIS_PDM

#ifdef THIS_ACM
U8 this_module = ACM_ID;
U8 other_module = PDM_ID;
U8 priority = PRIO_LOW;
U16 param = FAN_CURRENT_ID;
#endif

#ifdef THIS_PDM
U8 this_module = PDM_ID;
U8 other_module = ACM_ID;
U8 priority = PRIO_HIGH;
U16 param = RPM_ID;
#endif


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
	// TODO add some tutorials on how to do this
	if (init_can(this_module))
	{
		// an error has occurred
	}

	// init HAL_GetTick()
	HAL_SetTickFreq(HAL_TICK_FREQ_DEFAULT);

	// enable updating the RPM and fan_current. Parameters that are not added to this list
	// will not be updated over can, even if they are requested
	rpm.update_enabled = TRUE;
	fan_current.update_enabled = TRUE;
	//fan_current.update_enabled = TRUE;

	// adding can_callback_function
	led_to_change = GPIO_PIN_5; // this is LD2 on the dev board

	if (add_custom_can_func(SET_LED_STATE, &change_led_state,
		TRUE, (void*)&led_to_change))
	{
		// an error has occurred
	}
}


// can_rx_loop
//  This loop will handle CAN RX software tasks. Should be called every 1ms
void can_rx_loop()
{
	// handle each RX message in the buffer
	if (service_can_rx_buffer())
	{
		// an error has occurred
	}
}


// can_hardware_handling
//  this loop handles pulling messages from the RX mailbox and putting messages into the TX mailbox
//  should be called on the 1ms loop with high priority
void can_hardware_handling()
{
	service_can_hardware();
}


// backround_loop
//  Example of a loop that request parameters that need to be consistently up to date for
//  other parts of the program, can be called as often as needed (as long as the CAN bus is not overwhelmed)
void background_loop()
{
	// always request a param from the other module at 1ms intervals
	if (request_parameter(priority, other_module, param))
	{
		// an error has occurred
	}

	// adding a bunch of can commands to stress the bus. These should return COMMAND_NOT_ENABLED error on the CAN bus
	/*
	send_can_command(PRIO_LOW, other_module, CUST_COMMAND_2, 0);
	send_can_command(PRIO_LOW, other_module, CUST_COMMAND_2, 1);
	send_can_command(PRIO_HIGH, other_module, CUST_COMMAND_2, 0);
	send_can_command(PRIO_HIGH, other_module, CUST_COMMAND_2, 1);
	*/

	// update this modules parameter to show a change on the can bus
#ifdef THIS_ACM
	rpm.data++;
#endif

#ifdef THIS_PDM
	fan_current.data++;
#endif
}


// main_loop
//  another loop. This includes logic for a CAN command and conditional parameter requests
void main_loop()
{
	U8 button_state;

#ifdef THIS_ACM
	U8 foo;

	// Example accessing updating parameters that are requested in another loop
	if (rpm.data <= MIN_ON_RPM)
	{
		// if the engine is off
	}

	// Example of requesting a parameter in runtime. This example
	// uses time, but other logic could be used as well
	if (HAL_GetTick() - last_fan_current_rx >= FAN_CURRENT_UPDATE_TIME)
	{
		// don't send another data request if the request is already pending
		// A timeout may also be worth including just in case something goes wrong
		if (fan_current.pending_response == FALSE
				|| HAL_GetTick() - last_fan_current_rx >= FAN_CURRENT_UPDATE_TIMEOUT)
		{
			if (request_parameter(PRIO_HIGH, other_module, FAN_CURRENT_ID))
			{
				// error handling
			}
		}

		// use the parameter data for something
		foo = fan_current.data;

		// update the last time the fan_current was received. fan_current.last_rx cannot be
		// used directly in this case because the code inside the if statement may not be run
		// if it updates at a bad time
		last_fan_current_rx = fan_current.last_rx;
	}
#endif

	// If the button is pressed send a can command to another to change the LED state
	// To on or off depending on the button. Make sure to disable any heartbeat before
	// trying this, they may conflict
	button_state = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	if (send_can_command(PRIO_HIGH, other_module, SET_LED_STATE, button_state))
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
