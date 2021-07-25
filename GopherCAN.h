/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

// this will include auto generated code based on a spreadsheet of variables

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

// this file should be module-specific and exist in the module project directory.
// look at the file "GopherCAN_configs_example.h" for an example
#include "GopherCAN_config.h"


//#ifndef INC_GOPHERCAN_CONFIG_H_
//#error "Problem with GopherCAN_config.h"
//#endif


#include "base_types.h"
#include "GopherCAN_structs.h"
#include "GopherCAN_ring_buffer.h"
#include "GopherCAN_ids.h"

// make sure the target types are defined even if the dev forgot
#ifndef F0XX
#define F0XX -1
#endif
#ifndef F4XX
#define F4XX -4
#endif
#ifndef F7XX
#define F7XX -7
#endif

// choose the correct libraries to use based on the type of module
#if TARGET == F0XX
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_can.h"
#endif

#if TARGET == F4XX
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_can.h"
#endif

#if TARGET == F7XX
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_can.h"
#endif

// get the externs from the auto-generated file
#define AUTOGEN_EXTERNS
#include "GopherCAN_ids.h"

typedef enum
{
    PRIO_HIGH = 0b0,
    PRIO_LOW = 0b1
} PRIORITY;

// master or slave BxCAN type
typedef enum
{
    BXTYPE_MASTER = 0,
    BXTYPE_SLAVE = 1
} BXCAN_TYPE;




// externs for arrays in GopherCAN_ids.c
extern void* all_parameter_structs[NUM_OF_PARAMETERS];
extern U8 parameter_data_types[NUM_OF_PARAMETERS];
#ifdef MULTI_BUS
U8 module_bus_number[NUM_OF_MODULES];
#endif // MULTI_BUS


// function prototypes
S8 init_can(CAN_HandleTypeDef* hcan, MODULE_ID module_id, BXCAN_TYPE bx_type);
void set_all_params_state(boolean enabled);
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
    U8 command_param_0, U8 command_param_1, U8 command_param_2, U8 command_param_3);
S8 send_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);
S8 add_custom_can_func(GCAN_COMMAND_ID command_id, void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8),
    U8 init_state, void* param_ptr);
S8 mod_custom_can_func_state(U8 func_id, U8 state);
S8 service_can_rx_buffer(void);

void service_can_tx_hardware(CAN_HandleTypeDef* hcan);
void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);
void custom_service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);


#ifdef MULTI_BUS
void define_can_bus(CAN_HandleTypeDef* hcan, U8 gophercan_bus_id, U8 bus_number);
#endif

// function to add to the custom CAN commands by default just in case
void do_nothing(MODULE_ID sending_module, void* param,
	U8 remote_param0, U8 remote_param1, U8 remote_param2, U8 remote_param3);

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

// bxcan slave first filter bank starts at 14
#define SLAVE_FIRST_FILTER 14

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


// Macro functions to get different parts of an id from the U32
#define GET_ID_PRIO(id) (((id) & PRIORITY_MASK) >> (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE))
#define GET_ID_DEST(id) (((id) & DEST_MASK) >> (CAN_ID_SIZE - DEST_POS - DEST_SIZE))
#define GET_ID_SOURCE(id) (((id) & SOURCE_MASK) >> (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE))
#define GET_ID_ERROR(id) (((id) & ERROR_MASK) >> (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE))
#define GET_ID_PARAM(id) (((id) & PARAM_MASK) >> (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE))

// Macro function for dealing with the stupid BxCAN filter config
#define GET_ID_HIGH(id) ((((id) << 3) >> 16) & 0xffff)
#define GET_ID_LOW(id) ((((id) << 3) & 0xffff) | CAN_ID_EXT)

// Multi-bus struct
#ifdef MULTI_BUS
typedef struct
{
	CAN_MSG_RING_BUFFER* tx_buffer;
	CAN_HandleTypeDef* hcan;
	U8 gopher_can_id;
} GCAN_MULTI_BUS_STRUCT;
#endif

#endif /* GOPHERCAN_H_ */


// end of GopherCAN.h
