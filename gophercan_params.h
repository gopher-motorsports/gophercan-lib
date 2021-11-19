/*
 * gophercan_params.h
 *
 *  Created on: Aug 30, 2021
 *      Author: ian
 */

#ifndef GOPHERCAN_PARAMS_H_
#define GOPHERCAN_PARAMS_H_
#include "GopherCAN_structs.h"

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
extern FLOAT_CAN_STRUCT oil_cooler_pressure_in;
extern FLOAT_CAN_STRUCT oil_cooler_pressure_in_lowpass_1000;
extern FLOAT_CAN_STRUCT oil_cooler_pressure_out;
extern FLOAT_CAN_STRUCT oil_cooler_pressure_out_lowpass_1000;
extern FLOAT_CAN_STRUCT rad_water_temp_in;
extern FLOAT_CAN_STRUCT lateral_acceleration;
extern FLOAT_CAN_STRUCT longitudinal_acceleration;
extern FLOAT_CAN_STRUCT yaw_rate;
extern FLOAT_CAN_STRUCT back_left_tire_temp_channel_1;



#endif /* GOPHERCAN_PARAMS_H_ */
