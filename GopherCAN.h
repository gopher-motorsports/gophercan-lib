/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will include auto generated code based on a spreadsheet of variables

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

#define F0XX 0
#define F7XX 7

// Configuration defines. These are to be modified by the module specific developer

//#define TARGET F0XX
#define TARGET F7XX

// Note some initialization is different for multi-bus. Check GopherCAN_router_example.c for details
#define MULTI_BUS TRUE
//#define MULTI_BUS FALSE

#if MULTI_BUS == TRUE
#define CAN_ROUTER TRUE
//#define CAN_ROUTER FALSE

// up to 3 busses are supported. That is the most available in the STM32 series
#define NUM_OF_BUSSES 2
#endif

// end Configuration defines

#include "GopherCAN_structs.h"
#include "GopherCAN_ring_buffer.h"
//#include "base_types.h"

#if TARGET == F0XX
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#endif

#if TARGET == F7XX
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#endif


// ******** BEGIN AUTO GENERATED ********


// module IDs
typedef enum
{
	ALL_MODULES_ID = 0,
	DLM_ID = 1,
	ECU_ID = 2,
	PDM_ID = 3,
	TCM_ID = 4,
	ACM_ID = 5,
	DISPLAY_ID = 6
} MODULE_ID;

#define NUM_OF_MODULES 7


// parameter IDs
typedef enum
{
	CAN_COMMAND_ID = 0,
	RPM_ID = 1,
	FAN_CURRENT_ID = 2,
	U8_TESTER_ID = 3,
	U16_TESTER_ID = 4,
	U32_TESTER_ID = 5,
	U64_TESTER_ID = 6,
	S8_TESTER_ID = 7,
	S16_TESTER_ID = 8,
	S32_TESTER_ID = 9,
	S64_TESTER_ID = 10,
	FLOAT_TESTER_ID = 11
} GCAN_PARAM;

#define NUM_OF_PARAMETERS 12


// custom command IDs
typedef enum
{
	INC_VARIABLE = 0,
	SET_LED_STATE = 1,
	CUST_COMMAND_2 = 2
} GCAN_COMMAND;

#define NUM_OF_COMMANDS 3


// error IDs
#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5

// ******** END AUTO GENERATED ********


// priority enum
typedef enum
{
	PRIO_HIGH = 0b0,
	PRIO_LOW = 0b1
} PRIORITY;


// function prototypes
S8 init_can(CAN_HandleTypeDef* hcan, MODULE_ID module_id);
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM parameter);
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND command_id, U8 command_parameter);
S8 send_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM parameter);
S8 add_custom_can_func(U8 func_id, void (*func_ptr)(void*, U8), U8 init_state, void* param_ptr);
S8 mod_custom_can_func_state(U8 func_id, U8 state);
S8 service_can_rx_buffer(void);
void service_can_tx_hardware(CAN_HandleTypeDef* hcan);
void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

#if MULTI_BUS == TRUE
void define_can_bus(CAN_HandleTypeDef* hcan, U8 gophercan_bus_id, U8 bus_number);
#endif

// function to add to the custom CAN commands by default just in case
void do_nothing(void* param, U8 remote_param);

// ISR functions, do not call these in runtime
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan);
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef* hcan);

#if TARGET == F7XX
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox0AbortCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox1AbortCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox2AbortCallback(CAN_HandleTypeDef *hcan);
#endif


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


// return errors
#define INIT_FAILED             -1
#define BAD_MODULE_ID           -2
#define BAD_PARAMETER_ID        -3
#define BAD_COMMAND_ID          -4
#define FILTER_SET_FAILED       -5
#define IRQ_SET_FAILED          -6
#define CAN_START_FAILED        -7
#define TX_BUFFER_FULL          -8
#define TX_PROBLEM_ADDING       -9
#define NOT_FOUND_ERR           -10
#define NOT_ENABLED_ERR         -11
#define SIZE_ERR                -12
#define WRONG_DEST_ERR          -13

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
} DATATYPES;

// data type sizes (in bytes)
typedef enum
{
	REQ_PARAM_SIZE  = 0,
	COMMAND_SIZE    = 5,
	UNSIGNED8_SIZE  = 1,
	UNSIGNED16_SIZE = 2,
	UNSIGNED32_SIZE = 4,
	UNSIGNED64_SIZE = 8,
	SIGNED8_SIZE    = 1,
	SIGNED16_SIZE   = 2,
	SIGNED32_SIZE   = 4,
	SIGNED64_SIZE   = 8,
	FLOATING_SIZE   = 8
} DATATYPES_SIZE;


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
#define COMMAND_PARAM_0 1
#define COMMAND_PARAM_1 2
#define COMMAND_PARAM_2 3
#define COMMAND_PARAM_3 4


// general defines
#define BITS_IN_BYTE 8
#define U8_MAX 0xFF
#define CAN_INTERRUPT_PRIO 0
#define MAX_RX 5
#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32


// Macro functions to get different parts of an id from the U32
#define GET_ID_PRIO(id) ((id & PRIORITY_MASK) >> (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE))
#define GET_ID_DEST(id) ((id & DEST_MASK) >> (CAN_ID_SIZE - DEST_POS - DEST_SIZE))
#define GET_ID_SOURCE(id) ((id & SOURCE_MASK) >> (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE))
#define GET_ID_ERROR(id) ((id & ERROR_MASK) >> (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE))
#define GET_ID_PARAM(id) ((id & PARAM_MASK) >> (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE))


// Multi-bus struct
#if MULTI_BUS == TRUE
typedef struct
{
	CAN_MSG_RING_BUFFER* tx_buffer;
	CAN_HandleTypeDef* hcan;
	U8 gopher_can_id;
} GCAN_MULTI_BUS_STRUCT;
#endif

#endif /* GOPHERCAN_H_ */


// end of GopherCAN.h
