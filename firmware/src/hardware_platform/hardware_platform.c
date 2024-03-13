#include "hardware_platform.h"

#include "pin_definitions.h"
#include "drivers/led.h"
#include "drivers/dht22.h"
#include "drivers/piezo_buzzer.h"

#include "pico/stdio.h"
#include "pico/stdlib.h"

static LedID s_debug_led = INVALID_LED_ID;

int hardware_platform_init(void) {
    printf("initialising hardware platform...\n");
    stdio_init_all();

    if (! led_init(PIN_DEBUG_LED, &s_debug_led)) {
        printf("failed to init debug led\n");
        return 0;
    }

    if (! dht22_init(PIN_DHT22_DATA)) {
        printf("failed to init dht22\n");
        return 0;
    }

    if (! piezo_buzzer_init(PIN_BUZZER)) {
        printf("failed to init piezo buzzer\n");
        return 0;
    }

    printf("done!\n");
    return 1;
}

void set_debug_led(int on) {
    led_set_state(s_debug_led, on);
}

void poll_temp(void) {
    dht22_poll();
}

float get_temp_celsius(void) {
    return dht22_get_data()->temp_celsius;
}

float get_relative_humidity(void) {
    return dht22_get_data()->relative_humidity;
}

