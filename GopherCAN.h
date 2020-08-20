/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will include auto generated code based on a spreadsheet of variables

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

#include "base_types.h"

// TODO global library of CAN callback functions

// function prototypes
U8 init_can(U8 module_id);
U8 request_parameter(U8 priority, U8 dest_module, U16 parameter);
U8 send_can_command(U8 priority, U8 dest_module, U8 command_id);
U8 add_custom_can_func(U8 func_id, void* (*func_ptr)(void*), U8 init_state, void* param_ptr, void* return_val_ptr);
U8 mod_custom_can_func_ptr(U8 func_id, void* (*func_ptr)(void*), void* param_ptr, void* return_val_ptr);
U8 mod_custom_can_func_state(U8 func_id, U8 state);


// return messages
#define SUCCESS             0
#define INIT_FAILED        -1
#define BAD_PARAMETER      -2
#define ID_OUT_OF_BOUNDS   -3
#define NOT_IMPLEMENTED    -11


// Data types
#define UNSIGNED8  0
#define UNSIGNED16 1
#define UNSIGNED32 2
#define SIGNED8    3
#define SIGNED16   4
#define SIGNED32   5


// priority defines
#define PRIO_HIGH 0b0
#define PRIO_LOW  0b1


// other defines
#define ENABLED  1
#define DISABLED 0
#define TRUE     1
#define FALSE    0


// CAN message struct
typedef struct
{
	U32 id;             // only the least significant 29 bits will be used
	U8  dlc;            // [0, 8]
	U8  data[8];        // not all of these will matter depending on dlc
} CAN_MSG;


// custom function struct
typedef struct
{
	void* (*func_ptr)(void*);
	U8    func_enabled = DISABLED;
	void* param_ptr;
	void* return_val_ptr;
} CUST_FUNC;


// ******** BEGIN AUTO GENERATED ********

// assorted defines
#define NUM_OF_MODUALS    4
#define NUM_OF_PARAMETERS 6
#define NUM_OF_COMMANDS   3


// module IDs
#define ECM_ID 0
#define EDL_ID 1
#define PDM_ID 2
#define TCM_ID 3
#define ACM_ID 4


// request and custom commands
#define REQUEST_VALUE_ID 0
#define CAN_COMMAND_ID   1


// parameter IDs
#define RPM_ID 2
#define FAN_CURRENT_ID 3


// custom command IDs
#define INC_VARIABLE   0
#define TURN_FAN_OFF   1
#define CUST_COMMAND_2 2


// parameter structs
typedef struct
{
	// ID 2
	const U8  data_type        = UNSIGNED16;
	U8        update_enabled   = DISABLED;
	U8        pending_response = FALSE;
	U16       data;
} RPM_STRUCT;

typedef struct
{
	// ID 3
	const U8  data_type        = UNSIGNED8;
	U8        update_enabled   = DISABLED;
	U8        pending_response = FALSE;
	U8        data;
} FAN_CURRENT_STRUCT;
// TODO maybe include a variable to store the last time it was updated

// ******** END AUTO GENERATED ********

#endif /* GOPHERCAN_H_ */
