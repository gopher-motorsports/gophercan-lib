// start of GopherCAN_ids.c
//  (auto-generated)
//  These files are used in GopherCAN.c using an extern in GopherCAN_ids.h

#include "GopherCAN.h"

// all of the global parameter structs
CAN_COMMAND_STRUCT can_command = {0};
U16_CAN_STRUCT rpm_ecu = {0};
FLOAT_CAN_STRUCT wheel_slip_ecu = {0};
FLOAT_CAN_STRUCT wheel_slip_target_ecu = {0};
FLOAT_CAN_STRUCT aim_rpm_ecu = {0};
FLOAT_CAN_STRUCT wsfl_ecu = {0};
FLOAT_CAN_STRUCT wsfr_ecu = {0};
FLOAT_CAN_STRUCT wsrl_ecu = {0};
FLOAT_CAN_STRUCT wsrr_ecu = {0};
FLOAT_CAN_STRUCT ign_cut_level_ecu = {0};
FLOAT_CAN_STRUCT bat_volts_ecu = {0};
FLOAT_CAN_STRUCT engine_temp_ecu = {0};
FLOAT_CAN_STRUCT inlet_air_temp_ecu = {0};
FLOAT_CAN_STRUCT lambda_1_ecu = {0};
FLOAT_CAN_STRUCT throttle_pos_1_ecu = {0};
FLOAT_CAN_STRUCT throttle_pos_2_ecu = {0};
FLOAT_CAN_STRUCT pedal_pos_1_ecu = {0};
FLOAT_CAN_STRUCT pedal_pos_2_ecu = {0};
FLOAT_CAN_STRUCT man_pressure_ecu = {0};
FLOAT_CAN_STRUCT fuel_cut_lvl_ecu = {0};
FLOAT_CAN_STRUCT ign_advance_ecu = {0};
FLOAT_CAN_STRUCT oil_pressure_ecu = {0};
FLOAT_CAN_STRUCT oil_temp_ecu = {0};
FLOAT_CAN_STRUCT fuel_pressure_ecu = {0};
FLOAT_CAN_STRUCT throttle_pos_required_ecu = {0};
FLOAT_CAN_STRUCT brake_pressure_front_ecu = {0};
FLOAT_CAN_STRUCT etc_duty_ecu = {0};
FLOAT_CAN_STRUCT fan_duty_ecu = {0};
FLOAT_CAN_STRUCT fanoil_duty_ecu = {0};
FLOAT_CAN_STRUCT wpump_duty_ecu = {0};
FLOAT_CAN_STRUCT brake_light_duty_ecu = {0};
FLOAT_CAN_STRUCT warning_light_duty_ecu = {0};
FLOAT_CAN_STRUCT aux_5v_ecu = {0};
FLOAT_CAN_STRUCT eng_5v_ecu = {0};
FLOAT_CAN_STRUCT gps_lat_ecu = {0};
FLOAT_CAN_STRUCT gps_long_ecu = {0};
FLOAT_CAN_STRUCT brake_pressure_rear = {0};
FLOAT_CAN_STRUCT steering_angle = {0};
FLOAT_CAN_STRUCT throttle_position = {0};
FLOAT_CAN_STRUCT acceleration = {0};
FLOAT_CAN_STRUCT ride_height_rear = {0};
FLOAT_CAN_STRUCT ride_height_front = {0};
FLOAT_CAN_STRUCT rear_right_shock_pot = {0};
FLOAT_CAN_STRUCT rear_left_shock_pot = {0};
FLOAT_CAN_STRUCT front_left_shock_pot = {0};
FLOAT_CAN_STRUCT front_right_shock_pot = {0};
FLOAT_CAN_STRUCT right_rad_pressure_in = {0};
FLOAT_CAN_STRUCT right_rad_pressure_out = {0};
FLOAT_CAN_STRUCT right_rad_pressure_drop = {0};
FLOAT_CAN_STRUCT right_rad_temp_in = {0};
FLOAT_CAN_STRUCT right_rad_temp_out = {0};
FLOAT_CAN_STRUCT left_rad_pressure_in = {0};
FLOAT_CAN_STRUCT left_rad_pressure_out = {0};
FLOAT_CAN_STRUCT left_rad_pressure_drop = {0};
FLOAT_CAN_STRUCT left_rad_temp_in = {0};
FLOAT_CAN_STRUCT left_rad_temp_out = {0};
FLOAT_CAN_STRUCT air_tank_pressure = {0};
FLOAT_CAN_STRUCT pnumatic_line_pressure = {0};
FLOAT_CAN_STRUCT water_pump_pressure_in = {0};
FLOAT_CAN_STRUCT water_pump_pressure_out = {0};
FLOAT_CAN_STRUCT main_circuit_flow_rate = {0};
FLOAT_CAN_STRUCT exhaust_temp_1 = {0};
FLOAT_CAN_STRUCT exhaust_temp_2 = {0};
FLOAT_CAN_STRUCT exhaust_temp_3 = {0};
FLOAT_CAN_STRUCT exhaust_temp_4 = {0};
FLOAT_CAN_STRUCT exhaust_temp_5 = {0};
FLOAT_CAN_STRUCT lambda_header_1 = {0};
FLOAT_CAN_STRUCT lambda_header_2 = {0};
FLOAT_CAN_STRUCT oil_cooler_temp_in = {0};
FLOAT_CAN_STRUCT oil_cooler_temp_out = {0};
FLOAT_CAN_STRUCT oil_cooler_pressure_in = {0};
FLOAT_CAN_STRUCT oil_cooler_pressure_out = {0};
FLOAT_CAN_STRUCT firewall_temp = {0};
FLOAT_CAN_STRUCT yaw_rate = {0};
FLOAT_CAN_STRUCT y_axis_accel = {0};
FLOAT_CAN_STRUCT roll_rate = {0};
FLOAT_CAN_STRUCT x_axis_accel = {0};
FLOAT_CAN_STRUCT z_axis_accel = {0};
FLOAT_CAN_STRUCT brake_temp_fl = {0};
FLOAT_CAN_STRUCT brake_temp_fr = {0};
FLOAT_CAN_STRUCT brake_temp_rl = {0};
FLOAT_CAN_STRUCT brake_temp_rr = {0};
FLOAT_CAN_STRUCT tire_temp_rr_1 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_2 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_3 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_4 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_5 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_6 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_7 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_8 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_9 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_10 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_11 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_12 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_13 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_14 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_15 = {0};
FLOAT_CAN_STRUCT tire_temp_rr_16 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_1 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_2 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_3 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_4 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_5 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_6 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_7 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_8 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_9 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_10 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_11 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_12 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_13 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_14 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_15 = {0};
FLOAT_CAN_STRUCT tire_temp_rl_16 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_1 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_2 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_3 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_4 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_5 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_6 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_7 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_8 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_9 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_10 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_11 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_12 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_13 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_14 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_15 = {0};
FLOAT_CAN_STRUCT tire_temp_fl_16 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_1 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_2 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_3 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_4 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_5 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_6 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_7 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_8 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_9 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_10 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_11 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_12 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_13 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_14 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_15 = {0};
FLOAT_CAN_STRUCT tire_temp_fr_16 = {0};
U8_CAN_STRUCT sw_upshift = {0};
U8_CAN_STRUCT sw_downshift = {0};
U8_CAN_STRUCT sw_clutch_fast = {0};
U8_CAN_STRUCT sw_clutch_slow = {0};
U8_CAN_STRUCT sw_aero_front = {0};
U8_CAN_STRUCT sw_aero_rear = {0};
U8_CAN_STRUCT tcm_neutral = {0};
FLOAT_CAN_STRUCT tcm_shifter_position = {0};
FLOAT_CAN_STRUCT tcm_clutch_position = {0};
U32_CAN_STRUCT tcm_target_rpm = {0};
U32_CAN_STRUCT tcm_trans_rpm = {0};
U8_CAN_STRUCT tcm_current_gear = {0};
U8_CAN_STRUCT tcm_target_gear = {0};
U8_CAN_STRUCT tcm_currently_moving = {0};
U8_CAN_STRUCT tcm_throttle_blip = {0};
U8_CAN_STRUCT tcm_using_clutch = {0};
U8_CAN_STRUCT tcm_successful_shift = {0};
U8_CAN_STRUCT tcm_anti_stall = {0};
U32_CAN_STRUCT acm_fl_wing_angle = {0};
U32_CAN_STRUCT acm_fr_wing_angle = {0};
U32_CAN_STRUCT acm_rear_wing_angle = {0};
FLOAT_CAN_STRUCT pdm_0_current = {0};
FLOAT_CAN_STRUCT pdm_1_current = {0};
FLOAT_CAN_STRUCT pdm_2_current = {0};
FLOAT_CAN_STRUCT pdm_3_current = {0};
FLOAT_CAN_STRUCT pdm_4_current = {0};
FLOAT_CAN_STRUCT pdm_5_current = {0};
FLOAT_CAN_STRUCT pdm_6_current = {0};
FLOAT_CAN_STRUCT pdm_7_current = {0};
FLOAT_CAN_STRUCT pdm_8_current = {0};
FLOAT_CAN_STRUCT pdm_9_current = {0};
FLOAT_CAN_STRUCT pdm_10_current = {0};
FLOAT_CAN_STRUCT pdm_11_current = {0};
FLOAT_CAN_STRUCT pdm_12_current = {0};
FLOAT_CAN_STRUCT pdm_13_current = {0};
FLOAT_CAN_STRUCT pdm_14_current = {0};
FLOAT_CAN_STRUCT pdm_15_current = {0};
FLOAT_CAN_STRUCT pdm_16_current = {0};
FLOAT_CAN_STRUCT pdm_17_current = {0};
FLOAT_CAN_STRUCT pdm_18_current = {0};
FLOAT_CAN_STRUCT pdm_19_current = {0};
FLOAT_CAN_STRUCT pdm_voltage = {0};
FLOAT_CAN_STRUCT pdm_temp = {0};


