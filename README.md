# GopherCAN

This repository holds the source code for the GopherCAN library designed for bxCAN on STM32 microcontrollers.

To see an example of GopherCAN running in a project, look into the 'gcan-boiler-plate' repository

The primary functionality of the library is contained in 'GopherCAN.c' and 'GopherCAN.h'

The following are functions provided by GopherCAN.  More detailed usage information for these functions can be found in the 'GopherCAN.h' file.

```c
S8 init_can(U8 bus_id, CAN_HandleTypeDef* hcan, osMutexId_t tx_mutex, MODULE_ID module_id, BXCAN_TYPE bx_type);

S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);

S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id,
					U8 command_param_0, U8 command_param_1, U8 command_param_2, U8 command_param_3);

S8 send_parameter(CAN_INFO_STRUCT* param);

S8 add_custom_can_func(GCAN_COMMAND_ID command_id, void (*func_ptr)(MODULE_ID, void*, U8, U8, U8, U8),
					   U8 init_state, void* param_ptr);

S8 mod_custom_can_func_state(U8 func_id, U8 state);

void service_can_tx_hardware(CAN_HandleTypeDef* hcan);

void service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

void custom_service_can_rx_hardware(CAN_HandleTypeDef* hcan, U32 rx_mailbox);

S8 service_can_rx_buffer(void);
```

## Library Configuration

Projects using the GopherCAN library should include a 'GopherCAN_config.h' file:

```c
#ifndef GOPHERCAN_CONFIG_H
#define GOPHERCAN_CONFIG_H

// RX and TX buffer sizes (bytes)
#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32

// number of connected CAN buses (max 3)
#define NUM_OF_BUSSES 1

// if defined, GCAN tries to retransmit messages on their destination bus
//#define CAN_ROUTER

// if defined, all CAN messages are accepted
//#define NO_FILTER

// if defined, 11-bit ID data messages are filtered out
//#define IGNORE_DATA

#endif
```