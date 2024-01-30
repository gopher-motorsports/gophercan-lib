# GopherCAN

This repository holds the source code for the GopherCAN library designed for bxCAN on STM32 microcontrollers.

To see an example of GopherCAN running in a project, look into the 'gcan-boiler-plate' repository

The primary functionality of the library is contained in 'GopherCAN.c' and 'GopherCAN.h'

## Library Configuration

Projects using the GopherCAN library should include a 'GopherCAN_config.h' file:

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

## Network Configuration

`network_autogen/` is used to auto-generate `GopherCAN_network.h/c`.

Run `python autogen.py configs/[filename].yaml` to generate the network files.