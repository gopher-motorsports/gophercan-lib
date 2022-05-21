// GopherCAN_ids.h
//  (auto-generated)
//  This header file houses all of the auto-generated IDs for modules, parameters, and CAN
//  command IDs. This file can be completely auto-generated


#ifndef GOPHERCAN_IDS_H
#define GOPHERCAN_IDS_H

// Generated by GopherCANnon

#define GOPHERCAN_HASH REP_HASH_NOT_IMP


// Module IDs:
typedef enum
{
    ALL_MODULES_ID = 0,
    DLM_ID = 1,
    DAM_F_ID = 2,
    DAM_R_ID = 3,
    PDM_ID = 4,
    TCM_ID = 5,
    ACM_ID = 6,
    
    NUM_OF_MODULES
} MODULE_ID;


// Parameter IDs:
typedef enum
{
    CAN_COMMAND_ID = 0,
    RPM_ECU_ID = 1,
    WHEEL_SLIP_ECU_ID = 2,
    WHEEL_SLIP_TARGET_ECU_ID = 3,
    AIM_RPM_ECU_ID = 4,
    WSFL_ECU_ID = 5,
    WSFR_ECU_ID = 6,
    WSRL_ECU_ID = 7,
    WSRR_ECU_ID = 8,
    IGN_CUT_LEVEL_ECU_ID = 9,
    BAT_VOLTS_ECU_ID = 10,
    ENGINE_TEMP_ECU_ID = 11,
    INLET_AIR_TEMP_ECU_ID = 12,
    LAMBDA_1_ECU_ID = 13,
    THROTTLE_POS_1_ECU_ID = 14,
    THROTTLE_POS_2_ECU_ID = 15,
    PEDAL_POS_1_ECU_ID = 16,
    PEDAL_POS_2_ECU_ID = 17,
    MAN_PRESSURE_ECU_ID = 18,
    FUEL_CUT_LVL_ECU_ID = 19,
    IGN_ADVANCE_ECU_ID = 20,
    OIL_PRESSURE_ECU_ID = 21,
    OIL_TEMP_ECU_ID = 22,
    FUEL_PRESSURE_ECU_ID = 23,
    THROTTLE_POS_REQUIRED_ECU_ID = 24,
    BRAKE_PRESSURE_FRONT_ECU_ID = 25,
    ETC_DUTY_ECU_ID = 26,
    FAN_DUTY_ECU_ID = 27,
    FANOIL_DUTY_ECU_ID = 28,
    WPUMP_DUTY_ECU_ID = 29,
    BRAKE_LIGHT_DUTY_ECU_ID = 30,
    WARNING_LIGHT_DUTY_ECU_ID = 31,
    AUX_5V_ECU_ID = 32,
    ENG_5V_ECU_ID = 33,
    GPS_LAT_ECU_ID = 34,
    GPS_LONG_ECU_ID = 35,
    BRAKE_PRESSURE_REAR_ID = 36,
    STEERING_ANGLE_ID = 37,
    THROTTLE_POSITION_ID = 38,
    ACCELERATION_ID = 39,
    RIDE_HEIGHT_REAR_ID = 40,
    RIDE_HEIGHT_FRONT_ID = 41,
    REAR_RIGHT_SHOCK_POT_ID = 42,
    REAR_LEFT_SHOCK_POT_ID = 43,
    FRONT_LEFT_SHOCK_POT_ID = 44,
    FRONT_RIGHT_SHOCK_POT_ID = 45,
    RIGHT_RAD_PRESSURE_IN_ID = 46,
    RIGHT_RAD_PRESSURE_OUT_ID = 47,
    RIGHT_RAD_PRESSURE_DROP_ID = 48,
    RIGHT_RAD_TEMP_IN_ID = 49,
    RIGHT_RAD_TEMP_OUT_ID = 50,
    LEFT_RAD_PRESSURE_IN_ID = 51,
    LEFT_RAD_PRESSURE_OUT_ID = 52,
    LEFT_RAD_PRESSURE_DROP_ID = 53,
    LEFT_RAD_TEMP_IN_ID = 54,
    LEFT_RAD_TEMP_OUT_ID = 55,
    AIR_TANK_PRESSURE_ID = 56,
    PNUMATIC_LINE_PRESSURE_ID = 57,
    WATER_PUMP_PRESSURE_IN_ID = 58,
    WATER_PUMP_PRESSURE_OUT_ID = 59,
    MAIN_CIRCUIT_FLOW_RATE_ID = 60,
    EXHAUST_TEMP_1_ID = 61,
    EXHAUST_TEMP_2_ID = 62,
    EXHAUST_TEMP_3_ID = 63,
    EXHAUST_TEMP_4_ID = 64,
    EXHAUST_TEMP_5_ID = 65,
    LAMBDA_HEADER_1_ID = 66,
    LAMBDA_HEADER_2_ID = 67,
    OIL_COOLER_TEMP_IN_ID = 68,
    OIL_COOLER_TEMP_OUT_ID = 69,
    OIL_COOLER_PRESSURE_IN_ID = 70,
    OIL_COOLER_PRESSURE_OUT_ID = 71,
    FIREWALL_TEMP_ID = 72,
    YAW_RATE_ID = 73,
    Y_AXIS_ACCEL_ID = 74,
    ROLL_RATE_ID = 75,
    X_AXIS_ACCEL_ID = 76,
    Z_AXIS_ACCEL_ID = 77,
    BRAKE_TEMP_FL_ID = 78,
    BRAKE_TEMP_FR_ID = 79,
    BRAKE_TEMP_RL_ID = 80,
    BRAKE_TEMP_RR_ID = 81,
    TIRE_TEMP_RR_1_ID = 82,
    TIRE_TEMP_RR_2_ID = 83,
    TIRE_TEMP_RR_3_ID = 84,
    TIRE_TEMP_RR_4_ID = 85,
    TIRE_TEMP_RR_5_ID = 86,
    TIRE_TEMP_RR_6_ID = 87,
    TIRE_TEMP_RR_7_ID = 88,
    TIRE_TEMP_RR_8_ID = 89,
    TIRE_TEMP_RR_9_ID = 90,
    TIRE_TEMP_RR_10_ID = 91,
    TIRE_TEMP_RR_11_ID = 92,
    TIRE_TEMP_RR_12_ID = 93,
    TIRE_TEMP_RR_13_ID = 94,
    TIRE_TEMP_RR_14_ID = 95,
    TIRE_TEMP_RR_15_ID = 96,
    TIRE_TEMP_RR_16_ID = 97,
    TIRE_TEMP_RL_1_ID = 98,
    TIRE_TEMP_RL_2_ID = 99,
    TIRE_TEMP_RL_3_ID = 100,
    TIRE_TEMP_RL_4_ID = 101,
    TIRE_TEMP_RL_5_ID = 102,
    TIRE_TEMP_RL_6_ID = 103,
    TIRE_TEMP_RL_7_ID = 104,
    TIRE_TEMP_RL_8_ID = 105,
    TIRE_TEMP_RL_9_ID = 106,
    TIRE_TEMP_RL_10_ID = 107,
    TIRE_TEMP_RL_11_ID = 108,
    TIRE_TEMP_RL_12_ID = 109,
    TIRE_TEMP_RL_13_ID = 110,
    TIRE_TEMP_RL_14_ID = 111,
    TIRE_TEMP_RL_15_ID = 112,
    TIRE_TEMP_RL_16_ID = 113,
    TIRE_TEMP_FL_1_ID = 114,
    TIRE_TEMP_FL_2_ID = 115,
    TIRE_TEMP_FL_3_ID = 116,
    TIRE_TEMP_FL_4_ID = 117,
    TIRE_TEMP_FL_5_ID = 118,
    TIRE_TEMP_FL_6_ID = 119,
    TIRE_TEMP_FL_7_ID = 120,
    TIRE_TEMP_FL_8_ID = 121,
    TIRE_TEMP_FL_9_ID = 122,
    TIRE_TEMP_FL_10_ID = 123,
    TIRE_TEMP_FL_11_ID = 124,
    TIRE_TEMP_FL_12_ID = 125,
    TIRE_TEMP_FL_13_ID = 126,
    TIRE_TEMP_FL_14_ID = 127,
    TIRE_TEMP_FL_15_ID = 128,
    TIRE_TEMP_FL_16_ID = 129,
    TIRE_TEMP_FR_1_ID = 130,
    TIRE_TEMP_FR_2_ID = 131,
    TIRE_TEMP_FR_3_ID = 132,
    TIRE_TEMP_FR_4_ID = 133,
    TIRE_TEMP_FR_5_ID = 134,
    TIRE_TEMP_FR_6_ID = 135,
    TIRE_TEMP_FR_7_ID = 136,
    TIRE_TEMP_FR_8_ID = 137,
    TIRE_TEMP_FR_9_ID = 138,
    TIRE_TEMP_FR_10_ID = 139,
    TIRE_TEMP_FR_11_ID = 140,
    TIRE_TEMP_FR_12_ID = 141,
    TIRE_TEMP_FR_13_ID = 142,
    TIRE_TEMP_FR_14_ID = 143,
    TIRE_TEMP_FR_15_ID = 144,
    TIRE_TEMP_FR_16_ID = 145,
    SW_UPSHIFT_ID = 146,
    SW_DOWNSHIFT_ID = 147,
    SW_CLUTCH_FAST_ID = 148,
    SW_CLUTCH_SLOW_ID = 149,
    SW_AERO_FRONT_ID = 150,
    SW_AERO_REAR_ID = 151,
    TCM_NEUTRAL_ID = 152,
    TCM_SHIFTER_POSITION_ID = 153,
    TCM_CLUTCH_POSITION_ID = 154,
    TCM_TARGET_RPM_ID = 155,
    TCM_TRANS_RPM_ID = 156,
    TCM_CURRENT_GEAR_ID = 157,
    TCM_TARGET_GEAR_ID = 158,
    TCM_CURRENTLY_MOVING_ID = 159,
    TCM_THROTTLE_BLIP_ID = 160,
    TCM_USING_CLUTCH_ID = 161,
    TCM_SUCCESSFUL_SHIFT_ID = 162,
    TCM_ANTI_STALL_ID = 163,
    ACM_FL_WING_ANGLE_ID = 164,
    ACM_FR_WING_ANGLE_ID = 165,
    ACM_REAR_WING_ANGLE_ID = 166,
    PDM_1_CURRENT_ID = 167,
    PDM_2_CURRENT_ID = 168,
    PDM_3_CURRENT_ID = 169,
    PDM_4_CURRENT_ID = 170,
    PDM_5_CURRENT_ID = 171,
    PDM_6_CURRENT_ID = 172,
    PDM_7_CURRENT_ID = 173,
    PDM_8_CURRENT_ID = 174,
    PDM_9_CURRENT_ID = 175,
    PDM_10_CURRENT_ID = 176,
    PDM_11_CURRENT_ID = 177,
    PDM_12_CURRENT_ID = 178,
    PDM_13_CURRENT_ID = 179,
    PDM_14_CURRENT_ID = 180,
    PDM_15_CURRENT_ID = 181,
    PDM_16_CURRENT_ID = 182,
    PDM_17_CURRENT_ID = 183,
    PDM_18_CURRENT_ID = 184,
    PDM_19_CURRENT_ID = 185,
    PDM_20_CURRENT_ID = 186,
    PDM_VOLTAGE_ID = 187,
    PDM_TEMP_ID = 188,
    
    NUM_OF_PARAMETERS
} GCAN_PARAM_ID;


