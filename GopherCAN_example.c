/*
 * GopherCAN_example.c
 *
 *  Created on: Aug 19, 2020
 *      Author: calja
 */

#include "GopherCAN.h"
#include "GopherCAN_example.h"


// each parameter used will need to be pulled from GopherCAN.c
extern U16_CAN_STRUCT rpm;
extern U8_CAN_STRUCT fan_current;
extern U8_CAN_STRUCT u8_tester;
extern U16_CAN_STRUCT u16_tester;
extern U32_CAN_STRUCT u32_tester;
extern U64_CAN_STRUCT u64_tester;
extern S8_CAN_STRUCT s8_tester;
extern S16_CAN_STRUCT s16_tester;
extern S32_CAN_STRUCT s32_tester;
extern S64_CAN_STRUCT s64_tester;
extern FLOAT_CAN_STRUCT float_tester;

// the HAL_CAN struct. This example only works for a single CAN bus
CAN_HandleTypeDef* example_hcan;


// use this section to choose what module this should be (for testing 2 dev boards)
// and what functionality should be enabled
//#define F7XX_EXAMPLE                                                          // (F0xx otherwise)
#define THIS_ACM
//#define THIS_PDM
//#define ENABLE_LOGIC_REQ
#define ENABLE_BUTTON_LED

#ifdef THIS_ACM
U8 this_module = ACM_ID;
U8 other_module = PDM_ID;
U8 priority = PRIO_LOW;
U16 param = FAN_CURRENT_ID;

U32 last_u16_req = 0;
U32 last_u64_req = 0;
U32 last_s16_req = 0;
U32 last_s64_req = 0;
#endif

#ifdef THIS_PDM
U8 this_module = PDM_ID;
U8 other_module = ACM_ID;
U8 priority = PRIO_HIGH;
U16 param = RPM_ID;

U32 last_u8_req = 0;
U32 last_u32_req = 0;
U32 last_s8_req = 0;
U32 last_s32_req = 0;
U32 last_float_req = 0;
#endif


// some global variables for examples
U16 led_to_change;
U32 last_fan_current_req = 0;
U8 last_button_state = 0;


// the CAN callback function used in this example
void change_led_state(U8 sender, void* parameter, U8 remote_param, U8 UNUSED1, U8 UNUSED2, U8 UNUSED3);

// init
//  What needs to happen on startup in order to run GopherCAN
void init(CAN_HandleTypeDef* hcan_ptr)
{
	example_hcan = hcan_ptr;

	// initialize CAN
	// NOTE: CAN will also need to be added in CubeMX and code must be generated
	// Check the STM_CAN repo for the file "F0xx CAN Config Settings.pptx" for the correct settings
	if (init_can(example_hcan, this_module))
	{
		// an error has occurred, stay here
		while (1);
	}

	// enable updating the RPM and fan_current. Parameters that are not added to this list
	// will not be updated over CAN, even if they are requested
	rpm.update_enabled = TRUE;
	fan_current.update_enabled = TRUE;

	// enable the tester variables
	u8_tester.update_enabled = TRUE;
	u16_tester.update_enabled = TRUE;
	u32_tester.update_enabled = TRUE;
	u64_tester.update_enabled = TRUE;
	s8_tester.update_enabled = TRUE;
	s16_tester.update_enabled = TRUE;
	s32_tester.update_enabled = TRUE;
	s64_tester.update_enabled = TRUE;
	float_tester.update_enabled = TRUE;

	// adding can_callback_function
#ifndef F7XX_EXAMPLE
	led_to_change = GPIO_PIN_5; // this is LD2 on the dev board
#else
	led_to_change = GPIO_PIN_0; // this is LD1 on the dev board
#endif

	if (add_custom_can_func(SET_LED_STATE, &change_led_state,
		TRUE, (void*)&led_to_change))
	{
		// an error has occurred
	}
}


// can_rx_loop
//  This loop will handle CAN RX software tasks. Should be called every 1ms
//  or as often as received messages should be handled
void can_rx_loop()
{
	// handle each RX message in the buffer
	if (service_can_rx_buffer())
	{
		// an error has occurred
	}
}


// can_hardware_handling
//  this loop handles putting messages into the TX mailbox
//  should be called on the 1ms loop with high priority
//  or as often as messages should be sent. Note that service_can_tx_hardware
//  returns when there is no mailbox slots open, not when there are no more messages to send.
void can_hardware_handling()
{
	service_can_tx_hardware(example_hcan);
}


// backround_loop
//  Example of a loop that request parameters that need to be consistently up to date for
//  other parts of the program, can be called as often as needed (as long as the CAN bus is not overwhelmed)
void background_loop()
{
	// always request a param from the other module at 1ms intervals
	// this is not the best idea as this can cause a lot of bus traffic
	// logic-based requests are better, but this still works
	if (request_parameter(PRIO_LOW, other_module, RPM_ID))
	{
		// an error has occurred
	}

	// update this modules parameter to show a change on the can bus
	rpm.data++;
}


