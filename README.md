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

```
S8 init_can(CAN_HandleTypeDef* hcan, BUS_ID bus_id);
```

This ties the peripheral to a message buffer and `BUS_ID` (defined in the YAML). When a message is sent with a particular destination module (commands, requests, or errors), the bus ID is used to determine which peripheral to send the message to.

### Servicing Buffers

The following functions must be called frequently (e.g. in a 1ms loop):
```
void service_can_tx(CAN_HandleTypeDef* hcan);
S8 service_can_rx_buffer(void);
```

`service_can_tx` moves messages from the transmit buffer of an hcan instance to the CAN mailbox (where it is actually sent).

`service_can_rx_buffer` processes messages in the RX buffer. When a message is received, it is automatically moved from the CAN FIFOs to the RX buffer by an interrupt handler.

### Message Types

GopherCAN supports four fundamental message types: data, requests, commands and errors.

"Sending" a message simply means moving a message into the TX buffer of one or more hcan instances. The message won't be transmitted unless that peripheral is serviced with `service_can_tx`.

#### Data

Data messages use an 11-bit CAN ID. They are composed of groups of parameters and have the following format:

| ID<10:0> | Data<63:0> |
| --- | --- |
| `GROUP_ID` | `PARAM0` `PARAM1` `...` |

The ID of a group and the parameters it contains are defined in a YAML configuration. Groups can be sent either by specifying a parameter (the group it belongs to will be sent) or by specifying the group ID:
```
S8 send_parameter(CAN_INFO_STRUCT* param);
S8 send_group(U16 group_id);
```

Data messages are sent on all configured buses.

#### Requests

Requests use a 29-bit CAN ID. They contain no data.

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> |
| --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR` | `PARAMETER` |

*`ERROR`=0

```
S8 request_parameter(PRIORITY priority, MODULE_ID dest_module, GCAN_PARAM_ID parameter);
```

Requests are sent on the bus corresponding to the provided `MODULE_ID`. When a request is received and processed in the RX buffer, the specified parameter is automatically sent with `send_parameter`.

#### Commands

Commands use a 29-bit CAN ID. The data field supports up to four arguments.

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> | Data<0> | Data<1> | Data<2> | Data<3> | Data<4> | Data<5-7> |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR` | `PARAMETER` | `COMMAND_ID` | `A0` | `A1` | `A2` | `A3` | 0 |

*`ERROR`=0, `PARAMETER`=0

```
S8 send_can_command(PRIORITY priority, MODULE_ID dest_module, GCAN_COMMAND_ID command_id, U8 a0, U8 a1, U8 a2, U8 a3);
```

`A0-3` are arguments to pass along with the command, they are all optional and can have any value.

#### Errors

| ID<28> | ID<27:22> | ID<21:16> | ID<15> | ID<14:0> | Data<0> |
| --- | --- | --- | --- | --- | --- |
| `PRIORITY` | `DESTINATION` | `SOURCE` | `ERROR` | `PARAMETER` | `ERROR_ID` |

*`ERROR`=1

The `PRIORITY` and `PARAMETER` fields will be copied from the message that triggered an error.

### Event Handlers

### Callbacks

Callback functions can be attached to both STD ID messages and commands.

### Routing