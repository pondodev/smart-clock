#ifndef DHT22_H
#define DHT22_H

#include "pico/stdlib.h"

typedef struct DHT22_Data {
    float temp_celsius;
    float relative_humidity;
} DHT22_Data;

int dht22_init(uint data_pin);
void dht22_poll(void);
const DHT22_Data* dht22_get_data(void);

#endif

