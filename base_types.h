#ifndef BASETYPES_H
#define BASETYPES_H

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
typedef unsigned char       U8 ;
typedef unsigned short      U16;
typedef unsigned long       U32;
typedef unsigned long long  U64;
typedef signed char         S8 ;
typedef signed short        S16;
typedef signed long         S32;
typedef signed long long    S64;

// NULL definition
#ifndef NULL
#define NULL (void*)(0)
#endif // NULL

#endif // BASETYPES_H