// custom command IDs
typedef enum
{
    SET_LED_STATE = 0,
        // parameter 0: LED state
        // parameter 1: LED state (duplicate)
        // parameter 2: LED state (duplicate)
        // parameter 3: LED state (duplicate)
        // comment: Only the first led_state is used, the rest are duplication for testing
        
    SET_RTC_TIME = 1,
        // parameter 0: hour
        // parameter 1: minute
        // parameter 2: second
        // parameter 3: UNUSED
        // comment: pass in the hours, minutes, and seconds to set the RTC time. This is in 24 hour format
        
    SET_RTC_DATE = 2,
        // parameter 0: year
        // parameter 1: month
        // parameter 2: day
        // parameter 3: weekday
        // comment: pass in the year, month, date, and day of week to set the RTC date. Year is number of years since 1970. For weekday, mon = 1, teus = 2, ..., sun = 7
        
    SEND_BUCKET_PARAMS = 3,
        // parameter 0: UNUSED
        // parameter 1: UNUSED
        // parameter 2: UNUSED
        // parameter 3: UNUSED
        // comment: Command to start the DLM-DAM initialization process
        
    SET_BUCKET_SIZE = 4,
        // parameter 0: bucket ID
        // parameter 1: number of parameters in that bucket
        // parameter 2: UNUSED
        // parameter 3: UNUSED
        // comment: Sets the number of parameters in the bucket specified by the bucket ID
        
    ADD_PARAM_TO_BUCKET = 5,
        // parameter 0: bucket ID
        // parameter 1: parameter ID - most significan byte
        // parameter 2: parameter ID - least significan byte
        // parameter 3: UNUSED
        // comment: Adds a specified parameter to the specified DLM bucket
        
    ASSIGN_BUCKET_TO_FRQ = 6,
        // parameter 0: bucket ID
        // parameter 1: MS between requests - most significant byte
        // parameter 2: MS between requests - least significant byte
        // parameter 3: UNUSED
        // comment: Sets the wait time between requests of this bucket in milliseconds. A value of 0 will stop requesting this bucket
        
    BUCKET_OK = 7,
        // parameter 0: bucket ID
        // parameter 1: UNUSED
        // parameter 2: UNUSED
        // parameter 3: UNUSED
        // comment: This command is sent when the bucket has the number of parameters it expects
        
    REQUEST_BUCKET = 8,
        // parameter 0: bucket ID
        // parameter 1: UNUSED
        // parameter 2: UNUSED
        // parameter 3: UNUSED
        // comment: CAN command to send each parameter in the bucket
        
    
    NUM_OF_COMMANDS
} GCAN_COMMAND_ID;


