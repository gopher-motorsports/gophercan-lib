#include "GopherCAN_network.h"

// *** MODULES ***

U8 module_bus_number[NUM_OF_MODULES] = {
    ALL_BUSSES,
    GCAN0,
    GCAN1
    
};

// *** PARAMETERS ***

U16_CAN_STRUCT rpm_ecu = {RPM_ECU_ID,1,UNSIGNED16,UNSIGNED16_SIZE,MSB,1,0,0,0,0,0};
FLOAT_CAN_STRUCT engine_temp_ecu = {ENGINE_TEMP_ECU_ID,1,FLOATING,FLOATING_SIZE,MSB,1,0,0,0,0,0};

void* PARAMETERS[NUM_OF_PARAMETERS] = {
    &can_command,
    &rpm_ecu,
    &engine_temp_ecu
    
};

// *** GROUPS ***

PARAM_GROUP GROUPS[NUM_OF_GROUPS] = {
    { 1, {RPM_ECU_ID, RPM_ECU_ID, ENGINE_TEMP_ECU_ID, ENGINE_TEMP_ECU_ID, EMPTY_ID, EMPTY_ID, EMPTY_ID, EMPTY_ID} },
    { 18, {RPM_ECU_ID, EMPTY_ID, ENGINE_TEMP_ECU_ID, EMPTY_ID, EMPTY_ID, EMPTY_ID, EMPTY_ID, EMPTY_ID} }
    
};

// *** COMMANDS ***

CAN_COMMAND_STRUCT can_command = {0, 0, 0, CAN_COMMAND_ID};