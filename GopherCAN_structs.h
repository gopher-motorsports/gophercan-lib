// GopherCAN_structs.h
//  structs needed by GopherCAN.c/h and GopherCAN_ring_buffer.c/h


#ifndef GOPHERCAN_STRUCT_H
#define GOPHERCAN_STRUCT_H

#include "base_types.h"
#include "GopherCAN_ids.h"
#include "stm32f7xx_hal.h"

// float/U32 converter union
typedef union
{
	float f;
	U32 u32;
} FLOAT_CONVERTER;


// CAN message struct
typedef struct
{
	CAN_TxHeaderTypeDef header;
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
	void (*func_ptr)(U8, void*, U8, U8, U8, U8);
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

// structs for each parameter type

// custom command struct
typedef struct
{
	U32 last_rx;
	U8  command_id;
	U8  command_parameter;
	GCAN_PARAM_ID param_id;
} CAN_COMMAND_STRUCT;


// a struct with only the information about each CAN struct, without the data
typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
} CAN_INFO_STRUCT;


// parameter structs
typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	U8  data;
} U8_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	U16 data;
} U16_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	U32 data;
} U32_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	U64 data;
} U64_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	S8  data;
} S8_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	S16 data;
} S16_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	S32 data;
} S32_CAN_STRUCT;

typedef struct
{
	U32 last_rx;
	U8  update_enabled;
	U8  pending_response;
	GCAN_PARAM_ID param_id;
	S64 data;
} S64_CAN_STRUCT;

typedef struct
{
	U32   last_rx;
	U8    update_enabled;
	U8    pending_response;
	GCAN_PARAM_ID param_id;
	float data;
} FLOAT_CAN_STRUCT;



#endif


// End of GopherCAN_structs.h
