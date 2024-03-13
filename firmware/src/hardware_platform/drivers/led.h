#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"

#define INVALID_LED_ID 0xFF
typedef uint8_t LedID;

int led_init(uint pin, LedID* led_id_out);
void led_set_state(LedID led_id, int on);

#endif

