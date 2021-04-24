// GopherCAN_config.h
//  TODO DOCS

#ifndef GOPHERCAN_CONFIG_H
#define GOPHERCAN_CONFIG_H

#define F0XX 0
#define F4XX 4
#define F7XX 7

// Begin Configuration defines. These are to be modified by the module specific developer

#define TARGET F0XX
//#define TARGET F7XX

// RX and TX buffer sizes. These values should be chosen by the devs
#define RX_BUFFER_SIZE 32
#define TX_BUFFER_SIZE 32

// Note some initialization is different for multi-bus. Check GopherCAN_router_example.c for details
//#define MULTI_BUS

#ifdef MULTI_BUS
#define CAN_ROUTER

// up to 3 busses are supported. That is the most available in the STM32 series
#define NUM_OF_BUSSES 2
#endif

#endif


// End of GopherCAN_config.h
