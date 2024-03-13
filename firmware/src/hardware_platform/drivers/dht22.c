#include "dht22.h"

static uint         s_data_pin = 0;
static DHT22_Data   s_data_cache;

static inline int _wait_for_state(int state, uint16_t timeout_us, uint16_t* time_waited_us_out);
static inline int _check_timing(uint16_t time_us, uint16_t target_us);
static inline int _read_byte(uint8_t* byte_out);

int dht22_init(uint data_pin) {
    s_data_pin = data_pin;
    gpio_init(s_data_pin);
    gpio_set_dir(s_data_pin, GPIO_OUT);
    gpio_put(s_data_pin, 1);

    s_data_cache = (DHT22_Data) {
        .temp_celsius       = 0.f,
        .relative_humidity  = 0.f,
    };

    return 1;
}

// TODO: try implementing this in pio?
void dht22_poll(void) {
    // signal start data transfer
    gpio_put(s_data_pin, 0);
    sleep_ms(2);

    // wait for ack
    gpio_put(s_data_pin, 1);
    sleep_us(40);
    gpio_set_dir(s_data_pin, GPIO_IN);

    // TODO: clean up magic numbers

    // wait until start of ack
    if (! _wait_for_state(0, 80, NULL)) {
        printf("timeout while waiting on ACK\n");
        goto bail;
    }

    // ack will be 80us low then 80us high
    uint16_t time_slept_us;
    if (! _wait_for_state(1, 100, &time_slept_us)) {
        printf("timeout while waiting on ACK signal to go from low to high\n");
        goto bail;
    }

    if (! _wait_for_state(0, 100, &time_slept_us)) {
        printf("timeout while waiting on ACK signal to end\n");
        goto bail;
    }

    // now we can start reading in the data
    uint8_t data[5];
    for (size_t i = 0; i < 5; ++i) {
        if (! _read_byte(&data[i])) {
            printf("failed to read byte %zu\n", i);
            goto bail;
        }
    }

    const uint8_t sum = data[0]+data[1]+data[2]+data[3];
    if (sum != data[4]) {
        printf("checksum failed (expected 0x%02X, got 0x%02X)\n", sum, data[4]);
        goto bail;
    }

    // TODO: is this correct?
    uint16_t* relative_humidity     = (uint16_t*)&data[0];
    uint16_t* temp_celsius          = (uint16_t*)&data[2];
    s_data_cache.relative_humidity  = (float)*relative_humidity / 10.f;
    s_data_cache.temp_celsius       = (float)*temp_celsius / 10.f;

bail:
    gpio_set_dir(s_data_pin, GPIO_OUT);
    gpio_put(s_data_pin, 1);
    return;
}

const DHT22_Data* dht22_get_data(void) {
    return &s_data_cache;
}

static inline int _wait_for_state(int state, uint16_t timeout_us, uint16_t* time_waited_us_out) {
    uint16_t time_waited_us = 0;
    while (gpio_get(s_data_pin) != state && time_waited_us < timeout_us) {
        sleep_us(1);
        ++time_waited_us;
    }

    if (time_waited_us_out != NULL)
        *time_waited_us_out = time_waited_us;

    return time_waited_us != timeout_us;
}

static inline int _check_timing(uint16_t time_us, uint16_t target_us) {
    const uint16_t range        = 10;
    const uint16_t lower_bound  = target_us - range;
    const uint16_t upper_bound  = target_us + range;

    return time_us >= lower_bound && time_us <= upper_bound;
}

static inline int _read_byte(uint8_t* byte_out) {
    if (byte_out == NULL)
        return 0;

    // the data line should be low at this point, but just in case
    if (! _wait_for_state(0, 20, NULL)) {
        printf("timeout wait for low\n");
        return 0;
    }

    *byte_out = 0;
    for (size_t i = 0; i < 8; ++i) {
        uint16_t time_waited_us;
        // start of a bit is denoted by the IC pulling the line low for 50us
        if (! _wait_for_state(1, 70, &time_waited_us)) {
            printf("timeout wait for high\n");
            return 0;
        }

        // then we need to measure how long the line is held high for. 70us for
        // a 1, 28us for a 0
        if (! _wait_for_state(0, 90, &time_waited_us))
            return 0;

        const uint8_t bit = _check_timing(time_waited_us, 70) ? 1 : 0;
        *byte_out <<= 1;
        *byte_out |= bit;
    }

    return 1;
}