// this is the struct that will be used to reference based on ID
void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
    &can_command,
    &rpm_ecu,
    &wheel_slip_ecu,
    &wheel_slip_target_ecu,
    &aim_rpm_ecu,
    &wsfl_ecu,
    &wsfr_ecu,
    &wsrl_ecu,
    &wsrr_ecu,
    &ign_cut_level_ecu,
    &bat_volts_ecu,
    &engine_temp_ecu,
    &inlet_air_temp_ecu,
    &lambda_1_ecu,
    &throttle_pos_1_ecu,
    &throttle_pos_2_ecu,
    &pedal_pos_1_ecu,
    &pedal_pos_2_ecu,
    &man_pressure_ecu,
    &fuel_cut_lvl_ecu,
    &ign_advance_ecu,
    &oil_pressure_ecu,
    &oil_temp_ecu,
    &fuel_pressure_ecu,
    &throttle_pos_required_ecu,
    &brake_pressure_front_ecu,
    &etc_duty_ecu,
    &fan_duty_ecu,
    &fanoil_duty_ecu,
    &wpump_duty_ecu,
    &brake_light_duty_ecu,
    &warning_light_duty_ecu,
    &aux_5v_ecu,
    &eng_5v_ecu,
    &gps_lat_ecu,
    &gps_long_ecu,
    &brake_pressure_rear,
    &steering_angle,
    &throttle_position,
    &acceleration,
    &ride_height_rear,
    &ride_height_front,
    &rear_right_shock_pot,
    &rear_left_shock_pot,
    &front_left_shock_pot,
    &front_right_shock_pot,
    &right_rad_pressure_in,
    &right_rad_pressure_out,
    &right_rad_pressure_drop,
    &right_rad_temp_in,
    &right_rad_temp_out,
    &left_rad_pressure_in,
    &left_rad_pressure_out,
    &left_rad_pressure_drop,
    &left_rad_temp_in,
    &left_rad_temp_out,
    &air_tank_pressure,
    &pnumatic_line_pressure,
    &water_pump_pressure_in,
    &water_pump_pressure_out,
    &main_circuit_flow_rate,
    &exhaust_temp_1,
    &exhaust_temp_2,
    &exhaust_temp_3,
    &exhaust_temp_4,
    &exhaust_temp_5,
    &lambda_header_1,
    &lambda_header_2,
    &oil_cooler_temp_in,
    &oil_cooler_temp_out,
    &oil_cooler_pressure_in,
    &oil_cooler_pressure_out,
    &firewall_temp,
    &yaw_rate,
    &y_axis_accel,
    &roll_rate,
    &x_axis_accel,
    &z_axis_accel,
    &brake_temp_fl,
    &brake_temp_fr,
    &brake_temp_rl,
    &brake_temp_rr,
    &tire_temp_rr_1,
    &tire_temp_rr_2,
    &tire_temp_rr_3,
    &tire_temp_rr_4,
    &tire_temp_rr_5,
    &tire_temp_rr_6,
    &tire_temp_rr_7,
    &tire_temp_rr_8,
    &tire_temp_rr_9,
    &tire_temp_rr_10,
    &tire_temp_rr_11,
    &tire_temp_rr_12,
    &tire_temp_rr_13,
    &tire_temp_rr_14,
    &tire_temp_rr_15,
    &tire_temp_rr_16,
    &tire_temp_rl_1,
    &tire_temp_rl_2,
    &tire_temp_rl_3,
    &tire_temp_rl_4,
    &tire_temp_rl_5,
    &tire_temp_rl_6,
    &tire_temp_rl_7,
    &tire_temp_rl_8,
    &tire_temp_rl_9,
    &tire_temp_rl_10,
    &tire_temp_rl_11,
    &tire_temp_rl_12,
    &tire_temp_rl_13,
    &tire_temp_rl_14,
    &tire_temp_rl_15,
    &tire_temp_rl_16,
    &tire_temp_fl_1,
    &tire_temp_fl_2,
    &tire_temp_fl_3,
    &tire_temp_fl_4,
    &tire_temp_fl_5,
    &tire_temp_fl_6,
    &tire_temp_fl_7,
    &tire_temp_fl_8,
    &tire_temp_fl_9,
    &tire_temp_fl_10,
    &tire_temp_fl_11,
    &tire_temp_fl_12,
    &tire_temp_fl_13,
    &tire_temp_fl_14,
    &tire_temp_fl_15,
    &tire_temp_fl_16,
    &tire_temp_fr_1,
    &tire_temp_fr_2,
    &tire_temp_fr_3,
    &tire_temp_fr_4,
    &tire_temp_fr_5,
    &tire_temp_fr_6,
    &tire_temp_fr_7,
    &tire_temp_fr_8,
    &tire_temp_fr_9,
    &tire_temp_fr_10,
    &tire_temp_fr_11,
    &tire_temp_fr_12,
    &tire_temp_fr_13,
    &tire_temp_fr_14,
    &tire_temp_fr_15,
    &tire_temp_fr_16,
    &sw_upshift,
    &sw_downshift,
    &sw_clutch_fast,
    &sw_clutch_slow,
    &sw_aero_front,
    &sw_aero_rear,
    &tcm_neutral,
    &tcm_shifter_position,
    &tcm_clutch_position,
    &tcm_target_rpm,
    &tcm_trans_rpm,
    &tcm_current_gear,
    &tcm_target_gear,
    &tcm_currently_moving,
    &tcm_throttle_blip,
    &tcm_using_clutch,
    &tcm_successful_shift,
    &tcm_anti_stall,
    &acm_fl_wing_angle,
    &acm_fr_wing_angle,
    &acm_rear_wing_angle,
    &pdm_0_current,
    &pdm_1_current,
    &pdm_2_current,
    &pdm_3_current,
    &pdm_4_current,
    &pdm_5_current,
    &pdm_6_current,
    &pdm_7_current,
    &pdm_8_current,
    &pdm_9_current,
    &pdm_10_current,
    &pdm_11_current,
    &pdm_12_current,
    &pdm_13_current,
    &pdm_14_current,
    &pdm_15_current,
    &pdm_16_current,
    &pdm_17_current,
    &pdm_18_current,
    &pdm_19_current,
    &pdm_voltage,
    &pdm_temp
    
};

// this stores the data_type for each parameter, referenced by ID
U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
    COMMAND,
    UNSIGNED16,
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
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    FLOATING,
    FLOATING,
    UNSIGNED32,
    UNSIGNED32,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED8,
    UNSIGNED32,
    UNSIGNED32,
    UNSIGNED32,
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
    GCAN2,
    GCAN1,
    GCAN0,
    GCAN0,
    GCAN2
    
};
#endif // MULTI_BUS

// End of GopherCAN_ids.c

