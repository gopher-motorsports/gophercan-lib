/*
 * GopherCAN_example.c
 *
 *  Created on: Aug 19, 2020
 *      Author: calja
 */

#include "GopherCAN.h"
#include "GopherCAN_router_example.h"


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
CAN_HandleTypeDef* example_hcan0;
CAN_HandleTypeDef* example_hcan1;

//#define ENABLE_LOGIC_REQ
#define ENABLE_BUTTON_LED

U8 this_module = DLM_ID;
U8 other_module = ALL_MODULES_ID;
U8 priority = PRIO_HIGH;


// some global variables for examples
U16 led_to_change;
U32 last_fan_current_req = 0;
U8 last_button_state = 0;


// the CAN callback function used in this example
void router_change_led_state(void* parameter, U8 remote_param);

// router_init
//  What needs to happen on startup in order to run GopherCAN
void router_init(CAN_HandleTypeDef* hcan_ptr0, CAN_HandleTypeDef* hcan_ptr1)
{
	example_hcan0 = hcan_ptr0;
	example_hcan1 = hcan_ptr1;

	// initialize CAN
	// NOTE: CAN will also need to be added in CubeMX and code must be generated
	// Check the STM_CAN repo for the file "F0xx CAN Config Settings.pptx" for the correct settings
	if (init_can(example_hcan0, this_module)
			|| init_can(example_hcan1, this_module))
	{
		// an error has occurred, stay here
		while (1);
	}

	// Declare which bus is which using define_can_bus
	define_can_bus(example_hcan1, GCAN0, 0);
	define_can_bus(example_hcan0, GCAN1, 1);


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

	led_to_change = GPIO_PIN_0; // this is LD1 on the dev board

	if (add_custom_can_func(SET_LED_STATE, &router_change_led_state,
		TRUE, (void*)&led_to_change))
	{
		// an error has occurred
	}
}


// router_can_rx_loop
//  This loop will handle CAN RX software tasks. Should be called every 1ms
//  or as often as received messages should be handled
void router_can_rx_loop()
{
	// This is needed to account for a case where the RX buffer fills up, as the ISR is only
	//  triggered directly on reciving the message
	//service_can_rx_hardware(example_hcan0, CAN_RX_FIFO0);
	//service_can_rx_hardware(example_hcan0, CAN_RX_FIFO1);
	service_can_rx_hardware(example_hcan1, CAN_RX_FIFO0);
	service_can_rx_hardware(example_hcan1, CAN_RX_FIFO1);

	// handle each RX message in the buffer
	if (service_can_rx_buffer())
	{
		// an error has occurred
	}
}


// router_can_hardware_handling
//  this loop handles putting messages into the TX mailbox
//  should be called on the 1ms loop with high priority
//  or as often as messages should be sent. Note that service_can_tx_hardware
//  returns when there is no mailbox slots open, not when there are no more messages to send.
void router_can_hardware_handling()
{
	service_can_tx_hardware(example_hcan0);
	service_can_tx_hardware(example_hcan1);
}


// router_main_loop
//  another loop. This includes logic for a CAN command and conditional parameter requests
void router_main_loop()
{
	U8 button_state;

#ifdef ENABLE_BUTTON_LED
	// If the button is pressed send a can command to another to change the LED state
	// To on or off depending on the button. Make sure to disable any heartbeat before
	// trying this, they may conflict
	button_state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	// this is to prevent spamming CAN messages
	if (button_state != last_button_state)
	{
		last_button_state = button_state;

		if (send_can_command(PRIO_HIGH, other_module, SET_LED_STATE, button_state))
		{
			// error sending command
		}
	}
#endif
}


// can_callback_function example

// router_change_led_state
//  a custom function that will change the state of the LED specified
//  by parameter to remote_param. In this case parameter is a U16*, but
//  any data type can be pointed to, as long as it is configured and casted
//  correctly
void router_change_led_state(void* parameter, U8 remote_param)
{
	// this function will set the LED to high or low, depending on remote_param
	// the LED to change is dependent on the parameter stored on this module (*((U16*)parameter))
	U16 gpio_pin = *((U16*)parameter);

	HAL_GPIO_WritePin(GPIOB, gpio_pin, !!remote_param);

	return;
}


// router_testing_loop
//  this is used for testing things. Everything is better explained in other functions
void router_testing_loop()
{
	U32 current_tick = HAL_GetTick();

	// update all of the variables
	u8_tester.data += 1;
	u16_tester.data += 2;
	u32_tester.data += 4;
	u64_tester.data += 8;
	s8_tester.data -= 1;
	s16_tester.data -= 2;
	s32_tester.data -= 4;
	s64_tester.data -= 8;
	float_tester.data += 0.1;
}



// end of GopherCAN_example.c
