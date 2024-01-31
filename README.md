# GopherCAN

GopherCAN is a messaging protocol built on top of [CAN](https://en.wikipedia.org/wiki/CAN_bus).

`gophercan-lib` is a library designed for the bxCAN peripheral on STM32 microcontrollers. It provides utilities for sending and receiving CAN frames following the GopherCAN protocol.

Check out [CSS Electronics | CAN Bus Explained](https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial) for more information on CAN.

## Installation

```
cd FSAE
git clone https://github.com/gopher-motorsports/gophercan-lib.git
```

It's recommended that you create a folder for all Gopher Motorsports projects e.g. `FSAE`. Projects depending on `gophercan-lib` will expect it to be in a sibling directory.

## Configuration

### Network Configuration

YAML configuration files (found in `network_autogen/configs/`) are used to define what parameters, commands, modules, etc. exist in a GopherCAN network. A Python script uses these configs to generate `GopherCAN_network.h/c`:
```
cd network_autogen
python autogen.py configs/[FILENAME].yaml
```

See `network_autogen/configs/example.yaml` for an example configuration.

### Library Configuration

Projects using `gophercan-lib` should include a `GopherCAN_config.h` header:

```c
#ifndef GOPHERCAN_CONFIG_H
#define GOPHERCAN_CONFIG_H

#define THIS_MODULE_ID 1

// RX and TX buffer sizes (bytes)
#define RX_BUFFER_SIZE 128
#define TX_BUFFER_SIZE 32

// ID filters
//#define FILTER_ACCEPT_ALL // accept all messages
#define FILTER_ACCEPT_STD // accept 11-bit IDs
#define FILTER_ACCEPT_EXT_THIS_ID // accept 29-bit IDs with destination = THIS_MODULE_ID
#define FILTER_ACCEPT_EXT_ALL_ID // accept 29-bit IDs with destination = ALL_MODULES_ID

// if defined, messages received with a different destination module (29-bit ID) will be retransmitted
// this can be used to pass messages between unconnected modules
#define ENABLE_ROUTING

#endif
```

## Usage

### Initialization

Each CAN peripheral must be initialized:

```c
S8 init_can(CAN_HandleTypeDef* hcan, BUS_ID bus_id);
```

This ties the peripheral to a `BUS_ID` (defined in the YAML) and message buffer. When sending 29-bit ID messages with a `DESTINATION` field, the bus ID is used to determine which peripheral to send the message to.

### Servicing Buffers

The following functions must be called frequently (e.g. in a 1ms loop):
```c
void service_can_tx(CAN_HandleTypeDef* hcan);
S8 service_can_rx_buffer(void);
```

`service_can_tx` moves messages from the transmit buffer of an hcan instance to the CAN mailbox. The functions described below simply move messages to this buffer. Those messages are actually sent when the peripheral is serviced.

When a message is received, it is automatically moved from the CAN FIFOs to the RX buffer by an interrupt handler. `service_can_rx_buffer` processes the messages in the RX buffer (i.e. updates parameters, triggers callbacks, responds to requests, etc.). 

### Messages

GopherCAN supports four fundamental message types: data, requests, commands and errors.

#### Data

Data messages use an 11-bit CAN ID. They are composed of groups of parameters and have the following format:

| ID<10:0> | Data[0:7] |
| --- | --- |
| `GROUP_ID` | `PARAM0` `PARAM1` `...` |

```c
S8 send_parameter(GCAN_PARAM_ID param_id);
S8 send_group(U16 group_id);
```

The ID of a group and the parameters it contains are defined in the YAML configuration. Groups can be sent either by specifying a parameter ID (the group it belongs to will be sent) or by specifying the group ID directly. Data messages are added to the TX buffer of every bus.

#### Requests

Requests use a 29-bit CAN ID. They contain no data.

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> |
| --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR=0` | `PARAMETER` |

```c
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);
```

Requests are sent on the bus corresponding to the provided `MODULE_ID`. When a request is received and processed in the RX buffer, the specified `PARAMETER` is automatically sent with `send_parameter`.

#### Commands

Commands use a 29-bit CAN ID. The data field supports up to four arguments.

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> | Data[0] | Data[1] | Data[2] | Data[3] | Data[4] | Data[5:7] |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR=0` | `PARAMETER=0` | `COMMAND_ID` | `A0` | `A1` | `A2` | `A3` | 0 |

```c
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id, U8 a0, U8 a1, U8 a2, U8 a3);
```

`A0-3` are arguments to pass along with the command, they are all optional and can have any value.

#### Errors

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> | Data[0] | Data[1:7] |
| --- | --- | --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR=1` | `PARAMETER` | `ERROR_ID` | 0 |

The `PRIORITY` and `PARAMETER` fields will be copied from the message that triggered an error.

### Event Handlers

```c
// called by ISRs when a message is received
void GCAN_onRX(CAN_HandleTypeDef* hcan);

// called when an error message (EXT ID) is received
void GCAN_onError(U32 rx_time, U8 source_module, U16 parameter, U8 error_id);
```

Event handlers allow modules to implement custom logic in reponse to an event. These are defined as `__weak` functions and meant to be overriden. The default definition does nothing.

### Callbacks

Callback functions can be attached to both STD ID messages and commands.

```c
void attach_callback_std(U16 std_id, void (*func)());
void attach_callback_cmd(GCAN_COMMAND_ID cmd_id, void (*func)(MODULE_ID, U8, U8, U8, U8));
```

When the specified STD ID or command ID is received, the corresponding callback function will be triggered.

STD ID callbacks have no parameters. `std_id` must be a configured group ID.

Command callbacks take the source module's ID and up to four arbitrary arguments.

### Routing

Including `#define ENABLE_ROUTING` in `GopherCAN_config.h` enables message routing. When an EXT ID message is received on a bus different than the bus indicated by `DESTINATION`, the message will be retransmitted on the target bus.

This is useful for passing messages between two modules that are not physically connected to the same bus.

## Example

Below is a minimal example of using `gophercan-lib` in an STM32 project running FreeRTOS. This project is using `example.yaml` and one CAN peripheral in loopback mode.

```c
#include "GopherCAN.h"
#include "GopherCAN_network.h"

extern CAN_HandleTypeDef hcan1; // configured in loopback mode

void on_rpm() {
	// do some stuff with engine RPM ...
	return;
}

void on_set_time(MODULE_ID source, U8 hour, U8 min, U8 sec, U8 _) {
	// do some stuff with the time ...
	return;
}

void setup() {
	printf("initializing...\n");

	if (init_can(&hcan1, GCAN0))
		fault();

	attach_callback_std(engineRPM.info.GROUP_ID, on_rpm);
	attach_callback_cmd(SET_TIME, on_set_time);
}

// called in RTOS task
void loop() {
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

	send_parameter(ENGINERPM_ID);
	send_can_command(PRIO_LOW, THIS_MODULE_ID, SET_TIME, 1, 2, 3, 4);

	osDelay(1000);
}

// called in RTOS task
void service_can() {
	service_can_tx(&hcan1);
	service_can_rx_buffer();
	osDelay(1);
}

void fault() {
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
    HAL_Delay(5000);
    NVIC_SystemReset();
}
```