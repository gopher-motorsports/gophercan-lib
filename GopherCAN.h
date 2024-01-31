/*
 * GopherCAN.h
 *
 *  Created on: Aug 16, 2020
 *      Author: Calvin
 */

#ifndef GOPHERCAN_H_
#define GOPHERCAN_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "GopherCAN_network.h"

/*************************************************
 * CONFIGURATION
*************************************************/

// modules should include a GopherCAN configuration named "GopherCAN_config.h"
#include "GopherCAN_config.h"

#ifndef GOPHERCAN_CONFIG_H
#error "Problem with GopherCAN_config.h"
#endif

/*************************************************
 * TYPES
*************************************************/

typedef uint8_t   U8 ;
typedef uint16_t  U16;
typedef uint32_t  U32;
typedef uint64_t  U64;
typedef int8_t    S8 ;
typedef int16_t   S16;
typedef int32_t   S32;
typedef int64_t   S64;

typedef enum {
	TRUE = 1,
	FALSE = 0
} boolean;

#ifndef NULL
#define NULL (void*)(0)
#endif

typedef union {
    float f;
    U32 u32;
} FLOAT_CONVERTER;

/*************************************************
 * CAN ID (EXT, 29-bit)
*************************************************/

typedef struct {
    U8  priority;
    U8  dest_module;
    U8  source_module;
    U8  error;
    U16 parameter;
} CAN_ID;

typedef enum {
    PRIO_HIGH = 0b0,
    PRIO_LOW = 0b1
} PRIORITY;

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

// macros to extract 29-bit ID components from a U32
#define GET_ID_PRIO(id) (((id) & PRIORITY_MASK) >> (CAN_ID_SIZE - PRIORITY_POS - PRIORITY_SIZE))
#define GET_ID_DEST(id) (((id) & DEST_MASK) >> (CAN_ID_SIZE - DEST_POS - DEST_SIZE))
#define GET_ID_SOURCE(id) (((id) & SOURCE_MASK) >> (CAN_ID_SIZE - SOURCE_POS - SOURCE_SIZE))
#define GET_ID_ERROR(id) (((id) & ERROR_MASK) >> (CAN_ID_SIZE - ERROR_POS - ERROR_SIZE))
#define GET_ID_PARAM(id) (((id) & PARAM_MASK) >> (CAN_ID_SIZE - PARAM_POS - PARAM_SIZE))

/*************************************************
 * INITIALIZATION
*************************************************/

// init_can
//  Connects an hcan instance to a bus ID, configures ID filters, activates ISRs, starts CAN peripheral
// PARAMS:
//  CAN_HandleTypeDef* hcan: the bxCAN hcan pointer from the STM HAL library
//  BUS_ID bus_id: CAN bus identifier
// RETURNS:
//  error codes specified in GopherCAN.h
S8 init_can(CAN_HandleTypeDef* hcan, BUS_ID bus_id);

/*************************************************
 * EVENT HANDLERS
 * these functions are marked __weak, they should be redefined in application code
*************************************************/

// called by ISRs when a message is received
void GCAN_onRX(CAN_HandleTypeDef* hcan);

// called when an error message (EXT ID) is received
void GCAN_onError(U32 rx_time, U8 source_module, U16 parameter, U8 error_id);

/*************************************************
 * BUFFER SERVICE
*************************************************/

// service_can_tx
//  Moves messages from TX buffer to mailbox. Designed to be called frequently (1ms).
void service_can_tx(CAN_HandleTypeDef* hcan);

// service_can_rx_buffer
//  Processes messages in the RX buffer. Designed to be called frequently (1ms).
S8 service_can_rx_buffer(void);

/*************************************************
 * CALLBACKS
*************************************************/

// attach_callback_std
//  Configure a function to be called when a particular STD ID is received.
//  Note that std_id must be a configured group ID.
// PARAMS:
//  U16 std_id: 11-bit CAN ID to trigger the callback
//  void (*func)(): function pointer accepting no arguments
void attach_callback_std(U16 std_id, void (*func)());

// attach_callback_cmd
//  Configure a function to be called when a particular command is received.
// PARAMS:
//  U16 cmd_id: command ID to trigger the callback
//  void (*func)(): function pointer accepting the source module ID and four U8 arguments
void attach_callback_cmd(GCAN_COMMAND_ID cmd_id, void (*func)(MODULE_ID, U8, U8, U8, U8));

/*************************************************
 * DATA MESSAGE (STD 11-bit ID)
*************************************************/

// send_parameter
//  Sends the group containing a parameter.
S8 send_parameter(GCAN_PARAM_ID param_id);

// send_group
//  Encodes and transmits a group of parameters.
// PARAMS:
//  U16 group_id: the CAN ID of the group to be sent
// RETURNS:
//  error codes specified in GopherCAN.h
S8 send_group(U16 group_id);

/*************************************************
 * PARAMETER REQUESTS (EXT 29-bit ID)
*************************************************/

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

/*************************************************
 * COMMANDS (EXT 29-bit ID)
*************************************************/

// send_can_command
//	This function will send a CAN message with a command specified by command_id to the specified module
// params:
//  PRIORITY priority:          PRIO_LOW or PRIO_HIGH
//  MODULE_ID dest_module:      what module to send the command to
//  GCAN_COMMAND_ID command_id: what command the module should run
//  U8 a0:         argument 0 to run the function with. May not be used depending on the function
//  U8 a1:         argument 1
//  U8 a2:         argument 2
//  U8 a3:         argument 3
// returns:
//  error codes specified in GopherCAN.h
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
					U8 a0, U8 a1, U8 a2, U8 a3);

/*************************************************
 * RETURN CODES
*************************************************/

#define CAN_SUCCESS         0
#define NO_NEW_MESSAGE      1
#define NEW_MESSAGE         2
#define MAX_NEW_MESSAGES    3

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

/*************************************************
 * OTHER STUFF
*************************************************/

// prevents trimming 0s in CAN data field
// this might be needed to communicate with other modules
#define DISABLE_TRIM_ZEROS

// Data or Request message for the RTR bit
#define DATA_MESSAGE CAN_RTR_DATA                // 0U
#define REQUEST_DATA CAN_RTR_REMOTE              // 2U

// bxcan slave first filter bank starts at 14
#define SLAVE_FIRST_FILTER 14

// general defines
#define BITS_IN_BYTE 8
#define U8_MAX 0xFF
#define BEACON_ID 0xE5
#define BEACON_DATA_CHECK 0x0055AA03

// Macro function for dealing with the stupid BxCAN filter config
#define GET_ID_HIGH(id) ((((id) << 3) >> 16) & 0xffff)
#define GET_ID_LOW(id) ((((id) << 3) & 0xffff) | CAN_ID_EXT)

#endif /* GOPHERCAN_H_ */
