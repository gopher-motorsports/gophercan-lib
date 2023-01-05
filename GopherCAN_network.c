/*
 * parameters.c
 *
 *  Created on: Dec 31, 2022
 *      Author: jonathan
 */

#include "GopherCAN_network.h"

// *** MODULES ***

U8 module_bus_number[NUM_OF_MODULES] = {
    ALL_BUSSES,
    GCAN0,
    GCAN2,
    GCAN1,
    GCAN0,
    GCAN0,
    GCAN2
};

// *** PARAMETERS ***

CAN_COMMAND_STRUCT can_command = {0, 0, 0, CAN_COMMAND_ID};
U16_CAN_STRUCT rpm_ecu = {RPM_ECU_ID, 1, UNSIGNED16, UNSIGNED16_SIZE, LSB, 0.1, 0, 0, 0, 0, 0};
FLOAT_CAN_STRUCT engine_temp_ecu = {ENGINE_TEMP_ECU_ID, 1, FLOATING, FLOATING_SIZE, MSB, 0.1, -180, 0, 0, 0, 0};
U8_CAN_STRUCT tcm_neutral = {TCM_NEUTRAL_ID, 1, UNSIGNED8, UNSIGNED8_SIZE, LSB, 1, 0, 0, 0, 0, 0};

void* PARAMETERS[NUM_OF_PARAMETERS] = {
    &can_command,
    &rpm_ecu,
    &engine_temp_ecu,
    &tcm_neutral
};

// *** GROUPS ***
PARAM_GROUP GROUPS[NUM_OF_GROUPS] = {
        { 1, {RPM_ECU_ID, EMPTY_ID, ENGINE_TEMP_ECU_ID, ENGINE_TEMP_ECU_ID, ENGINE_TEMP_ECU_ID, TCM_NEUTRAL_ID, EMPTY_ID, EMPTY_ID} }
};
