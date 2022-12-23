// start of GopherCAN_ids.c
//  (auto-generated)
//  These files are used in GopherCAN.c using an extern in GopherCAN_ids.h

#include "GopherCAN.h"

// all of the global parameter structs
CAN_COMMAND_STRUCT can_command = {0};
FLOAT_CAN_STRUCT slip_angle = {0};


// this is the struct that will be used to reference based on ID
void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
    &can_command,
    &slip_angle
    
};

// this stores the data_type for each parameter, referenced by ID
U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
    COMMAND,
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