// Error IDs:
#define ID_NOT_FOUND 0
#define COMMAND_ID_NOT_FOUND 1
#define PARAM_NOT_ENABLED 2
#define SIZE_ERROR 3
#define DATATYPE_NOT_FOUND 4
#define COMMAND_NOT_ENABLED 5



#endif // GOPHERCAN_IDS_H


// this is so this file can be included again when this part is needed
#ifdef AUTOGEN_EXTERNS
#ifndef GOPHERCAN_IDS_H_P2
#define GOPHERCAN_IDS_H_P2

// all of the global parameter struct externs so all files including GopherCAN.h
// have access
extern CAN_COMMAND_STRUCT can_command;
extern U16_CAN_STRUCT rpm_ecu;
extern FLOAT_CAN_STRUCT wheel_slip_ecu;
extern FLOAT_CAN_STRUCT wheel_slip_target_ecu;
extern FLOAT_CAN_STRUCT aim_rpm_ecu;
extern FLOAT_CAN_STRUCT wsfl_ecu;
extern FLOAT_CAN_STRUCT wsfr_ecu;
extern FLOAT_CAN_STRUCT wsrl_ecu;
extern FLOAT_CAN_STRUCT wsrr_ecu;
extern FLOAT_CAN_STRUCT ign_cut_level_ecu;
extern FLOAT_CAN_STRUCT bat_volts_ecu;
extern FLOAT_CAN_STRUCT engine_temp_ecu;
extern FLOAT_CAN_STRUCT inlet_air_temp_ecu;
extern FLOAT_CAN_STRUCT lambda_1_ecu;
extern FLOAT_CAN_STRUCT throttle_pos_1_ecu;
extern FLOAT_CAN_STRUCT throttle_pos_2_ecu;
extern FLOAT_CAN_STRUCT pedal_pos_1_ecu;
extern FLOAT_CAN_STRUCT pedal_pos_2_ecu;
extern FLOAT_CAN_STRUCT man_pressure_ecu;
extern FLOAT_CAN_STRUCT fuel_cut_lvl_ecu;
extern FLOAT_CAN_STRUCT ign_advance_ecu;
extern FLOAT_CAN_STRUCT oil_pressure_ecu;
extern FLOAT_CAN_STRUCT oil_temp_ecu;
extern FLOAT_CAN_STRUCT fuel_pressure_ecu;
extern FLOAT_CAN_STRUCT throttle_pos_required_ecu;
extern FLOAT_CAN_STRUCT brake_pressure_front_ecu;
extern FLOAT_CAN_STRUCT etc_duty_ecu;
extern FLOAT_CAN_STRUCT fan_duty_ecu;
extern FLOAT_CAN_STRUCT fanoil_duty_ecu;
extern FLOAT_CAN_STRUCT wpump_duty_ecu;
extern FLOAT_CAN_STRUCT brake_light_duty_ecu;
extern FLOAT_CAN_STRUCT warning_light_duty_ecu;
extern FLOAT_CAN_STRUCT aux_5v_ecu;
extern FLOAT_CAN_STRUCT eng_5v_ecu;
extern FLOAT_CAN_STRUCT gps_lat_ecu;
extern FLOAT_CAN_STRUCT gps_long_ecu;
extern FLOAT_CAN_STRUCT brake_pressure_rear;
extern FLOAT_CAN_STRUCT steering_angle;
extern FLOAT_CAN_STRUCT throttle_position;
extern FLOAT_CAN_STRUCT acceleration;
extern FLOAT_CAN_STRUCT ride_height_rear;
extern FLOAT_CAN_STRUCT ride_height_front;
extern FLOAT_CAN_STRUCT rear_right_shock_pot;
extern FLOAT_CAN_STRUCT rear_left_shock_pot;
extern FLOAT_CAN_STRUCT front_left_shock_pot;
extern FLOAT_CAN_STRUCT front_right_shock_pot;
extern FLOAT_CAN_STRUCT right_rad_pressure_in;
extern FLOAT_CAN_STRUCT right_rad_pressure_out;
extern FLOAT_CAN_STRUCT right_rad_pressure_drop;
extern FLOAT_CAN_STRUCT right_rad_temp_in;
extern FLOAT_CAN_STRUCT right_rad_temp_out;
extern FLOAT_CAN_STRUCT left_rad_pressure_in;
extern FLOAT_CAN_STRUCT left_rad_pressure_out;
extern FLOAT_CAN_STRUCT left_rad_pressure_drop;
extern FLOAT_CAN_STRUCT left_rad_temp_in;
extern FLOAT_CAN_STRUCT left_rad_temp_out;
extern FLOAT_CAN_STRUCT air_tank_pressure;
extern FLOAT_CAN_STRUCT pnumatic_line_pressure;
extern FLOAT_CAN_STRUCT water_pump_pressure_in;
extern FLOAT_CAN_STRUCT water_pump_pressure_out;
extern FLOAT_CAN_STRUCT main_circuit_flow_rate;
extern FLOAT_CAN_STRUCT exhaust_temp_1;
extern FLOAT_CAN_STRUCT exhaust_temp_2;
extern FLOAT_CAN_STRUCT exhaust_temp_3;
extern FLOAT_CAN_STRUCT exhaust_temp_4;
extern FLOAT_CAN_STRUCT exhaust_temp_5;
extern FLOAT_CAN_STRUCT lambda_header_1;
extern FLOAT_CAN_STRUCT lambda_header_2;
extern FLOAT_CAN_STRUCT oil_cooler_temp_in;
extern FLOAT_CAN_STRUCT oil_cooler_temp_out;
extern FLOAT_CAN_STRUCT oil_cooler_pressure_in;
extern FLOAT_CAN_STRUCT oil_cooler_pressure_out;
extern FLOAT_CAN_STRUCT firewall_temp;
extern FLOAT_CAN_STRUCT yaw_rate;
extern FLOAT_CAN_STRUCT y_axis_accel;
extern FLOAT_CAN_STRUCT roll_rate;
extern FLOAT_CAN_STRUCT x_axis_accel;
extern FLOAT_CAN_STRUCT z_axis_accel;
extern FLOAT_CAN_STRUCT brake_temp_fl;
extern FLOAT_CAN_STRUCT brake_temp_fr;
extern FLOAT_CAN_STRUCT brake_temp_rl;
extern FLOAT_CAN_STRUCT brake_temp_rr;
extern FLOAT_CAN_STRUCT tire_temp_rr_1;
extern FLOAT_CAN_STRUCT tire_temp_rr_2;
extern FLOAT_CAN_STRUCT tire_temp_rr_3;
extern FLOAT_CAN_STRUCT tire_temp_rr_4;
extern FLOAT_CAN_STRUCT tire_temp_rr_5;
extern FLOAT_CAN_STRUCT tire_temp_rr_6;
extern FLOAT_CAN_STRUCT tire_temp_rr_7;
extern FLOAT_CAN_STRUCT tire_temp_rr_8;
extern FLOAT_CAN_STRUCT tire_temp_rr_9;
extern FLOAT_CAN_STRUCT tire_temp_rr_10;
extern FLOAT_CAN_STRUCT tire_temp_rr_11;
extern FLOAT_CAN_STRUCT tire_temp_rr_12;
extern FLOAT_CAN_STRUCT tire_temp_rr_13;
extern FLOAT_CAN_STRUCT tire_temp_rr_14;
extern FLOAT_CAN_STRUCT tire_temp_rr_15;
extern FLOAT_CAN_STRUCT tire_temp_rr_16;
extern FLOAT_CAN_STRUCT tire_temp_rl_1;
extern FLOAT_CAN_STRUCT tire_temp_rl_2;
extern FLOAT_CAN_STRUCT tire_temp_rl_3;
extern FLOAT_CAN_STRUCT tire_temp_rl_4;
extern FLOAT_CAN_STRUCT tire_temp_rl_5;
extern FLOAT_CAN_STRUCT tire_temp_rl_6;
extern FLOAT_CAN_STRUCT tire_temp_rl_7;
extern FLOAT_CAN_STRUCT tire_temp_rl_8;
extern FLOAT_CAN_STRUCT tire_temp_rl_9;
extern FLOAT_CAN_STRUCT tire_temp_rl_10;
extern FLOAT_CAN_STRUCT tire_temp_rl_11;
extern FLOAT_CAN_STRUCT tire_temp_rl_12;
extern FLOAT_CAN_STRUCT tire_temp_rl_13;
extern FLOAT_CAN_STRUCT tire_temp_rl_14;
extern FLOAT_CAN_STRUCT tire_temp_rl_15;
extern FLOAT_CAN_STRUCT tire_temp_rl_16;
extern FLOAT_CAN_STRUCT tire_temp_fl_1;
extern FLOAT_CAN_STRUCT tire_temp_fl_2;
extern FLOAT_CAN_STRUCT tire_temp_fl_3;
extern FLOAT_CAN_STRUCT tire_temp_fl_4;
extern FLOAT_CAN_STRUCT tire_temp_fl_5;
extern FLOAT_CAN_STRUCT tire_temp_fl_6;
extern FLOAT_CAN_STRUCT tire_temp_fl_7;
extern FLOAT_CAN_STRUCT tire_temp_fl_8;
extern FLOAT_CAN_STRUCT tire_temp_fl_9;
extern FLOAT_CAN_STRUCT tire_temp_fl_10;
extern FLOAT_CAN_STRUCT tire_temp_fl_11;
extern FLOAT_CAN_STRUCT tire_temp_fl_12;
extern FLOAT_CAN_STRUCT tire_temp_fl_13;
extern FLOAT_CAN_STRUCT tire_temp_fl_14;
extern FLOAT_CAN_STRUCT tire_temp_fl_15;
extern FLOAT_CAN_STRUCT tire_temp_fl_16;
extern FLOAT_CAN_STRUCT tire_temp_fr_1;
extern FLOAT_CAN_STRUCT tire_temp_fr_2;
extern FLOAT_CAN_STRUCT tire_temp_fr_3;
extern FLOAT_CAN_STRUCT tire_temp_fr_4;
extern FLOAT_CAN_STRUCT tire_temp_fr_5;
extern FLOAT_CAN_STRUCT tire_temp_fr_6;
extern FLOAT_CAN_STRUCT tire_temp_fr_7;
extern FLOAT_CAN_STRUCT tire_temp_fr_8;
extern FLOAT_CAN_STRUCT tire_temp_fr_9;
extern FLOAT_CAN_STRUCT tire_temp_fr_10;
extern FLOAT_CAN_STRUCT tire_temp_fr_11;
extern FLOAT_CAN_STRUCT tire_temp_fr_12;
extern FLOAT_CAN_STRUCT tire_temp_fr_13;
extern FLOAT_CAN_STRUCT tire_temp_fr_14;
extern FLOAT_CAN_STRUCT tire_temp_fr_15;
extern FLOAT_CAN_STRUCT tire_temp_fr_16;
extern U8_CAN_STRUCT sw_upshift;
extern U8_CAN_STRUCT sw_downshift;
extern U8_CAN_STRUCT sw_clutch_fast;
extern U8_CAN_STRUCT sw_clutch_slow;
extern U8_CAN_STRUCT sw_aero_front;
extern U8_CAN_STRUCT sw_aero_rear;
extern U8_CAN_STRUCT tcm_neutral;
extern FLOAT_CAN_STRUCT tcm_shifter_position;
extern FLOAT_CAN_STRUCT tcm_clutch_position;
extern U32_CAN_STRUCT tcm_target_rpm;
extern U32_CAN_STRUCT tcm_trans_rpm;
extern U8_CAN_STRUCT tcm_current_gear;
extern U8_CAN_STRUCT tcm_target_gear;
extern U8_CAN_STRUCT tcm_currently_moving;
extern U8_CAN_STRUCT tcm_throttle_blip;
extern U8_CAN_STRUCT tcm_using_clutch;
extern U8_CAN_STRUCT tcm_successful_shift;
extern U8_CAN_STRUCT tcm_anti_stall;
extern U32_CAN_STRUCT acm_fl_wing_angle;
extern U32_CAN_STRUCT acm_fr_wing_angle;
extern U32_CAN_STRUCT acm_rear_wing_angle;
extern FLOAT_CAN_STRUCT pdm_1_current;
extern FLOAT_CAN_STRUCT pdm_2_current;
extern FLOAT_CAN_STRUCT pdm_3_current;
extern FLOAT_CAN_STRUCT pdm_4_current;
extern FLOAT_CAN_STRUCT pdm_5_current;
extern FLOAT_CAN_STRUCT pdm_6_current;
extern FLOAT_CAN_STRUCT pdm_7_current;
extern FLOAT_CAN_STRUCT pdm_8_current;
extern FLOAT_CAN_STRUCT pdm_9_current;
extern FLOAT_CAN_STRUCT pdm_10_current;
extern FLOAT_CAN_STRUCT pdm_11_current;
extern FLOAT_CAN_STRUCT pdm_12_current;
extern FLOAT_CAN_STRUCT pdm_13_current;
extern FLOAT_CAN_STRUCT pdm_14_current;
extern FLOAT_CAN_STRUCT pdm_15_current;
extern FLOAT_CAN_STRUCT pdm_16_current;
extern FLOAT_CAN_STRUCT pdm_17_current;
extern FLOAT_CAN_STRUCT pdm_18_current;
extern FLOAT_CAN_STRUCT pdm_19_current;
extern FLOAT_CAN_STRUCT pdm_20_current;
extern FLOAT_CAN_STRUCT pdm_voltage;
extern FLOAT_CAN_STRUCT pdm_temp;


#endif // GOPHERCAN_IDS_H_P2
#endif // AUTOGEN_EXTERNS


// End of GopherCAN_ids.h

