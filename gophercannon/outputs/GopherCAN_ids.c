// start of GopherCAN_ids.c
//  (auto-generated)
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
FLOAT_CAN_STRUCT float_tester_7;
FLOAT_CAN_STRUCT dam_chan_1;
FLOAT_CAN_STRUCT dam_chan_2;
FLOAT_CAN_STRUCT dam_chan_3;
FLOAT_CAN_STRUCT dam_chan_4;
FLOAT_CAN_STRUCT dam_chan_5;
FLOAT_CAN_STRUCT dam_chan_6;
FLOAT_CAN_STRUCT dam_chan_7;
FLOAT_CAN_STRUCT dam_chan_8;
FLOAT_CAN_STRUCT dam_chan_9;
FLOAT_CAN_STRUCT dam_chan_10;
FLOAT_CAN_STRUCT dam_chan_11;
FLOAT_CAN_STRUCT dam_chan_12;
FLOAT_CAN_STRUCT dam_chan_13;
FLOAT_CAN_STRUCT dam_chan_14;
FLOAT_CAN_STRUCT dam_chan_15;
FLOAT_CAN_STRUCT dam_chan_16;
FLOAT_CAN_STRUCT dam_chan_17;
FLOAT_CAN_STRUCT dam_chan_18;
FLOAT_CAN_STRUCT dam_chan_19;
FLOAT_CAN_STRUCT dam_chan_20;
FLOAT_CAN_STRUCT dam_chan_21;
FLOAT_CAN_STRUCT dam_chan_22;
FLOAT_CAN_STRUCT dam_chan_23;
FLOAT_CAN_STRUCT dam_chan_24;
FLOAT_CAN_STRUCT brake_pressure;
FLOAT_CAN_STRUCT steering_anglels;
FLOAT_CAN_STRUCT yaw_rate;
FLOAT_CAN_STRUCT throttle_position;
FLOAT_CAN_STRUCT acceleration;
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
    &float_tester_7,
    &dam_chan_1,
    &dam_chan_2,
    &dam_chan_3,
    &dam_chan_4,
    &dam_chan_5,
    &dam_chan_6,
    &dam_chan_7,
    &dam_chan_8,
    &dam_chan_9,
    &dam_chan_10,
    &dam_chan_11,
    &dam_chan_12,
    &dam_chan_13,
    &dam_chan_14,
    &dam_chan_15,
    &dam_chan_16,
    &dam_chan_17,
    &dam_chan_18,
    &dam_chan_19,
    &dam_chan_20,
    &dam_chan_21,
    &dam_chan_22,
    &dam_chan_23,
    &dam_chan_24,
    &brake_pressure,
    &steering_anglels,
    &yaw_rate,
    &throttle_position,
    &acceleration,
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
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
    FLOATING,
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

