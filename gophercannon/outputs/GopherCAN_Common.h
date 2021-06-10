#ifndef GOPHERCAN_COMMON_H
#define GOPHERCAN_COMMON_H

// Generated by GopherCANnon

#define GOPHERCAN_HASH 320255973501901

// Module IDs:
typedef enum
{
    ALL_MODULES_ID = 0,
    ACM_ID = 1,
    DAM_ID = 2,
    
} MODULE_ID;

#define NUM_OF_MODULES 2

// Parameter IDs:
typedef enum
{
    CAN_COMMAND_ID = 0,
    AMBIENT_AIR_PRESSURE_ID = 1,
    AMBIENT_AIR_TEMPERATURE_ID = 1,
    AIRSPEED_FRONT_ID = 1,
    AIRSPEED_REAR_ID = 1,
    YAW_RATE_ID = 1,
    ROLL_RATE_ID = 1,
    LONGITUDINAL_ACCELERATION_ID = 1,
    LATERAL_ACCELERATION_ID = 1,
    VERTICAL_ACCELERATION_ID = 1,
    BRAKE_FLUID_TEMP_FRONT_LEFT_ID = 1,
    BRAKE_FLUID_TEMP_FRONT_RIGHT_ID = 1,
    BRAKE_FLUID_TEMP_REAR_LEFT_ID = 1,
    BRAKE_FLUID_TEMP_REAR_RIGHT_ID = 1,
    BRAKE_PRESSURE_FRONT_ID = 1,
    BRAKE_PRESSURE_REAR_ID = 1,
    BRAKE_PAD_TEMP_FRONT_LEFT_ID = 1,
    BRAKE_PAD_TEMP_FRONT_RIGHT_ID = 1,
    BRAKE_PAD_TEMP_REAR_LEFT_ID = 1,
    BRAKE_PAD_TEMP_REAR_RIGHT_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_LEFT_SECTOR_1_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_LEFT_SECTOR_2_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_LEFT_SECTOR_3_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_LEFT_SECTOR_4_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_RIGHT_SECTOR_1_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_RIGHT_SECTOR_2_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_RIGHT_SECTOR_3_ID = 1,
    BRAKE_ROTOR_TEMP_FRONT_RIGHT_SECTOR_4_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_LEFT_SECTOR_1_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_LEFT_SECTOR_2_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_LEFT_SECTOR_3_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_LEFT_SECTOR_4_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_RIGHT_SECTOR_1_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_RIGHT_SECTOR_2_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_RIGHT_SECTOR_3_ID = 1,
    BRAKE_ROTOR_TEMP_REAR_RIGHT_SECTOR_4_ID = 1,
    CHASSIS_SLIP_ANGLE_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_1_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_2_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_3_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_4_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_5_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_6_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_7_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_8_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_9_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_10_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_11_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_12_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_13_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_14_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_15_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_16_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_1_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_2_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_3_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_4_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_5_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_6_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_7_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_8_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_9_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_10_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_11_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_12_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_13_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_14_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_15_ID = 1,
    EXTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_16_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_1_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_2_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_3_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_4_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_5_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_6_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_7_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_8_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_9_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_10_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_11_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_12_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_13_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_14_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_15_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_16_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_1_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_2_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_3_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_4_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_5_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_6_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_7_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_8_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_9_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_10_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_11_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_12_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_13_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_14_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_15_ID = 1,
    EXTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_16_ID = 1,
    SHOCK_POSITION_FRONT_LEFT_ID = 1,
    SHOCK_POSITION_FRONT_LEFT_ID = 1,
    SHOCK_POSITION_FRONT_LEFT_ID = 1,
    SHOCK_POSITION_FRONT_LEFT_ID = 1,
    FIREWALL_TEMP_ID = 1,
    WATER_FLOWRATE_MAIN_CIRCUIT_ID = 1,
    STEERING_ANGULAR_VELOCITY_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_1_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_2_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_3_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_4_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_5_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_6_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_7_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_8_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_9_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_10_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_11_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_12_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_13_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_14_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_15_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_LEFT_SECTOR_16_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_1_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_2_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_3_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_4_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_5_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_6_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_7_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_8_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_9_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_10_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_11_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_12_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_13_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_14_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_15_ID = 1,
    INTERNAL_TIRE_TEMP_FRONT_RIGHT_SECTOR_16_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_1_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_2_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_3_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_4_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_5_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_6_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_7_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_8_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_9_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_10_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_11_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_12_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_13_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_14_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_15_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_LEFT_SECTOR_16_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_1_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_2_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_3_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_4_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_5_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_6_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_7_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_8_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_9_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_10_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_11_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_12_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_13_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_14_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_15_ID = 1,
    INTERNAL_TIRE_TEMP_REAR_RIGHT_SECTOR_16_ID = 1,
    TIRE_PRESSURE_FRONT_LEFT_GAUGE_ID = 1,
    TIRE_PRESSURE_FRONT_RIGHT_GAUGE_ID = 1,
    TIRE_PRESSURE_REAR_LEFT_GAUGE_ID = 1,
    TIRE_PRESSURE_REAR_RIGHT_GAUGE_ID = 1,
    TIRE_PRESSURE_FRONT_LEFT_ABSOLUTE_ID = 1,
    TIRE_PRESSURE_FRONT_RIGHT_ABSOLUTE_ID = 1,
    TIRE_PRESSURE_REAR_LEFT_ABSOLUTE_ID = 1,
    TIRE_PRESSURE_REAR_RIGHT_ABSOLUTE_ID = 1,
    TIRE_PRESSURE_SENSOR_BATTERY_VOLTAGE_FRONT_LEFT_ID = 1,
    TIRE_PRESSURE_SENSOR_BATTERY_VOLTAGE_FRONT_RIGHT_ID = 1,
    TIRE_PRESSURE_SENSOR_BATTERY_VOLTAGE_REAR_LEFT_ID = 1,
    TIRE_PRESSURE_SENSOR_BATTERY_VOLTAGE_REAR_RIGHT_ID = 1,
    TIRE_PRESSURE_SENSOR_TEMPERATURE_FRONT_LEFT_ID = 1,
    TIRE_PRESSURE_SENSOR_TEMPERATURE_FRONT_RIGHT_ID = 1,
    TIRE_PRESSURE_SENSOR_TEMPERATURE_REAR_LEFT_ID = 1,
    TIRE_PRESSURE_SENSOR_TEMPERATURE_REAR_RIGHT_ID = 1,
    WATER_FLOWRATE_OIL_COOLER_CIRCUIT_ID = 1,
    OIL_COOLER_PRESSURE_INLET_ID = 1,
    OIL_COOLER_PRESSURE_OUTLET_ID = 1,
    OIL_COOLER_WATER_TEMPERATURE_INLET_ID = 1,
    OIL_COOLER_WATER_TEMPERATURE_OUTLET_ID = 1,
    RADIATOR_LEFT_AERODYNAMIC_PRESSURE_DROP_ID = 1,
    RADIATOR_RIGHT_AERODYNAMIC_PRESSURE_DROP_ID = 1,
    RIDE_HEIGHT_FRONT_ID = 1,
    RIDE_HEIGHT_REAR_ID = 1,
    STEERING_ANGULE_ID = 1,
    RADIATOR_LEFT_WATER_PRESSURE_INLET_ID = 1,
    RADIATOR_LEFT_WATER_PRESSURE_OUTLET_ID = 1,
    RADIATOR_LEFT_WATER_TEMPERATURE_INLET_ID = 1,
    RADIATOR_LEFT_WATER_TEMPERATURE_OUTLET_ID = 1,
    ACCELERATOR_PEDAL_POSITION_ID = 1,
    ENGINE_BLOCK_WATER_TEMPERATURE_ID = 1,
    EXHAUST_PRESSURE_ID = 1,
    FUEL_PRESSURE_ID = 1,
    MANIFOLD_AIR_PRESSURE_ID = 1,
    MANIFOLD_AIR_TEMPERATURE_ID = 1,
    OIL_PRESSURE_ID = 1,
    OIL_TEMPERATURE_ID = 1,
    THROTTLE_POSITION_ID = 1,
    WHEEL_SPEED_FRONT_LEFT_ID = 1,
    WHEEL_SPEED_FRONT_RIGHT_ID = 1,
    WHEEL_SPEED_REAR_LEFT_ID = 1,
    WHEEL_SPEED_REAR_RIGHT_ID = 1,
    EXHAUST_TEMP_OUTLET_ID = 1,
    EXHAUST_TEMP_CYLINDER_1_ID = 1,
    EXHAUST_TEMP_CYLINDER_2_ID = 1,
    EXHAUST_TEMP_CYLINDER_3_ID = 1,
    EXHAUST_TEMP_CYLINDER_4_ID = 1,
    LAMBDA_OUTLET_ID = 1,
    LAMBDA_CYLINDER_1_ID = 1,
    LAMBDA_CYLINDER_2_ID = 1,
    LAMBDA_CYLINDER_3_ID = 1,
    LAMBDA_CYLINDER_4_ID = 1,
    BRAKE_OVERTRAVEL_ID = 1,
    CLUTCH_LINKAGE_POSITION_ID = 1,
    TRANSMISSION_RPM_ID = 1,
    NEUTRAL_SWITCH_ID = 1,
    PADDLE_SHIFTING_TANK_PRESSURE_ID = 1,
    PADDLE_SHIFTING_LINE_PRESSURE_ID = 1,
    SHIFT_LINKAGE_POSITION_ID = 1,
    SHIFT_DRUM_POSITION_ID = 1,
    AMBIENT_PRESSURE_ENGINE_BAY_ID = 1,
    AMBIENT_TEMPERATURE_ENGINE_BAY_ID = 1,
    
} GCAN_PARAM;

