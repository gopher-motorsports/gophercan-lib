// GopherCAN_ids.h
//  This header file houses all of the auto-generated IDs for modules, parameters, and CAN
//  command IDs. This file can be completely auto-generated


#ifndef GOPHERCAN_IDS_H
#define GOPHERCAN_IDS_H

// ******** BEGIN AUTO GENERATED ********


// module IDs
typedef enum
{
	ALL_MODULES_ID = 0,
	DLM_ID = 1,
	DAM_ID = 2,
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
} GCAN_PARAM_ID;

#define NUM_OF_PARAMETERS 12


// custom command IDs
typedef enum
{
	SET_LED_STATE = 0,
	SET_RTC_TIME = 1,
	SET_RTC_DATE = 2,
	SEND_BUCKET_PARAMS = 3,
	SET_BUCKET_SIZE = 4,
	ADD_PARAM_TO_BUCKET = 5,
	ASSIGN_BUCKET_TO_FRQ = 6,
	BUCKET_OK = 7,
	REQUEST_BUCKET = 8
} GCAN_COMMAND_ID;

#define NUM_OF_COMMANDS 9


// error IDs
#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5

#endif

// this is so this file can be included again when this part is needed
#ifdef AUTOGEN_EXTERNS
#ifndef GOPHERCAN_IDS_H_P2
#define GOPHERCAN_IDS_H_P2

// all of the global parameter struct externs so all files including GopherCAN.h
// have access
extern CAN_COMMAND_STRUCT can_command;
extern U16_CAN_STRUCT rpm;
extern U8_CAN_STRUCT fan_current;
extern U8_CAN_STRUCT u8_tester;
extern U16_CAN_STRUCT u16_tester;
extern U32_CAN_STRUCT u32_tester;
extern U64_CAN_STRUCT u64_tester;
extern S8_CAN_STRUCT s8_tester;
extern S16_CAN_STRUCT s16_tester;
extern S32_CAN_STRUCT s32_tester;
extern S64_CAN_STRUCT s64_tester;
extern FLOAT_CAN_STRUCT float_tester;

#endif // GOPHERCAN_IDS_H_P2
#endif // AUTOGEN_EXTERNS


// End of GopherCAN_ids.h
