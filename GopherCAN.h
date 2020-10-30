/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will include auto generated code based on a spreadsheet of variables

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

// Configuration defines. These are to be modified by the module specific developer
#define F0XX
//#define F7XX
//#define CAN_ROUTER
//#define MULTI_BUS
#ifdef MULTI_BUS
#define NUM_OF_BUSSES 1
#endif


#include "..\\C-Utils\\base_types.h"
//#include "base_types.h"

#ifdef F0XX
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#endif

#ifdef F7XX
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#endif

// function prototypes
S8 init_can(CAN_HandleTypeDef* hcan, U8 module_id);
S8 request_parameter(U8 priority, U8 dest_module, U16 parameter);
S8 send_can_command(U8 priority, U8 dest_module, U8 command_id, U8 command_parameter);
S8 send_parameter(U8 priority, U8 dest_module, U16 parameter);
S8 add_custom_can_func(U8 func_id, void (*func_ptr)(void*, U8), U8 init_state, void* param_ptr);
S8 mod_custom_can_func_state(U8 func_id, U8 state);
S8 service_can_rx_buffer(void);
void define_can_bus(CAN_HandleTypeDef* hcan, U8 gophercan_bus_id, U8 bus_number);
void service_can_tx_hardware(CAN_HandleTypeDef* hcan);

// function to add to the custom CAN commands by default just in case
void do_nothing(void* param, U8 remote_param);

// ISR functions, do not call these in runtime
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan);


// ******** BEGIN AUTO GENERATED ********


// module IDs
#define ALL_MODULES_ID 0
#define DLM_ID 1
#define ECU_ID 2
#define PDM_ID 3
#define TCM_ID 4
#define ACM_ID 5
#define DISPLAY_ID 6

#define NUM_OF_MODULES 7


// parameter IDs
#define CAN_COMMAND_ID 0
#define RPM_ID 1
#define FAN_CURRENT_ID 2
#define U8_TESTER_ID 3
#define U16_TESTER_ID 4
#define U32_TESTER_ID 5
#define U64_TESTER_ID 6
#define S8_TESTER_ID 7
#define S16_TESTER_ID 8
#define S32_TESTER_ID 9
#define S64_TESTER_ID 10
#define FLOAT_TESTER_ID 11

#define NUM_OF_PARAMETERS 12


// custom command IDs
#define INC_VARIABLE   0
#define SET_LED_STATE  1
#define CUST_COMMAND_2 2

#define NUM_OF_COMMANDS 3


// error IDs
#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5

// ******** END AUTO GENERATED ********


// CAN bus IDs. There should be one of these for each GopherCAN bus on the car, plus the ALL_BUSSES define
#define ALL_BUSSES 0xFF
#define GCAN0 0
#define GCAN1 1
#define GCAN2 2


// return messages
#define CAN_SUCCESS         0
#define NO_NEW_MESSAGE      1
#define NEW_MESSAGE         2
#define MAX_NEW_MESSAGES    3


// Data or Request message for the RTR bit
#define DATA_MESSAGE CAN_RTR_DATA                                     // 0U
#define REQUEST_DATA CAN_RTR_REMOTE                                   // 2U


// errors
#define INIT_FAILED             -1
#define BAD_MODULE_ID           -2
#define BAD_PARAMETER_ID        -3
#define BAD_COMMAND_ID          -4
#define FILTER_SET_FAILED       -5
#define IRQ_SET_FAILED          -6
#define CAN_START_FAILED        -7
#define TX_BUFFER_FULL          -8
#define TX_PROBLEM_ADDING       -9
#define MAX_PENDING_TX          -10
#define NOT_FOUND_ERR           -11
#define NOT_ENABLED_ERR         -12
#define SIZE_ERR                -13

#define NOT_IMPLEMENTED         -99


// Data types
typedef enum
{
	COMMAND    = 0,
	UNSIGNED8  = 1,
	UNSIGNED16 = 2,
	UNSIGNED32 = 3,
	UNSIGNED64 = 4,
	SIGNED8    = 5,
	SIGNED16   = 6,
	SIGNED32   = 7,
	SIGNED64   = 8,
	FLOATING   = 9
} datatypes;

// data type sizes (in bytes)
typedef enum
{
	REQ_PARAM_SIZE  = 0,
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
#define MAX_RX 5
#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32


// float/U32 converter union
typedef union
{
	float f;
	U32 u32;
} FLOAT_CONVERTER;


// CAN message struct
typedef struct
{
	U32 id;             // only the least significant 29 bits will be used
	U8  rtr_bit;        // 0 or 1: DATA_MESSAGE or REQUEST_DATA
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


// custom command struct
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


// end of GopherCAN.h
