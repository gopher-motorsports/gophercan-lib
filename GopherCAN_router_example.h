#ifndef GOPHERCAN_ROUTER_EXAMPLE_H
#define GOPHERCAN_ROUTER_EXAMPLE_H

// some defines used for examples
#define MIN_ON_RPM 100
#define FAN_CURRENT_UPDATE_TIME 1000
#define MIN_PARAM_UPDATE_TIME 100
#define PARAM_UPDATE_TIMEOUT 1000

void router_init(CAN_HandleTypeDef* hcan_ptr0, CAN_HandleTypeDef* hcan_ptr1);
void router_can_rx_loop();
void router_can_hardware_handling();
void router_testing_loop();
void router_main_loop();

#endif
