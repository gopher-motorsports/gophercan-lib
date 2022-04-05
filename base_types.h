#ifndef BASETYPES_H
#define BASETYPES_H

#include <stdint.h>

// Buffers will be of size 2 ^ RING_BUFFER_POWER.
// Increasing leads to heavier low-pass filtering of
// average but takes longer to calculate. Suggested value is 8
#define RING_BUFFER_POWER 4

// bitField is used to access individual pins of each port. Credits to Atmel
typedef struct
{
	unsigned char bit0:1;
	unsigned char bit1:1;
	unsigned char bit2:1;
	unsigned char bit3:1;
	unsigned char bit4:1;
	unsigned char bit5:1;
	unsigned char bit6:1;
	unsigned char bit7:1;
}bitField;

typedef enum {
	TRUE = 1,
	FALSE = 0
} boolean;

// Type Definitions
typedef uint8_t   U8 ;
typedef uint16_t  U16;
typedef uint32_t  U32;
typedef uint64_t  U64;
typedef int8_t    S8 ;
typedef int16_t   S16;
typedef int32_t   S32;
typedef int64_t   S64;

// NULL definition
#ifndef NULL
#define NULL (void*)(0)
#endif // NULL

#endif // BASETYPES_H
