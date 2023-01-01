/*
 * parameters.c
 *
 *  Created on: Dec 31, 2022
 *      Author: jonathan
 */

#include "GopherCAN_structs.h"
#include "GopherCAN_parameters.h"

CAN_COMMAND_STRUCT can_command = {0};
U16_CAN_STRUCT rpm_ecu = {RPM_ECU_ID, UNSIGNED16, 0, 0, 0, 0, 0};
FLOAT_CAN_STRUCT engine_temp_ecu = {ENGINE_TEMP_ECU_ID, FLOATING, 0, 0, 0, 0, 0};
U8_CAN_STRUCT tcm_neutral = {TCM_NEUTRAL_ID, UNSIGNED8, 0, 0, 0, 0, 0};

void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
    &can_command,
    &rpm_ecu,
    &engine_temp_ecu,
    &tcm_neutral

};
