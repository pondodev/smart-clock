#include "event_dispatch.h"
#include "hardware_platform/hardware_platform.h"

#include "pico/stdlib.h"

static void _main_loop(void);
static void _poll_debug_led(void* args);
static void _poll_temp(void* args);

int main(void) {
    if (! hardware_platform_init()) {
        printf("failed to initialise hardware\n");
        return 1;
    }

    event_dispatch_init(_main_loop);

    install_timer(_poll_debug_led, NULL, 500);
    install_timer(_poll_temp, NULL, 2000);

    event_dispatch_run();

    return 0;
}

static void _main_loop(void) {
    // TODO
    sleep_us(1);
}

static void _poll_debug_led(void* args) {
    (void)args;

    static int on = 0;
    on = ! on;
    set_debug_led(on);
}

static void _poll_temp(void* args) {
    (void)args;

    poll_temp();
    printf("temp: %.1fC humidity %.1f\%\n", get_temp_celsius(), get_relative_humidity());
}

