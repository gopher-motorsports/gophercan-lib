// start of GopherCAN_ids.c
//  These files are used in GopherCAN.c using an extern in GopherCAN_ids.h

#include "GopherCAN.h"

// all of the global parameter structs
CAN_COMMAND_STRUCT can_command;
U8_CAN_STRUCT u8_tester;
U16_CAN_STRUCT u16_tester;
U32_CAN_STRUCT u32_tester;
U64_CAN_STRUCT u64_tester;
S8_CAN_STRUCT s8_tester;
S16_CAN_STRUCT s16_tester;
S32_CAN_STRUCT s32_tester;
S64_CAN_STRUCT s64_tester;
FLOAT_CAN_STRUCT float_tester;
FLOAT_CAN_STRUCT float_tester_2;
FLOAT_CAN_STRUCT float_tester_3;
FLOAT_CAN_STRUCT float_tester_4;
FLOAT_CAN_STRUCT float_tester_5;
FLOAT_CAN_STRUCT float_tester_6;
U16_CAN_STRUCT rpm;
U8_CAN_STRUCT fan_current;
FLOAT_CAN_STRUCT example_new_param;


// this is the struct that will be used to reference based on ID
void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
    &can_command,
    &u8_tester,
    &u16_tester,
    &u32_tester,
    &u64_tester,
    &s8_tester,
    &s16_tester,
    &s32_tester,
    &s64_tester,
    &float_tester,
    &float_tester_2,
    &float_tester_3,
    &float_tester_4,
    &float_tester_5,
    &float_tester_6,
    &rpm,
    &fan_current,
    &example_new_param
    
};

// this stores the data_type for each parameter, referenced by ID
U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
    COMMAND,
    UNSIGNED8,
    UNSIGNED16,
    UNSIGNED32,
    UNSIGNED64,
    SIGNED8,
    SIGNED16,
    SIGNED32,
    SIGNED64,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    UNSIGNED16,
    UNSIGNED8,
    FLOATING
    
};

// if there are multiple busses, this shows which bus they are on
#ifdef MULTI_BUS
U8 module_bus_number[NUM_OF_MODULES] =
{
    ALL_BUSSES,
    GCAN0,
    GCAN0,
    GCAN1,
    GCAN2,
    GCAN0,
    GCAN1,
    GCAN0
    
};
#endif // MULTI_BUS

// End of GopherCAN_ids.c

