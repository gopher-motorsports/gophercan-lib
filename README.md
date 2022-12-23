# GopherCAN

This repository holds the source code for the GopherCAN library designed for bxCAN on STM32 microcontrollers.

To see an example of GopherCAN running in a project, look into the 'gcan-boiler-plate' repository

The primary functionality of the library is contained in 'GopherCAN.c' and 'GopherCAN.h'

The following are functions provided by GopherCAN.  More detailed usage information for these functions can be found in the 'GopherCAN.h' file.

```c
S8 init_can(CAN_HandleTypeDef* hcan, MODULE_ID module_id, BXCAN_TYPE bx_type);

void set_all_params_state(boolean enabled);

S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);

S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
					U8 command_param_0, U8 command_param_1, U8 command_param_2, U8 command_param_3);

S8 send_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);

S8 add_custom_can_func(GCAN_COMMAND_ID command_id, void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8),
					   U8 init_state, void* param_ptr);

S8 mod_custom_can_func_state(U8 func_id, U8 state);

void service_can_tx_hardware(CAN_HandleTypeDef* hcan);

void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

void custom_service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

S8 service_can_rx_buffer(void);

In multi-bus mode:

void define_can_bus(CAN_HandleTypeDef* hcan, U8 gophercan_bus_id, U8 bus_number);
```
