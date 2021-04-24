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


// ******** END AUTO GENERATED ********


#endif


// End of GopherCAN_ids.h
