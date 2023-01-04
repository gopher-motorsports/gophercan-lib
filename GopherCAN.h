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

#ifndef GOPHERCAN_CONFIG_H
#error "Problem with GopherCAN_config.h"
#endif

#include "base_types.h"
#include "GopherCAN_network.h"
#include "main.h" // main.h includes the HAL

// determine target MCU
#define F0XX 0
#define F4XX 4
#define F7XX 7

#ifdef __STM32F0xx_HAL_H
#define TARGET F0XX
#endif

#ifdef __STM32F4xx_HAL_H
#define TARGET F4XX
#endif

#ifdef __STM32F7xx_HAL_H
#define TARGET F7XX
#endif

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

typedef union
{
    float f;
    U32 u32;
} FLOAT_CONVERTER;

typedef struct
{
    U8  priority;
    U8  dest_module;
    U8  source_module;
    U8  error;
    U16 parameter;
} CAN_ID;

typedef struct
{
    void (*func_ptr)(U8, void*, U8, U8, U8, U8);
    U8    func_enabled;
    void* param_ptr;
} CUST_FUNC;

typedef struct
{
    CAN_TxHeaderTypeDef header;
    U8  data[8];        // not all of these will matter depending on dlc
} CAN_MSG;

typedef struct
{
    CAN_MSG* message_buffer;    // pointer to the array where the messages are stored
    U8 size;                    // the total amount of space available
    U8 head;                    // the position of the "first" element
    U8 fill_level;              // the number of elements after the head that are apart of the buffer
} CAN_MSG_RING_BUFFER;

// function prototypes

// init_can
// 	This function will set up the CAN registers with the inputed module_id
//	as a filter. All parameters that should be enabled should be set after
//  calling this function
// params:
//  CAN_HandleTypeDef* hcan: the BXcan hcan pointer from the STM HAL library
//  MODULE_ID module_id:     what module this is (ex. PDM_ID, ACM_ID)
//  BXCAN_TYPE bx_type:      master or slave BXcan type. This is usually BXTYPE_MASTER
// returns:
//  error codes specified in GopherCAN.h
S8 init_can(CAN_HandleTypeDef* hcan, MODULE_ID module_id, BXCAN_TYPE bx_type);

// request_parameter
// 	This function will send out a CAN message requesting the parameter
//	given by the parameter ID from the module specified by the module ID
// params:
//  PRIORITY priority:        PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:    what module to request the parameter from
//  GCAN_PARAM_ID parameter:  what parameter to request
// returns:
//  error codes specified in GopherCAN.h
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);

// send_can_command
//	This function will send a CAN message with a command specified
//	by command_id to the specified module
// params:
//  PRIORITY priority:          PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:      what module to send the command to
//  GCAN_COMMAND_ID command_id: what command the module should run
//  U8 command_param_0:         parameter 0 to run the function with. May not be used depending on the function
//  U8 command_param_1:         parameter 1
//  U8 command_param_2:         parameter 2
//  U8 command_param_3:         parameter 3
// returns:
//  error codes specified in GopherCAN.h
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
					U8 command_param_0, U8 command_param_1, U8 command_param_2, U8 command_param_3);

// send_parameter
// encodes and sends the specified parameter's group in a standard 11-bit CAN frame
// params:
// CAN_INFO_STRUCT* param: parameter to send (along with its group)
// returns:
// error codes specified in GopherCAN.h
S8 send_parameter(CAN_INFO_STRUCT* param);

// add_custom_can_func
//  add a user function to the array of functions to check if
//  a CAN command message is sent. Note the functions must be of type 'void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8)',
//  so structs and casts are needed to get multiple params. The third-sixth parameter (U8, U8, U8, U8) will be
//  sent by the module in the CAN command message. This function can also be called to overwrite
//  or modify existing custom commands
// params:
//  GCAN_COMMAND_ID command_id:                         what command ID is being defined
//  void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8): the pointer to the function that should be run if this command_id is called
//  U8 init_state:                                      TRUE or FALSE, whether to start with the command enabled
//  void* param_ptr:                                    pointer to the parameter that should be used. This can point to any
//                                                       data type (including NULL) as long as it is casted correctly
// returns:
//  error codes specified in GopherCAN.h
S8 add_custom_can_func(GCAN_COMMAND_ID command_id, void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8),
					   U8 init_state, void* param_ptr);

// mod_custom_can_func_state
//  change the state (enabled or disabled) of the specified custom CAN function
// params:
//  GCAN_COMMAND_ID command_id: what command ID should have its state modified
//  U8 state:                   TRUE or FALSE. what state to set this command to
// returns:
//  error codes specified in GopherCAN.h
S8 mod_custom_can_func_state(U8 func_id, U8 state);

// service_can_tx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN commands.
//  then will fill as many tx mailboxes as possible from the tx_message_buffer
//
//  designed to be called at high priority on 1ms loop
void service_can_tx_hardware(CAN_HandleTypeDef* hcan);

// service_can_rx_hardware
//  Method to interact directly with the CAN registers through the HAL_CAN functions.
//  Will take all messages from rx_mailbox (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//  and put them into the rx_buffer
// params:
// CAN_HandleTypeDef* hcan: the BXcan hcan pointer from the STM HAL library
//  U32 rx_mailbox:         the mailbox to service (CAN_RX_FIFO0 or CAN_RX_FIFO1)
//                           Make sure this is valid, no error checking is done
//
//  designed to be called as an ISR whenever there is an RX message pending
void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

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
S8 service_can_rx_buffer(void);

#if NUM_OF_BUSSES > 1
// define_can_bus
//  Use this function to associate an hcan handle with a specific GopherCAN bus ID.
//  Also send in the bus number [0, 2] for choosing which of the three slots to fill
//  with that bus data.
//  (Example: if hcan = &hcan1, bus_number = 0. hcan = &hcan2, bus_number = 1, ect)
// params:
//  CAN_HandleTypeDef* hcan: Which HAL hcan pointer to assign to this bus
//  U8 gophercan_bus_id:     What GopherCAN bus id this bus will be assigned to. Reference master spreadsheet
//  U8 bus_number:           [0,2], Which local CAN bus is being assigned. This same value can be used to modify
//                            This parameter later if needed
//
// WARNING: if multiple buses are connected, this function must be called as part of the initialization step,
//           right after init() has been called for all active busses
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


// buffer macros
#define IS_FULL(buffer) ((buffer)->fill_level >= (buffer)->size)
#define IS_EMPTY(buffer) ((buffer)->fill_level == 0)
#define GET_FROM_BUFFER(buffer, index) ((buffer)->message_buffer + (((buffer)->head + index) % (buffer)->size))


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
#define ENCODING_ERR            -14
#define DECODING_ERR            -15

#define NOT_IMPLEMENTED         -99

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
#if NUM_OF_BUSSES > 1
typedef struct
{
	CAN_MSG_RING_BUFFER* tx_buffer;
	CAN_HandleTypeDef* hcan;
	U8 gopher_can_id;
} GCAN_MULTI_BUS_STRUCT;
#endif

#endif /* GOPHERCAN_H_ */


// end of GopherCAN.h
