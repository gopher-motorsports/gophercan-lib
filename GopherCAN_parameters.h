/*
 * parameters.h
 *
 *  Created on: Dec 31, 2022
 *      Author: jonathan
 */

#ifndef GOPHERCAN_PARAMETERS_H
#define GOPHERCAN_PARAMETERS_H

#include "base_types.h"

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

typedef enum
{
    CAN_COMMAND_ID = 0,
    RPM_ECU_ID = 1,
    ENGINE_TEMP_ECU_ID = 2,
    TCM_NEUTRAL_ID = 3,

    NUM_OF_PARAMETERS
} GCAN_PARAM_ID;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
} CAN_INFO_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    U8  data;
} U8_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    U16 data;
} U16_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    U32 data;
} U32_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    U64 data;
} U64_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    S8  data;
} S8_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    S16 data;
} S16_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    S32 data;
} S32_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    S64 data;
} S64_CAN_STRUCT;

typedef struct
{
    GCAN_PARAM_ID ID;
    DATATYPES TYPE;
    float SCALE;
    float OFFSET;
    U32 last_rx;
    U8  pending_response;
    float data;
} FLOAT_CAN_STRUCT;

extern void* all_parameter_structs[NUM_OF_PARAMETERS];

#endif /* GOPHERCAN_PARAMETERS_H */

// this is so this file can be included again when this part is needed
#ifdef AUTOGEN_EXTERNS
#ifndef GOPHERCAN_PARAMETERS_EXTERN_H
#define GOPHERCAN_PARAMETERS_EXTERN_H

// all of the global parameter struct externs so all files including GopherCAN.h
// have access
extern CAN_COMMAND_STRUCT can_command;
extern U16_CAN_STRUCT rpm_ecu;
extern FLOAT_CAN_STRUCT engine_temp_ecu;
extern U8_CAN_STRUCT tcm_neutral;


#endif // GOPHERCAN_PARAMETERS_EXTERN_H
#endif // AUTOGEN_EXTERNS
