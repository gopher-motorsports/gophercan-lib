/*
 * GopherCAN.c
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will have some auto generated sections

#include "GopherCAN.h"
#include "GopherCAN_ring_buffer.h"

// static function prototypes
static S8   init_filters(CAN_HandleTypeDef* hcan);
static S8   tx_can_message(CAN_MSG* message);
static S8   parameter_requested(CAN_MSG* message, CAN_ID* id);
static S8   run_can_command(CAN_MSG* message, CAN_ID* id);
static void build_message_id(CAN_MSG* msg, CAN_ID* id);
static void get_message_id(CAN_ID* id, CAN_MSG* message);
static S8   send_error_message(CAN_ID* id, U8 error_id);
static S8   tx_can_message(CAN_MSG* message);
static S8   service_can_rx_message(CAN_MSG* message);
static void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

// all of the custom functions and an array to enable or disable
// each command ID corresponds to an index in the array
CUST_FUNC cust_funcs[NUM_OF_COMMANDS];

// what module this is configured to be
U8 this_module_id;

// a struct to store the last error type message received
ERROR_MSG last_error;

// stores the last hcan error code
U32 hcan_error = HAL_CAN_ERROR_NONE;

// buffers to store RX and TX messages
CAN_MSG_RING_BUFFER rx_buffer;
CAN_MSG rx_buffer_mem[RX_BUFFER_SIZE];
CAN_MSG_RING_BUFFER tx_buffer;
CAN_MSG tx_buffer_mem[TX_BUFFER_SIZE];

// if there are more than one CAN busses, we need more TX buffers
// the HAL_CAN handle pointer is for checking which bus that handle is referring to
// if the pointer is not found, the program will default to bus 0
#if NUM_OF_BUSSES > 1
CAN_MSG_RING_BUFFER tx_buffer_1;
CAN_MSG tx_buffer_mem_1[TX_BUFFER_SIZE];
CAN_HandleTypeDef* hcan_1;
U8 can_1_gopher_id;
#endif
#if NUM_OF_BUSSES > 2
CAN_MSG_RING_BUFFER tx_buffer_2;
CAN_MSG tx_buffer_mem_2[TX_BUFFER_SIZE];
CAN_HandleTypeDef* hcan_2;
U8 can_2_gopher_id;
#endif

// ******** BEGIN AUTO GENERATED ********

// all of the global parameter structs
CAN_COMMAND_STRUCT can_command;
U16_CAN_STRUCT rpm;
U8_CAN_STRUCT fan_current;
U8_CAN_STRUCT u8_tester;
U16_CAN_STRUCT u16_tester;
U32_CAN_STRUCT u32_tester;
U64_CAN_STRUCT u64_tester;
S8_CAN_STRUCT s8_tester;
S16_CAN_STRUCT s16_tester;
S32_CAN_STRUCT s32_tester;
S64_CAN_STRUCT s64_tester;
FLOAT_CAN_STRUCT float_tester;

// this is the struct that will be used to reference based on ID
static void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
	&can_command,    // ID 0
	&rpm,    // ID 1
	&fan_current,    // ID 2
	&u8_tester,    // ID 3
	&u16_tester,    // ID 4
	&u32_tester,    // ID 5
	&u64_tester,    // ID 6
	&s8_tester,    // ID 7
	&s16_tester,    // ID 8
	&s32_tester,    // ID 9
	&s64_tester,    // ID 10
	&float_tester    // ID 11
};

// this stores the data_type for each parameter, referenced by ID
static U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
	COMMAND,
	UNSIGNED16,
	UNSIGNED8,
	UNSIGNED8,
	UNSIGNED16,
	UNSIGNED32,
	UNSIGNED64,
	SIGNED8,
	SIGNED16,
	SIGNED32,
	SIGNED64,
	FLOATING
};

// if there are multiple busses, this shows which bus they are on
#if NUM_OF_BUSSES > 1
static U8 module_bus_number[NUM_OF_MODULES] =
{
	ALL_BUSSES,
	GCAN0,
	GCAN0,
	GCAN0,
	GCAN1,
	GCAN1,
	GCAN2
};
#endif

// ******** END AUTO GENERATED ********


// init_can
// 	This function will set up the CAN registers with the inputed module_id
//	as a filter. All parameters that should be enabled should be set after
//  calling this function
// params:
//  U8 module_id: what module this is (ex. PDM_ID, ACM_ID)
// returns:
//  error codes specified in GopherCAN.h
S8 init_can(CAN_HandleTypeDef* hcan, U8 module_id)
{
	U8 c;
	CAN_INFO_STRUCT* data_struct;

	// set the current module
	this_module_id = module_id;

	// init HAL_GetTick()
	HAL_SetTickFreq(HAL_TICK_FREQ_DEFAULT);

	// setup the two buffers
	init_buffer(&rx_buffer, rx_buffer_mem, RX_BUFFER_SIZE);
	init_buffer(&tx_buffer, tx_buffer_mem, TX_BUFFER_SIZE);

	// if there are more CAN busses, set up the additional TX buffers
#if NUM_OF_BUSSES > 1
	init_buffer(&tx_buffer_1, tx_buffer_mem_1, TX_BUFFER_SIZE);
#endif
#if NUM_OF_BUSSES > 2
	init_buffer(&tx_buffer_2, tx_buffer_mem_2, TX_BUFFER_SIZE);
#endif

	// disable each parameter until the user manually enables them
	for (c = CAN_COMMAND_ID + 1; c < NUM_OF_PARAMETERS; c++)
	{
		data_struct = (CAN_INFO_STRUCT*)(all_parameter_structs[c]);

		data_struct->last_rx = 0;
		data_struct->update_enabled = FALSE;
		data_struct->pending_response = FALSE;
	}

	// set each function pointer to the do_nothing() function
	for (c = 0; c < NUM_OF_COMMANDS; c++)
	{
		cust_funcs[c].func_ptr = &do_nothing;
		cust_funcs[c].func_enabled = FALSE;
		cust_funcs[c].param_ptr = NULL;

	}

	if (init_filters(hcan))
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

	// start can!
	if (HAL_CAN_Start(hcan) != HAL_OK)
	{
		return CAN_START_FAILED;
	}

	return CAN_SUCCESS;
}


// init_filters
//  function called within init() that sets up all of the filters
static S8 init_filters(CAN_HandleTypeDef* hcan)
{
	CAN_FilterTypeDef filterConfig;
	U32 filt_id_low;
	U32 filt_id_high;
	U32 filt_mask_high;
	U32 filt_mask_low;

	// Define the filter values based on this_module_id
	// High and low id are the same because the id exclusively must be the module id
	filt_id_low = this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_id_high = this_module_id << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_mask_low = DEST_MASK;
	filt_mask_high = DEST_MASK;

	// Set the the parameters on the filter struct (FIFO0)
	filterConfig.FilterBank = 0;                                      // Modify bank 0 (of 13)
	filterConfig.FilterActivation = CAN_FILTER_ENABLE;                // enable the filter
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;             // use FIFO0
	filterConfig.FilterMode = CAN_FILTERMODE_IDMASK;                  // Use mask mode to filter
	filterConfig.FilterScale = CAN_FILTERSCALE_32BIT;                 // 32 bit mask
	filterConfig.FilterIdLow = filt_id_low;                           // Low bound of accepted values
	filterConfig.FilterIdHigh = filt_id_high;                         // High bound of accepted values
	filterConfig.FilterMaskIdLow = filt_mask_low;                     // Which bits matter when filtering (high)
	filterConfig.FilterMaskIdHigh = filt_mask_high;                   // Which bits matter when filtering (low)

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// Set the the parameters on the filter struct (FIFO1)
	// all other parameters are the same as FIFO0
	filterConfig.FilterBank = 1;                                      // Modify bank 1 (of 13)
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;             // use FIFO1

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// set the filters for the general module ID
	filt_id_low = ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_id_high = ALL_MODULES_ID << (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
	filt_mask_low = DEST_MASK;
	filt_mask_high = DEST_MASK;

	// Set the the parameters on the filter struct (FIFO0)
	filterConfig.FilterBank = 2;                                      // Modify bank 2 (of 13)
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;             // use FIFO0
	filterConfig.FilterIdLow = filt_id_low;                           // Low bound of accepted values
	filterConfig.FilterIdHigh = filt_id_high;                         // High bound of accepted values
	filterConfig.FilterMaskIdLow = filt_mask_low;                     // Which bits matter when filtering (high)
	filterConfig.FilterMaskIdHigh = filt_mask_high;                   // Which bits matter when filtering (low)

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	// Set the the parameters on the filter struct (FIFO1)
	// all other parameters are the same as FIFO0
	filterConfig.FilterBank = 3;                                      // Modify bank 3 (of 13)
	filterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO1;             // use FIFO1

	if (HAL_CAN_ConfigFilter(hcan, &filterConfig) != HAL_OK)
	{
		return FILTER_SET_FAILED;
	}

	return CAN_SUCCESS;
}


// define_can_bus
//  TODO DOCS
void define_can_bus(CAN_HandleTypeDef* hcan, U8 gophercan_bus_id, U8 bus_number)
{
	switch (bus_number)
	{
#if NUM_OF_BUSSES > 2
	case 2:
		hcan_2 = hcan;
		can_2_gopher_id = gophercan_bus_id;
		break;
#endif

#if NUM_OF_BUSSES > 1
	case 1:
		hcan_1 = hcan;
		can_1_gopher_id = gophercan_bus_id;
		break;
#endif

	default:
		// don't do anything, if the hcan pointer is not found it defaults to bus 0 on the module
		break;
	}
}


// HAL_CAN_RxFifo0MsgPendingCallback
//  ISR called when CAN_RX_FIFO0 has a pending message
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	service_can_rx_hardware(hcan, CAN_RX_FIFO0);

	// TODO router specific functionality that directly adds messages that need to be routed
	//  directly to the correct TX buffer
}


// HAL_CAN_RxFifo1MsgPendingCallback
//  ISR called when CAN_RX_FIFO1 has a pending message
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	service_can_rx_hardware(hcan, CAN_RX_FIFO1);

	// TODO router specific functionality that directly adds messages that need to be routed
	//  directly to the correct TX buffer
}


// the F7xx has ISRs for available TX mailboxes having an opening
#ifdef F7XX

// TODO tx mailbox opening ISRs
//  make sure when the appropriate tx_buffer is empty, this ISR is disabled or else there will be
//  an infinite loop

#endif


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
	id.parameter = parameter;

	build_message_id(&message, &id);

	// set the RTR bit to REQUEST_DATA
	message.rtr_bit = REQUEST_DATA;

	message.dlc = REQ_PARAM_SIZE;

	// set the pending response to true for this parameter, will be set to true once
	// the value is recieved from the CAN bus
	((CAN_INFO_STRUCT*)(all_parameter_structs[parameter]))->pending_response = TRUE;

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

	// set the RTR bit to be a data message
	message.rtr_bit = DATA_MESSAGE;

	message.dlc = sizeof(command_id) + sizeof(command_parameter);

	message.data[0] = command_id;
	message.data[1] = command_parameter;

	return tx_can_message(&message);
}


// send_parameter
//  function to directly send a CAN message with the specified parameter to
//  another module
// params:
//  U8 priority:    PRIO_LOW or PRIO_HIGH
//  U8 dest_module: what module to send the parameter to
//  U16 parameter:  what parameter to send
// returns:
//  error codes specified in GopherCAN.h
S8 send_parameter(U8 priority, U8 dest_module, U16 parameter)
{
	CAN_ID id;
	CAN_MSG message;
	U64 data = 0;
	S8 c;
	FLOAT_CONVERTER float_con;

	// make sure the parameter is valid
	if (parameter <= CAN_COMMAND_ID || parameter >= NUM_OF_PARAMETERS)
	{
		return BAD_PARAMETER_ID;
	}

	// make sure the parameter is enabled
	if (!((CAN_INFO_STRUCT*)(all_parameter_structs[parameter]))->update_enabled)
	{
		return NOT_ENABLED_ERR;
	}

	// build the return message ID
	id.priority = priority;
	id.dest_module = dest_module;
	id.source_module = this_module_id;
	id.error = FALSE;
	id.parameter = parameter;

	build_message_id(&message, &id);

	// set the RTR bit to data type
	message.rtr_bit = DATA_MESSAGE;

	// get the value of the data on this module and build the CAN message
	if (parameter_data_types[parameter] == UNSIGNED8
		|| parameter_data_types[parameter] == SIGNED8)
	{
		data |= ((U8_CAN_STRUCT*)(all_parameter_structs[parameter]))->data;
		message.dlc = sizeof(U8);
	}

	else if (parameter_data_types[parameter] == UNSIGNED16
		|| parameter_data_types[parameter] == SIGNED16)
	{
		data |= ((U16_CAN_STRUCT*)(all_parameter_structs[parameter]))->data;
		message.dlc = sizeof(U16);
	}

	else if (parameter_data_types[parameter] == UNSIGNED32
		|| parameter_data_types[parameter] == SIGNED32)
	{
		data |= ((U32_CAN_STRUCT*)(all_parameter_structs[parameter]))->data;
		message.dlc = sizeof(U32);
	}

	else if (parameter_data_types[parameter] == UNSIGNED64
		|| parameter_data_types[parameter] == SIGNED64)
	{
		data |= ((U64_CAN_STRUCT*)(all_parameter_structs[parameter]))->data;
		message.dlc = sizeof(U64);
	}

	else if (parameter_data_types[parameter] == FLOATING)
	{
		// Union to get the bitwise data of the float
		float_con.f = ((FLOAT_CAN_STRUCT*)(all_parameter_structs[parameter]))->data;

		data |= float_con.u32;
		message.dlc = sizeof(float);
	}

	// build the data in the message (big endian)
	for (c = message.dlc - 1; c >= 0; c--)
	{
		message.data[c] = (U8)(data >> (c * BITS_IN_BYTE));
	}

	// send the built CAN message
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


// service_can_tx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN commands.
//  then will fill as many tx mailboxes as possible from the tx_message_buffer
//
//  designed to be called at high priority on 1ms loop
void service_can_tx_hardware(CAN_HandleTypeDef* hcan)
{
	CAN_TxHeaderTypeDef tx_header;
	CAN_MSG* message;
	CAN_MSG_RING_BUFFER* buffer;

	// With multiple busses, choose the correct bus buffer to be working with
#if NUM_OF_BUSSES > 2
	if (hcan == hcan_2)
	{
		buffer = &tx_buffer_2;
	}
	else
#endif
#if NUM_OF_BUSSES > 1
	if (hcan == hcan_1)
	{
		buffer = &tx_buffer_1;
	}
	else
#endif
	buffer = &tx_buffer;

	// add messages to the the TX mailboxes until they are full
	while (!is_empty(buffer) && HAL_CAN_GetTxMailboxesFreeLevel(hcan))
	{
		U32 tx_mailbox_num;

		// get the next CAN message from the TX buffer (FIFO)
		message = get_from_buffer(buffer, 0);

		// configure the settings/params of the CAN message
		tx_header.IDE = CAN_ID_EXT;                                          // 29 bit id
		tx_header.TransmitGlobalTime = DISABLE;                              // do not send a timestamp
		tx_header.ExtId = message->id;
		tx_header.RTR = message->rtr_bit;
		tx_header.DLC = message->dlc;

		// add the message to the sending list
		if (HAL_CAN_AddTxMessage(hcan, &tx_header, message->data, &tx_mailbox_num) != HAL_OK)
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


// service_can_rx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN functions.
//  Will take all messages from rx_mailbox (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//  and put them into the rx_buffer
// params:
//  U32 rx_mailbox: the mailbox to service (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//    Make sure this is valid, no error checking is done
//
//  designed to be called as an ISR whenever there is an RX message pending
static void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox)
{
	CAN_RxHeaderTypeDef rx_header;
	CAN_MSG* message;

	// get all the pending RX messages from the RX mailbox and store into the RX buffer
	while (!is_full(&rx_buffer) && HAL_CAN_GetRxFifoFillLevel(hcan, rx_mailbox))
	{
		// set message to the correct pointer from the RX buffer (the "last" message in the buffer)
		message = get_from_buffer(&rx_buffer, rx_buffer.fill_level);

		// Build the message from the registers on the STM32
		if (HAL_CAN_GetRxMessage(hcan, rx_mailbox, &rx_header, message->data) != HAL_OK)
		{
			// this will always be HAL_ERROR. Check hcan->ErrorCode
			// hardware error (do not move the head as the message did not send, try again later)

			hcan_error = hcan->ErrorCode;
			return;
		}

		// modify the rx_buffer data to reflect the new message
		rx_buffer.fill_level++;

		// move the header ID, RTR bit, and DLC into the GopherCAN message struct
		message->rtr_bit = rx_header.RTR;
		message->id = rx_header.ExtId;
		message->dlc = rx_header.DLC;
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
	while (!is_empty(&rx_buffer))
	{
		// get the message at the head of the array
		current_message = get_from_buffer(&rx_buffer, 0);

		// WARNING: CAN errors from other modules are not handled in this version. The message is just discarded
		// Use a CAN bus analyzer to see what the message is for debugging
		service_can_rx_message(current_message);

		// move the head now that the first element has been removed
		remove_from_front(&rx_buffer);
	}

	return CAN_SUCCESS;
}


// tx_can_message
//  Takes in a CAN_MSG struct, adds it to the TX buffer
static S8 tx_can_message(CAN_MSG* message_to_add)
{
	CAN_MSG* buffer_message;
	U8 c;
	CAN_MSG_RING_BUFFER* buffer;

	// If there are multiple busses, choose the correct bus based on the routing table
#if NUM_OF_BUSSES > 1
	U8 dest_module;
	dest_module = (message_to_add->id & DEST_MASK) >> (CAN_ID_SIZE - DEST_POS - DEST_SIZE);
#endif

#if NUM_OF_BUSSES > 2
	if (module_bus_number[dest_module] == can_2_gopher_id)
	{
		buffer = &tx_buffer_2;
	}
	else
#endif
#if NUM_OF_BUSSES > 1
	if (module_bus_number[dest_module] == can_2_gopher_id)
	{
		buffer = &tx_buffer_2;
	}
	else
#endif
	buffer = &tx_buffer;

	// check to make sure the buffer is not full
	if (is_full(buffer))
	{
		return TX_BUFFER_FULL;
	}

	// set the message in the next open element in the buffer to message_to_add (by value, not by reference)
	buffer_message = get_from_buffer(buffer, buffer->fill_level);

	buffer_message->id = message_to_add->id;
	buffer_message->dlc = message_to_add->dlc;
	buffer_message->rtr_bit = message_to_add->rtr_bit;

	for (c = 0; c < buffer_message->dlc; c++)
	{
		buffer_message->data[c] = message_to_add->data[c];
	}

	// adjust the fill_level to reflect the new message added
	buffer->fill_level++;

	// TODO if F7xx, enable TX mailbox slot open interrupts

	return CAN_SUCCESS;
}


// service_can_rx_message
//  CAN message bus interrupt function this will update all
//  the global variables or trigger the CAN functions if needed.
//  Designed to be called by service_can_rx_software to loop perform
//  this task for each pending CAN message
static S8 service_can_rx_message(CAN_MSG* message)
{
	CAN_ID id;
	CAN_INFO_STRUCT* data_struct = 0;
	FLOAT_CONVERTER float_con;
	U64 recieved_data = 0;
	S8 c;

	get_message_id(&id, message);

	// if the message received has the error flag high, put the details into the last_error struct, then return
	if (id.error)
	{
		// this could possibly be changed into a ring buffer
		last_error.last_rx = HAL_GetTick();
		last_error.source_module = id.source_module;
		last_error.parameter = id.parameter;
		if (message->dlc > 0)
		{
			last_error.error_id = message->data[0];
		}

		// return success because the problem is not with the RX
		return CAN_SUCCESS;
	}

	// error checking on the parameter requested
	if (id.parameter < 0 || id.parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(&id, ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}
	
	// get the associated data struct and set last_rx
	data_struct = (CAN_INFO_STRUCT*)(all_parameter_structs[id.parameter]);
	data_struct->last_rx = HAL_GetTick();

    // run command: run the command specified by the CAN message on this module
	if (parameter_data_types[id.parameter] == COMMAND)
	{
		return run_can_command(message, &id);;
	}

	// Check the update_enabled flag (if it is not a CAN command)
	if (!(data_struct->update_enabled))
	{
		send_error_message(&id, PARAM_NOT_ENABLED);
		return NOT_ENABLED_ERR;
	}

	// request parameter: return a CAN message with the data taken from this module
	if (message->rtr_bit)
	{
		return parameter_requested(message, &id);
	}

	// this code should only be reached if the message is a data message

	// build the data U64 (big endian)
	for (c = (message->dlc - 1); c >= 0; c--)
	{
		recieved_data |= message->data[c] << (c * BITS_IN_BYTE);
	}

	// Switch the pending_response flag
	data_struct->pending_response = FALSE;

	// this switch will handle all of the different possible data types
	// that can be sent over CAN
	switch (parameter_data_types[id.parameter])
	{
	case UNSIGNED8:
		((U8_CAN_STRUCT*)(data_struct))->data = (U8)recieved_data;
		return CAN_SUCCESS;

	case UNSIGNED16:
		((U16_CAN_STRUCT*)(data_struct))->data = (U16)recieved_data;
		return CAN_SUCCESS;

	case UNSIGNED32:
		((U32_CAN_STRUCT*)(data_struct))->data = (U32)recieved_data;
		return CAN_SUCCESS;

	case UNSIGNED64:
		((U64_CAN_STRUCT*)(data_struct))->data = (U64)recieved_data;
		return CAN_SUCCESS;

	case SIGNED8:
		((S8_CAN_STRUCT*)(data_struct))->data = (S8)recieved_data;
		return CAN_SUCCESS;

	case SIGNED16:
		((S16_CAN_STRUCT*)(data_struct))->data = (S16)recieved_data;
		return CAN_SUCCESS;

	case SIGNED32:
		((S32_CAN_STRUCT*)(data_struct))->data = (S32)recieved_data;
		return CAN_SUCCESS;

	case SIGNED64:
		((S64_CAN_STRUCT*)(data_struct))->data = (S64)recieved_data;
		return CAN_SUCCESS;

	case FLOATING:
		// Union to get the bitwise data of the float
		float_con.u32 = (U32)recieved_data;

		((FLOAT_CAN_STRUCT*)(data_struct))->data = float_con.f;
		return CAN_SUCCESS;

	default:
		send_error_message(&id, DATATYPE_NOT_FOUND);
		return NOT_FOUND_ERR;
	}

	return CAN_SUCCESS;
}


// parameter_requested
//  return a CAN message with the data taken from this module
static S8 parameter_requested(CAN_MSG* message, CAN_ID* id)
{
	if (message->dlc != REQ_PARAM_SIZE)
	{
		send_error_message(id, SIZE_ERROR);

		return SIZE_ERR;
	}

	// the requested parameter is stored in id->parameter of the received CAN message
	if (id->parameter <= CAN_COMMAND_ID || id->parameter >= NUM_OF_PARAMETERS)
	{
		send_error_message(id, ID_NOT_FOUND);

		return NOT_FOUND_ERR;
	}

	// send the parameter data to the module that requested
	return send_parameter(id->priority, id->source_module, id->parameter);
}


// run_can_command
//  run the command specified by the CAN message on this module
static S8 run_can_command(CAN_MSG* message, CAN_ID* id)
{
	U8 command_id;
	CUST_FUNC* this_function;

	// DLC error checking
	if (message->dlc != COMMAND_SIZE)
	{
		send_error_message(id, SIZE_ERROR);

		return SIZE_ERR;
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
	(*(this_function->func_ptr))(this_function->param_ptr, message->data[COMMAND_PARAMETER_POS]);

	return CAN_SUCCESS;
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

	// set the RTR bit to a data message
	message.rtr_bit = DATA_MESSAGE;

	// set the DLC and data
	message.dlc = sizeof(error_id);
	message.data[0] = error_id;

	// send the CAN message
	return tx_can_message(&message);
}


// do_nothing
//  this exists to give a default function pointer to all of the CAN commands
//  to avoid errors from bad function pointers
void do_nothing(void* param, U8 remote_param)
{
	// this function has successfully done nothing
}


// end of GopherCAN.c