// main_loop
//  another loop. This includes logic for a CAN command and conditional parameter requests
void main_loop()
{
	U8 button_state;

#ifdef ENABLE_LOGIC_REQ
	U8 foo;

	// Example accessing updating parameters that are requested in another loop
	if (rpm.data <= MIN_ON_RPM)
	{
		// if the engine is off
	}

	// Example of requesting a parameter in runtime. This example
	// uses time, but other logic could be used as well
	if (HAL_GetTick() - fan_current.last_rx >= FAN_CURRENT_UPDATE_TIME)
	{
		// don't send another data request if the request is already pending
		// A timeout may also be worth including just in case something goes wrong
		if (fan_current.pending_response == FALSE
				|| HAL_GetTick() - last_fan_current_req >= PARAM_UPDATE_TIMEOUT)
		{
			if (request_parameter(PRIO_HIGH, other_module, FAN_CURRENT_ID))
			{
				// error handling
			}
		}

		// use the parameter data for something
		foo = fan_current.data;
		foo++;

		// update the last time the fan_current was received. fan_current.last_rx cannot be
		// used directly in this case to prevent spamming requests after the timeout is reached
		last_fan_current_req = fan_current.last_rx;
	}

	fan_current.data++;
#endif

#ifdef ENABLE_BUTTON_LED
	// If the button is pressed send a can command to another to change the LED state
	// To on or off depending on the button. Make sure to disable any heartbeat before
	// trying this, they may conflict
	button_state = !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	// for some reason the button is the opposite on the F7
#ifdef F7XX_EXAMPLE
	button_state = !button_state;
#endif

	// this is to prevent spamming CAN messages
	if (button_state != last_button_state)
	{
		last_button_state = button_state;

		if (send_can_command(PRIO_HIGH, other_module, SET_LED_STATE,
				button_state, button_state, button_state, button_state))
		{
			// error sending command
		}
	}
#endif
}


// can_callback_function example

// change_led_state
//  a custom function that will change the state of the LED specified
//  by parameter to remote_param. In this case parameter is a U16*, but
//  any data type can be pointed to, as long as it is configured and casted
//  correctly
void change_led_state(U8 sender, void* parameter, U8 remote_param, U8 UNUSED1, U8 UNUSED2, U8 UNUSED3)
{
	// this function will set the LED to high or low, depending on remote_param
	// the LED to change is dependent on the parameter stored on this module (*((U16*)parameter))
	U16 gpio_pin = *((U16*)parameter);

#ifndef F7XX_EXAMPLE
	HAL_GPIO_WritePin(GPIOA, gpio_pin, !!remote_param);
#else
	HAL_GPIO_WritePin(GPIOB, gpio_pin, !!remote_param);
#endif

	return;
}


// testing_loop
//  this is used for testing things. Everything is better explained in other functions
void testing_loop()
{
	U32 current_tick = HAL_GetTick();

	/*
	// adding a bunch of can commands to stress the bus. These should return COMMAND_NOT_ENABLED error on the CAN bus
	send_can_command(PRIO_LOW, other_module, CUST_COMMAND_2, 0);
	send_can_command(PRIO_LOW, other_module, CUST_COMMAND_2, 1);
	send_can_command(PRIO_HIGH, other_module, CUST_COMMAND_2, 0);
	send_can_command(PRIO_HIGH, other_module, CUST_COMMAND_2, 1);
	*/

	// all data type testing
	// ACM has U8, U32, S8, S32, floating (asks for the others)
	// PDM has U16, U64, S16, S64 (asks for the others)
#ifdef THIS_ACM
	u8_tester.data += 1;
	u32_tester.data += 4;
	s8_tester.data -= 1;
	s32_tester.data -= 4;
	float_tester.data += 0.1;

	if ((u16_tester.pending_response == FALSE
			&& current_tick - u16_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_u16_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, PDM_ID, U16_TESTER_ID);

		last_u16_req = current_tick;
	}

	if ((u64_tester.pending_response == FALSE
			&& current_tick - u64_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_u64_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, PDM_ID, U64_TESTER_ID);

		last_u64_req = current_tick;
	}

	if ((s16_tester.pending_response == FALSE
			&& current_tick - s16_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_s16_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, PDM_ID, S16_TESTER_ID);

		last_s16_req = current_tick;
	}

	if ((s64_tester.pending_response == FALSE
			&& current_tick - s64_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_s64_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, PDM_ID, S64_TESTER_ID);

		last_s64_req = current_tick;
	}

#endif

#ifdef THIS_PDM
	u16_tester.data += 2;
	u64_tester.data += 8;
	s16_tester.data -= 2;
	s64_tester.data -= 8;

	if ((u8_tester.pending_response == FALSE
			&& current_tick - u8_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_u8_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, ACM_ID, U8_TESTER_ID);

		last_u8_req = current_tick;
	}

	if ((u32_tester.pending_response == FALSE
			&& current_tick - u32_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_u32_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, ACM_ID, U32_TESTER_ID);

		last_u32_req = current_tick;
	}

	if ((s8_tester.pending_response == FALSE
			&& current_tick - s8_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_s8_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, ACM_ID, S8_TESTER_ID);

		last_s8_req = current_tick;
	}

	if ((s32_tester.pending_response == FALSE
			&& current_tick - s32_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_s32_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, ACM_ID, S32_TESTER_ID);

		last_s32_req = current_tick;
	}

	if ((float_tester.pending_response == FALSE
			&& current_tick - float_tester.last_rx >= MIN_PARAM_UPDATE_TIME)
			|| current_tick - last_float_req >= PARAM_UPDATE_TIMEOUT)
	{
		request_parameter(PRIO_HIGH, ACM_ID, FLOAT_TESTER_ID);

		last_float_req = current_tick;
	}
#endif
}



// end of GopherCAN_example.c