#define NUM_OF_PARAMETERS 234

// Error IDs:
#define ID_NOT_FOUND_ID = 0,


// global parameter structs
CAN_COMMAND_STRUCT CAN_COMMAND;
FLOAT_CAN_STRUCT ambient_air_pressure,
FLOAT_CAN_STRUCT ambient_air_temperature,
FLOAT_CAN_STRUCT airspeed_front,
FLOAT_CAN_STRUCT airspeed_rear,
FLOAT_CAN_STRUCT yaw_rate,
FLOAT_CAN_STRUCT roll_rate,
FLOAT_CAN_STRUCT longitudinal_acceleration,
FLOAT_CAN_STRUCT lateral_acceleration,
FLOAT_CAN_STRUCT vertical_acceleration,
FLOAT_CAN_STRUCT brake_fluid_temp_front_left,
FLOAT_CAN_STRUCT brake_fluid_temp_front_right,
FLOAT_CAN_STRUCT brake_fluid_temp_rear_left,
FLOAT_CAN_STRUCT brake_fluid_temp_rear_right,
FLOAT_CAN_STRUCT brake_pressure_front,
FLOAT_CAN_STRUCT brake_pressure_rear,
FLOAT_CAN_STRUCT brake_pad_temp_front_left,
FLOAT_CAN_STRUCT brake_pad_temp_front_right,
FLOAT_CAN_STRUCT brake_pad_temp_rear_left,
FLOAT_CAN_STRUCT brake_pad_temp_rear_right,
FLOAT_CAN_STRUCT brake_rotor_temp_front_left_sector_1,
FLOAT_CAN_STRUCT brake_rotor_temp_front_left_sector_2,
FLOAT_CAN_STRUCT brake_rotor_temp_front_left_sector_3,
FLOAT_CAN_STRUCT brake_rotor_temp_front_left_sector_4,
FLOAT_CAN_STRUCT brake_rotor_temp_front_right_sector_1,
FLOAT_CAN_STRUCT brake_rotor_temp_front_right_sector_2,
FLOAT_CAN_STRUCT brake_rotor_temp_front_right_sector_3,
FLOAT_CAN_STRUCT brake_rotor_temp_front_right_sector_4,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_left_sector_1,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_left_sector_2,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_left_sector_3,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_left_sector_4,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_right_sector_1,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_right_sector_2,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_right_sector_3,
FLOAT_CAN_STRUCT brake_rotor_temp_rear_right_sector_4,
FLOAT_CAN_STRUCT chassis_slip_angle,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_1,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_2,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_3,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_4,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_5,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_6,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_7,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_8,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_9,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_10,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_11,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_12,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_13,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_14,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_15,
FLOAT_CAN_STRUCT external_tire_temp_front_left_sector_16,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_1,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_2,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_3,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_4,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_5,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_6,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_7,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_8,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_9,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_10,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_11,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_12,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_13,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_14,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_15,
FLOAT_CAN_STRUCT external_tire_temp_front_right_sector_16,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_1,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_2,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_3,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_4,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_5,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_6,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_7,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_8,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_9,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_10,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_11,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_12,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_13,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_14,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_15,
FLOAT_CAN_STRUCT external_tire_temp_rear_left_sector_16,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_1,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_2,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_3,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_4,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_5,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_6,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_7,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_8,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_9,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_10,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_11,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_12,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_13,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_14,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_15,
FLOAT_CAN_STRUCT external_tire_temp_rear_right_sector_16,
FLOAT_CAN_STRUCT shock_position_front_left,
FLOAT_CAN_STRUCT shock_position_front_left,
FLOAT_CAN_STRUCT shock_position_front_left,
FLOAT_CAN_STRUCT shock_position_front_left,
FLOAT_CAN_STRUCT firewall_temp,
FLOAT_CAN_STRUCT water_flowrate_main_circuit,
FLOAT_CAN_STRUCT steering_angular_velocity,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_1,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_2,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_3,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_4,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_5,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_6,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_7,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_8,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_9,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_10,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_11,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_12,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_13,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_14,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_15,
FLOAT_CAN_STRUCT internal_tire_temp_front_left_sector_16,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_1,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_2,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_3,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_4,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_5,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_6,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_7,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_8,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_9,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_10,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_11,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_12,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_13,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_14,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_15,
FLOAT_CAN_STRUCT internal_tire_temp_front_right_sector_16,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_1,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_2,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_3,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_4,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_5,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_6,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_7,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_8,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_9,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_10,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_11,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_12,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_13,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_14,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_15,
FLOAT_CAN_STRUCT internal_tire_temp_rear_left_sector_16,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_1,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_2,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_3,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_4,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_5,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_6,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_7,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_8,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_9,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_10,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_11,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_12,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_13,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_14,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_15,
FLOAT_CAN_STRUCT internal_tire_temp_rear_right_sector_16,
FLOAT_CAN_STRUCT tire_pressure_front_left_gauge,
FLOAT_CAN_STRUCT tire_pressure_front_right_gauge,
FLOAT_CAN_STRUCT tire_pressure_rear_left_gauge,
FLOAT_CAN_STRUCT tire_pressure_rear_right_gauge,
FLOAT_CAN_STRUCT tire_pressure_front_left_absolute,
FLOAT_CAN_STRUCT tire_pressure_front_right_absolute,
FLOAT_CAN_STRUCT tire_pressure_rear_left_absolute,
FLOAT_CAN_STRUCT tire_pressure_rear_right_absolute,
FLOAT_CAN_STRUCT tire_pressure_sensor_battery_voltage_front_left,
FLOAT_CAN_STRUCT tire_pressure_sensor_battery_voltage_front_right,
FLOAT_CAN_STRUCT tire_pressure_sensor_battery_voltage_rear_left,
FLOAT_CAN_STRUCT tire_pressure_sensor_battery_voltage_rear_right,
FLOAT_CAN_STRUCT tire_pressure_sensor_temperature_front_left,
FLOAT_CAN_STRUCT tire_pressure_sensor_temperature_front_right,
FLOAT_CAN_STRUCT tire_pressure_sensor_temperature_rear_left,
FLOAT_CAN_STRUCT tire_pressure_sensor_temperature_rear_right,
FLOAT_CAN_STRUCT water_flowrate_oil_cooler_circuit,
FLOAT_CAN_STRUCT oil_cooler_pressure_inlet,
FLOAT_CAN_STRUCT oil_cooler_pressure_outlet,
FLOAT_CAN_STRUCT oil_cooler_water_temperature_inlet,
FLOAT_CAN_STRUCT oil_cooler_water_temperature_outlet,
FLOAT_CAN_STRUCT radiator_left_aerodynamic_pressure_drop,
FLOAT_CAN_STRUCT radiator_right_aerodynamic_pressure_drop,
FLOAT_CAN_STRUCT ride_height_front,
FLOAT_CAN_STRUCT ride_height_rear,
FLOAT_CAN_STRUCT steering_angule,
FLOAT_CAN_STRUCT radiator_left_water_pressure_inlet,
FLOAT_CAN_STRUCT radiator_left_water_pressure_outlet,
FLOAT_CAN_STRUCT radiator_left_water_temperature_inlet,
FLOAT_CAN_STRUCT radiator_left_water_temperature_outlet,
FLOAT_CAN_STRUCT accelerator_pedal_position,
FLOAT_CAN_STRUCT engine_block_water_temperature,
FLOAT_CAN_STRUCT exhaust_pressure,
FLOAT_CAN_STRUCT fuel_pressure,
FLOAT_CAN_STRUCT manifold_air_pressure,
FLOAT_CAN_STRUCT manifold_air_temperature,
FLOAT_CAN_STRUCT oil_pressure,
FLOAT_CAN_STRUCT oil_temperature,
FLOAT_CAN_STRUCT throttle_position,
FLOAT_CAN_STRUCT wheel_speed_front_left,
FLOAT_CAN_STRUCT wheel_speed_front_right,
FLOAT_CAN_STRUCT wheel_speed_rear_left,
FLOAT_CAN_STRUCT wheel_speed_rear_right,
FLOAT_CAN_STRUCT exhaust_temp_outlet,
FLOAT_CAN_STRUCT exhaust_temp_cylinder_1,
FLOAT_CAN_STRUCT exhaust_temp_cylinder_2,
FLOAT_CAN_STRUCT exhaust_temp_cylinder_3,
FLOAT_CAN_STRUCT exhaust_temp_cylinder_4,
FLOAT_CAN_STRUCT lambda_outlet,
FLOAT_CAN_STRUCT lambda_cylinder_1,
FLOAT_CAN_STRUCT lambda_cylinder_2,
FLOAT_CAN_STRUCT lambda_cylinder_3,
FLOAT_CAN_STRUCT lambda_cylinder_4,
FLOAT_CAN_STRUCT brake_overtravel,
FLOAT_CAN_STRUCT clutch_linkage_position,
FLOAT_CAN_STRUCT transmission_rpm,
FLOAT_CAN_STRUCT neutral_switch,
FLOAT_CAN_STRUCT paddle_shifting_tank_pressure,
FLOAT_CAN_STRUCT paddle_shifting_line_pressure,
FLOAT_CAN_STRUCT shift_linkage_position,
FLOAT_CAN_STRUCT shift_drum_position,
FLOAT_CAN_STRUCT ambient_pressure_engine_bay,
FLOAT_CAN_STRUCT ambient_temperature_engine_bay,


