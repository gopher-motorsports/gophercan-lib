#ifndef GOPHERCAN_NETWORK_H
#define GOPHERCAN_NETWORK_H

#include "base_types.h"

#define CAN_DATA_BYTES 8

// *** TYPES ***

typedef enum {
    COMMAND = 0,
    UNSIGNED8 = 1,
    UNSIGNED16 = 2,
    UNSIGNED32 = 3,
    UNSIGNED64 = 4,
    SIGNED8 = 5,
    SIGNED16 = 6,
    SIGNED32 = 7,
    SIGNED64 = 8,
    FLOATING = 9
} DATATYPES;

typedef enum {
    REQ_PARAM_SIZE = 0,
    COMMAND_SIZE = 5,
    UNSIGNED8_SIZE = 1,
    UNSIGNED16_SIZE = 2,
    UNSIGNED32_SIZE = 4,
    UNSIGNED64_SIZE = 8,
    SIGNED8_SIZE = 1,
    SIGNED16_SIZE = 2,
    SIGNED32_SIZE = 4,
    SIGNED64_SIZE = 8,
    FLOATING_SIZE = 4
} DATATYPES_SIZE;

// *** BUSES ***

#define ALL_BUSSES 0xFF
#define GCAN0 0
#define GCAN1 1
#define GCAN2 2

// *** MODULES ***

typedef enum
{
    ALL_MODULES_ID = 0,
    PLM_ID = 1,
    TCM_ID = 2,
    
    NUM_OF_MODULES
} MODULE_ID;

extern U8 module_bus_number[NUM_OF_MODULES];

// *** PARAMETERS ***

typedef enum {
    EMPTY_ID = -1,
    CAN_COMMAND_ID = 0,
    RPM_ECU_ID = 1,
    ENGINE_TEMP_ECU_ID = 2,
    
    NUM_OF_PARAMETERS
} GCAN_PARAM_ID;

typedef enum {
    LSB,
    MSB
} ENCODING;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
} CAN_INFO_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    U8 data;
} U8_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    U16 data;
} U16_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    U32 data;
} U32_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    U64 data;
} U64_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    S8 data;
} S8_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    S16 data;
} S16_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    S32 data;
} S32_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    S64 data;
} S64_CAN_STRUCT;

typedef struct {
    GCAN_PARAM_ID ID;
    U16 GROUP_ID;
    DATATYPES TYPE;
    DATATYPES_SIZE SIZE;
    ENCODING ENC;
    float SCALE;
    float OFFSET;
    U32 last_tx;
    U32 last_rx;
    U8 pending_response;
    float data;
} FLOAT_CAN_STRUCT;

extern U16_CAN_STRUCT rpm_ecu;
extern FLOAT_CAN_STRUCT engine_temp_ecu;

extern void* PARAMETERS[NUM_OF_PARAMETERS];

// *** GROUPS ***

#define NUM_OF_GROUPS 2

typedef struct
{
    U16 id;
    GCAN_PARAM_ID slots[CAN_DATA_BYTES];
} PARAM_GROUP;

extern PARAM_GROUP GROUPS[NUM_OF_GROUPS];

// *** COMMANDS ***

typedef struct
{
    U32 last_rx;
    U8  command_id;
    U8  command_parameter;
    GCAN_PARAM_ID param_id;
} CAN_COMMAND_STRUCT;

typedef enum
{
    SET_LED_STATE = 0,
    SET_RTC_TIME = 1,
    
    NUM_OF_COMMANDS
} GCAN_COMMAND_ID;

extern CAN_COMMAND_STRUCT can_command;

// *** ERRORS ***

#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5

typedef struct
{
    U32 last_rx;
    U8  source_module;
    U16 parameter;
    U8  error_id;
} ERROR_MSG;

#endif /* GOPHERCAN_NETWORK_H */