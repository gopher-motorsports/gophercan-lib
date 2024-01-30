# GopherCAN

GopherCAN is a messaging protocol built on top of [CAN](https://en.wikipedia.org/wiki/CAN_bus). It defines what information is packed into the ID and data fields in a CAN frame.

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

### Sending & Receiving Messages

GopherCAN supports four fundamental message types: data, commands, requests, and errors.

Data messages use an 11-bit CAN ID. They are composed of groups of parameters defined in a YAML configuration.

Commands, requests, and errors use a 29-bit CAN ID.

### Event Handlers

### Callbacks

### Routing