// parameter lookup table
void* all_parameter_structs[NUM_OF_PARAMETERS] =
{
        &can_command,
        &ambient_air_pressure,
        &ambient_air_temperature,
        &airspeed_front,
        &airspeed_rear,
        &yaw_rate,
        &roll_rate,
        &longitudinal_acceleration,
        &lateral_acceleration,
        &vertical_acceleration,
        &brake_fluid_temp_front_left,
        &brake_fluid_temp_front_right,
        &brake_fluid_temp_rear_left,
        &brake_fluid_temp_rear_right,
        &brake_pressure_front,
        &brake_pressure_rear,
        &brake_pad_temp_front_left,
        &brake_pad_temp_front_right,
        &brake_pad_temp_rear_left,
        &brake_pad_temp_rear_right,
        &brake_rotor_temp_front_left_sector_1,
        &brake_rotor_temp_front_left_sector_2,
        &brake_rotor_temp_front_left_sector_3,
        &brake_rotor_temp_front_left_sector_4,
        &brake_rotor_temp_front_right_sector_1,
        &brake_rotor_temp_front_right_sector_2,
        &brake_rotor_temp_front_right_sector_3,
        &brake_rotor_temp_front_right_sector_4,
        &brake_rotor_temp_rear_left_sector_1,
        &brake_rotor_temp_rear_left_sector_2,
        &brake_rotor_temp_rear_left_sector_3,
        &brake_rotor_temp_rear_left_sector_4,
        &brake_rotor_temp_rear_right_sector_1,
        &brake_rotor_temp_rear_right_sector_2,
        &brake_rotor_temp_rear_right_sector_3,
        &brake_rotor_temp_rear_right_sector_4,
        &chassis_slip_angle,
        &external_tire_temp_front_left_sector_1,
        &external_tire_temp_front_left_sector_2,
        &external_tire_temp_front_left_sector_3,
        &external_tire_temp_front_left_sector_4,
        &external_tire_temp_front_left_sector_5,
        &external_tire_temp_front_left_sector_6,
        &external_tire_temp_front_left_sector_7,
        &external_tire_temp_front_left_sector_8,
        &external_tire_temp_front_left_sector_9,
        &external_tire_temp_front_left_sector_10,
        &external_tire_temp_front_left_sector_11,
        &external_tire_temp_front_left_sector_12,
        &external_tire_temp_front_left_sector_13,
        &external_tire_temp_front_left_sector_14,
        &external_tire_temp_front_left_sector_15,
        &external_tire_temp_front_left_sector_16,
        &external_tire_temp_front_right_sector_1,
        &external_tire_temp_front_right_sector_2,
        &external_tire_temp_front_right_sector_3,
        &external_tire_temp_front_right_sector_4,
        &external_tire_temp_front_right_sector_5,
        &external_tire_temp_front_right_sector_6,
        &external_tire_temp_front_right_sector_7,
        &external_tire_temp_front_right_sector_8,
        &external_tire_temp_front_right_sector_9,
        &external_tire_temp_front_right_sector_10,
        &external_tire_temp_front_right_sector_11,
        &external_tire_temp_front_right_sector_12,
        &external_tire_temp_front_right_sector_13,
        &external_tire_temp_front_right_sector_14,
        &external_tire_temp_front_right_sector_15,
        &external_tire_temp_front_right_sector_16,
        &external_tire_temp_rear_left_sector_1,
        &external_tire_temp_rear_left_sector_2,
        &external_tire_temp_rear_left_sector_3,
        &external_tire_temp_rear_left_sector_4,
        &external_tire_temp_rear_left_sector_5,
        &external_tire_temp_rear_left_sector_6,
        &external_tire_temp_rear_left_sector_7,
        &external_tire_temp_rear_left_sector_8,
        &external_tire_temp_rear_left_sector_9,
        &external_tire_temp_rear_left_sector_10,
        &external_tire_temp_rear_left_sector_11,
        &external_tire_temp_rear_left_sector_12,
        &external_tire_temp_rear_left_sector_13,
        &external_tire_temp_rear_left_sector_14,
        &external_tire_temp_rear_left_sector_15,
        &external_tire_temp_rear_left_sector_16,
        &external_tire_temp_rear_right_sector_1,
        &external_tire_temp_rear_right_sector_2,
        &external_tire_temp_rear_right_sector_3,
        &external_tire_temp_rear_right_sector_4,
        &external_tire_temp_rear_right_sector_5,
        &external_tire_temp_rear_right_sector_6,
        &external_tire_temp_rear_right_sector_7,
        &external_tire_temp_rear_right_sector_8,
        &external_tire_temp_rear_right_sector_9,
        &external_tire_temp_rear_right_sector_10,
        &external_tire_temp_rear_right_sector_11,
        &external_tire_temp_rear_right_sector_12,
        &external_tire_temp_rear_right_sector_13,
        &external_tire_temp_rear_right_sector_14,
        &external_tire_temp_rear_right_sector_15,
        &external_tire_temp_rear_right_sector_16,
        &shock_position_front_left,
        &shock_position_front_left,
        &shock_position_front_left,
        &shock_position_front_left,
        &firewall_temp,
        &water_flowrate_main_circuit,
        &steering_angular_velocity,
        &internal_tire_temp_front_left_sector_1,
        &internal_tire_temp_front_left_sector_2,
        &internal_tire_temp_front_left_sector_3,
        &internal_tire_temp_front_left_sector_4,
        &internal_tire_temp_front_left_sector_5,
        &internal_tire_temp_front_left_sector_6,
        &internal_tire_temp_front_left_sector_7,
        &internal_tire_temp_front_left_sector_8,
        &internal_tire_temp_front_left_sector_9,
        &internal_tire_temp_front_left_sector_10,
        &internal_tire_temp_front_left_sector_11,
        &internal_tire_temp_front_left_sector_12,
        &internal_tire_temp_front_left_sector_13,
        &internal_tire_temp_front_left_sector_14,
        &internal_tire_temp_front_left_sector_15,
        &internal_tire_temp_front_left_sector_16,
        &internal_tire_temp_front_right_sector_1,
        &internal_tire_temp_front_right_sector_2,
        &internal_tire_temp_front_right_sector_3,
        &internal_tire_temp_front_right_sector_4,
        &internal_tire_temp_front_right_sector_5,
        &internal_tire_temp_front_right_sector_6,
        &internal_tire_temp_front_right_sector_7,
        &internal_tire_temp_front_right_sector_8,
        &internal_tire_temp_front_right_sector_9,
        &internal_tire_temp_front_right_sector_10,
        &internal_tire_temp_front_right_sector_11,
        &internal_tire_temp_front_right_sector_12,
        &internal_tire_temp_front_right_sector_13,
        &internal_tire_temp_front_right_sector_14,
        &internal_tire_temp_front_right_sector_15,
        &internal_tire_temp_front_right_sector_16,
        &internal_tire_temp_rear_left_sector_1,
        &internal_tire_temp_rear_left_sector_2,
        &internal_tire_temp_rear_left_sector_3,
        &internal_tire_temp_rear_left_sector_4,
        &internal_tire_temp_rear_left_sector_5,
        &internal_tire_temp_rear_left_sector_6,
        &internal_tire_temp_rear_left_sector_7,
        &internal_tire_temp_rear_left_sector_8,
        &internal_tire_temp_rear_left_sector_9,
        &internal_tire_temp_rear_left_sector_10,
        &internal_tire_temp_rear_left_sector_11,
        &internal_tire_temp_rear_left_sector_12,
        &internal_tire_temp_rear_left_sector_13,
        &internal_tire_temp_rear_left_sector_14,
        &internal_tire_temp_rear_left_sector_15,
        &internal_tire_temp_rear_left_sector_16,
        &internal_tire_temp_rear_right_sector_1,
        &internal_tire_temp_rear_right_sector_2,
        &internal_tire_temp_rear_right_sector_3,
        &internal_tire_temp_rear_right_sector_4,
        &internal_tire_temp_rear_right_sector_5,
        &internal_tire_temp_rear_right_sector_6,
        &internal_tire_temp_rear_right_sector_7,
        &internal_tire_temp_rear_right_sector_8,
        &internal_tire_temp_rear_right_sector_9,
        &internal_tire_temp_rear_right_sector_10,
        &internal_tire_temp_rear_right_sector_11,
        &internal_tire_temp_rear_right_sector_12,
        &internal_tire_temp_rear_right_sector_13,
        &internal_tire_temp_rear_right_sector_14,
        &internal_tire_temp_rear_right_sector_15,
        &internal_tire_temp_rear_right_sector_16,
        &tire_pressure_front_left_gauge,
        &tire_pressure_front_right_gauge,
        &tire_pressure_rear_left_gauge,
        &tire_pressure_rear_right_gauge,
        &tire_pressure_front_left_absolute,
        &tire_pressure_front_right_absolute,
        &tire_pressure_rear_left_absolute,
        &tire_pressure_rear_right_absolute,
        &tire_pressure_sensor_battery_voltage_front_left,
        &tire_pressure_sensor_battery_voltage_front_right,
        &tire_pressure_sensor_battery_voltage_rear_left,
        &tire_pressure_sensor_battery_voltage_rear_right,
        &tire_pressure_sensor_temperature_front_left,
        &tire_pressure_sensor_temperature_front_right,
        &tire_pressure_sensor_temperature_rear_left,
        &tire_pressure_sensor_temperature_rear_right,
        &water_flowrate_oil_cooler_circuit,
        &oil_cooler_pressure_inlet,
        &oil_cooler_pressure_outlet,
        &oil_cooler_water_temperature_inlet,
        &oil_cooler_water_temperature_outlet,
        &radiator_left_aerodynamic_pressure_drop,
        &radiator_right_aerodynamic_pressure_drop,
        &ride_height_front,
        &ride_height_rear,
        &steering_angule,
        &radiator_left_water_pressure_inlet,
        &radiator_left_water_pressure_outlet,
        &radiator_left_water_temperature_inlet,
        &radiator_left_water_temperature_outlet,
        &accelerator_pedal_position,
        &engine_block_water_temperature,
        &exhaust_pressure,
        &fuel_pressure,
        &manifold_air_pressure,
        &manifold_air_temperature,
        &oil_pressure,
        &oil_temperature,
        &throttle_position,
        &wheel_speed_front_left,
        &wheel_speed_front_right,
        &wheel_speed_rear_left,
        &wheel_speed_rear_right,
        &exhaust_temp_outlet,
        &exhaust_temp_cylinder_1,
        &exhaust_temp_cylinder_2,
        &exhaust_temp_cylinder_3,
        &exhaust_temp_cylinder_4,
        &lambda_outlet,
        &lambda_cylinder_1,
        &lambda_cylinder_2,
        &lambda_cylinder_3,
        &lambda_cylinder_4,
        &brake_overtravel,
        &clutch_linkage_position,
        &transmission_rpm,
        &neutral_switch,
        &paddle_shifting_tank_pressure,
        &paddle_shifting_line_pressure,
        &shift_linkage_position,
        &shift_drum_position,
        &ambient_pressure_engine_bay,
        &ambient_temperature_engine_bay,
        
};

U8 parameter_data_types[NUM_OF_PARAMETERS] =
{
        COMMAND,
        UNSIGNED16,
        UNSIGNED8,
        UNSIGNED8,
        UNSIGNED16,
        UNSIGNED32,
        UNSIGNED64,
        SIGNED8,
        SIGNED16,
        SIGNED32,
        SIGNED64,
        FLOATING
};

#endif // GOPHERCAN_COMMON_H