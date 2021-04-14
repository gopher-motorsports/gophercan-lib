// GopherCAN_structs.h
//  structs needed by GopherCAN.c/h and GopherCAN_ring_buffer.c/h


#ifndef GOPHERCAN_STRUCT_H
#define GOPHERCAN_STRUCT_H

#include "base_types.h"


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


#endif


// End of GopherCAN_structs.h
