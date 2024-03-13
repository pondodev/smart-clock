#include "led.h"

#include <string.h>

typedef struct LedInfo {
    int     active;
    uint    pin;
} LedInfo;

#define MAX_LEDS 10
static LedInfo* s_led_info_buffer = NULL;

static void _init_led_info_buffer(void);

int led_init(uint pin, LedID* led_id_out) {
    if (s_led_info_buffer == NULL)
        _init_led_info_buffer();

    LedInfo* led = NULL;
    for (LedID i = 0; i < MAX_LEDS; ++i) {
        if (s_led_info_buffer[i].active)
            continue;

        led = &s_led_info_buffer[i];
        *led_id_out = i;
        break;
    }

    if (led == NULL)
        return INVALID_LED_ID;

    led->active = 1;
    led->pin    = pin;

    gpio_init(led->pin);
    gpio_set_dir(led->pin, GPIO_OUT);
    gpio_put(led->pin, 0);

    return 1;
}

void led_set_state(LedID led_id, int on) {
    gpio_put(s_led_info_buffer[led_id].pin, on);
}

static void _init_led_info_buffer(void) {
    const size_t led_buf_size = sizeof(LedInfo)*MAX_LEDS;
    s_led_info_buffer = malloc(led_buf_size);
    memset(s_led_info_buffer, 0, led_buf_size);
}

