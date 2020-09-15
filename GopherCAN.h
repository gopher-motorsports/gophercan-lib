/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will include auto generated code based on a spreadsheet of variables

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

//#include "..\\C-Utils\\base_types.h"
#include "base_types.h"

// function prototypes
S8 init_can(U8 module_id);
S8 request_parameter(U8 priority, U8 dest_module, U16 parameter);
S8 send_can_command(U8 priority, U8 dest_module, U8 command_id, U8 command_parameter);
S8 add_custom_can_func(U8 func_id, void (*func_ptr)(void*, U8), U8 init_state, void* param_ptr);
S8 mod_custom_can_func_ptr(U8 func_id, void (*func_ptr)(void*, U8), void* param_ptr);
S8 mod_custom_can_func_state(U8 func_id, U8 state);
void HAL_CAN_RxCallback();


// ******** BEGIN AUTO GENERATED ********

// number of each type
#define NUM_OF_MODULES    4
#define NUM_OF_PARAMETERS 4
#define NUM_OF_COMMANDS   3


// module IDs
#define ECU_ID 0
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
#define SET_LED_STATE  1
#define CUST_COMMAND_2 2


// error IDs
#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5

// ******** END AUTO GENERATED ********


// return messages
#define CAN_SUCCESS         0
#define INIT_FAILED        -1
#define BAD_MODULE_ID      -2
#define BAD_PARAMETER_ID   -3
#define BAD_COMMAND_ID     -4
#define FILTER_SET_FAILED  -5
#define IRQ_SET_FAILED     -6
#define CAN_START_FAILED   -7
#define TX_MAILBOXES_FULL  -8
#define TX_PROBLEM_ADDING  -9


// Data types
typedef enum
{
	REQ_PARAM  = 0,
	COMMAND    = 1,
	UNSIGNED8  = 2,
	UNSIGNED16 = 3,
	UNSIGNED32 = 4,
	UNSIGNED64 = 5,
	SIGNED8    = 6,
	SIGNED16   = 7,
	SIGNED32   = 8,
	SIGNED64   = 9,
	FLOATING   = 10
} datatypes;

// data type sizes (in bytes)
typedef enum
{
	REQ_PARAM_SIZE  = 2,
	COMMAND_SIZE    = 2,
	UNSIGNED8_SIZE  = 1,
	UNSIGNED16_SIZE = 2,
	UNSIGNED32_SIZE = 4,
	UNSIGNED64_SIZE = 8,
	SIGNED8_SIZE    = 1,
	SIGNED16_SIZE   = 2,
	SIGNED32_SIZE   = 4,
	SIGNED64_SIZE   = 8,
	FLOATING_SIZE   = 8
} datatype_size;


// priority defines
#define PRIO_HIGH 0b0
#define PRIO_LOW  0b1


// CAN message ID positions. Sizes are in number of bits
#define CAN_ID_SIZE   29

#define PRIORITY_MASK 0b00010000000000000000000000000000
#define PRIORITY_POS  0
#define PRIORITY_SIZE 1

#define DEST_MASK     0b00001111110000000000000000000000
#define DEST_POS      1
#define DEST_SIZE     6

#define SOURCE_MASK   0b00000000001111110000000000000000
#define SOURCE_POS    7
#define SOURCE_SIZE   6

#define ERROR_MASK    0b00000000000000001000000000000000
#define ERROR_POS     13
#define ERROR_SIZE    1

#define PARAM_MASK    0b00000000000000000111111111111111
#define PARAM_POS     14
#define PARAM_SIZE    15


// custom function data positions
#define COMMAND_ID_POS 0
#define COMMAND_PARAMETER_POS 1


// general defines
#define BITS_IN_BYTE 8
#define U8_MAX 0xFF
#define CAN_INTERRUPT_PRIO 0


// float/U32 converter union
typedef union
{
	float f;
	U32 u32;
} FLOAT_CONVERTER;


// CAN message struct
typedef struct
{
	U32 id;             // only the most significant 29 bits will be used
	U8  dlc;            // [0, 8]
	U8  data[8];        // not all of these will matter depending on dlc
} CAN_MSG;


// CAN ID struct
typedef struct
{
	U8  priority;
	U8  dest_module;
	U8  source_module;
	U8  error;
	U16 parameter;
} CAN_ID;


// custom function struct
typedef struct
{
	void (*func_ptr)(void*, U8);
	U8    func_enabled;
	void* param_ptr;
} CUST_FUNC;


// error message struct
typedef struct
{
	U32 last_rx;
	U8  source_module;
	U16 parameter;
	U8  error_id;
} ERROR_MSG;


// request parameter and custom command structs
typedef struct
{
	U32 last_rx;
	U16 parameter_id;
} REQ_PARAM_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  command_id;
	U8  command_parameter;
} CAN_COMMAND_STRUCT;


// a struct with only the information about each CAN struct, without the data
typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
} CAN_INFO_STRUCT;


// parameter structs
typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	U8  data;
} U8_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	U16 data;
} U16_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	U32 data;
} U32_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	U64 data;
} U64_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	S8  data;
} S8_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	S16 data;
} S16_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	S32 data;
} S32_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	S64 data;
} S64_CAN_STRUCT;

typedef struct
{
	U32   last_rx;
	U8    update_enabled;
	U8    pending_response;
	float data;
} FLOAT_CAN_STRUCT;


#endif /* GOPHERCAN_H_ */
