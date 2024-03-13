#include "piezo_buzzer.h"

#include "../pin_definitions.h"

#include "pico/stdlib.h"
#include "hardware/pwm.h"

static uint s_pin       = 0;
static uint s_slice_num = 0;

int piezo_buzzer_init(uint pin) {
    // TODO: pwm
    s_pin = pin;
    gpio_init(s_pin);
    gpio_set_dir(s_pin, GPIO_OUT);
    gpio_put(s_pin, 0);
}

void piezo_buzzer_beep(void) {
    // TODO
}

