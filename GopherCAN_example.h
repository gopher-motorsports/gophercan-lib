#ifndef GOPHERCAN_EXAMPLE_H
#define GOPHERCAN_EXAMPLE_H

// some defines used for examples
#define MIN_ON_RPM 100
#define FAN_CURRENT_UPDATE_TIME 1000

void init();
void background_loop();
void main_loop();
//void change_led_state(void* parameter, U8 remote_param);

#endif
