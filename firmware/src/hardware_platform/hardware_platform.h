#ifndef HARDWARE_PLATFORM_H
#define HARDWARE_PLATFORM_H

int hardware_platform_init(void);

void set_debug_led(int on);
void poll_temp(void);
float get_temp_celsius(void);
float get_relative_humidity(void);

#endif

