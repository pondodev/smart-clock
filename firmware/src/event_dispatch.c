#include "event_dispatch.h"
#include "pico/time.h"

typedef struct TimerInfo {
    EventDispatchTimerCallback  cb;
    void*                       args;
    uint64_t                    delay_us;
    uint64_t                    time_remaining_us;
} TimerInfo;

#define MAX_TIMERS 10

static int                          s_running           = 0;
static EventDispatchMainCallback    s_main_cb           = NULL;
static TimerInfo*                   s_timer_info_buffer = NULL;
static absolute_time_t              s_last_timer_update = 0;

static inline void _process_timers(void);

void event_dispatch_init(EventDispatchMainCallback main_cb) {
    if (main_cb == NULL) {
        printf("event dispatch: main_cb cannot be NULL\n");
        return;
    }

    s_main_cb = main_cb;
    size_t timer_buffer_size = sizeof(TimerInfo)*MAX_TIMERS;
    s_timer_info_buffer = malloc(timer_buffer_size);
    memset(s_timer_info_buffer, 0, timer_buffer_size);
}

int event_dispatch_run(void) {
    if (s_running || s_main_cb == NULL)
        return 0;

    s_last_timer_update = get_absolute_time();

    s_running = 1;
    while (s_running) {
        s_main_cb();
        _process_timers();
    }

    return 1;
}

void event_dispatch_stop(void) {
    s_running = 0;
    free(s_timer_info_buffer);
    s_timer_info_buffer = NULL;
}

TimerID install_timer(EventDispatchTimerCallback cb, void* args, uint32_t delay_ms) {
    if (cb == NULL)
        return INVALID_TIMER_ID;

    TimerID id = 0;
    TimerInfo* timer = NULL;
    while (id != MAX_TIMERS) {
        if (s_timer_info_buffer[id].cb == NULL) {
            timer = &s_timer_info_buffer[id];
            break;
        }

        ++id;
    }

    if (timer == NULL)
        return INVALID_TIMER_ID;

    *timer = (TimerInfo) {
        .cb                 = cb,
        .args               = args,
        .delay_us           = delay_ms*1000,
        .time_remaining_us  = delay_ms*1000,
    };

    return id;
}

void remove_timer(TimerID timer_id) {
    if (timer_id == INVALID_TIMER_ID || timer_id >= MAX_TIMERS)
        return;

    memset(&s_timer_info_buffer[timer_id], 0, sizeof(TimerInfo));
}

static inline void _process_timers(void) {
    const absolute_time_t current_time = get_absolute_time();
    const uint64_t time_delta = current_time - s_last_timer_update;

    for (TimerID id = 0; id < MAX_TIMERS; ++id) {
        TimerInfo* timer = &s_timer_info_buffer[id];
        if (timer->cb == NULL)
            continue;

        if (timer->time_remaining_us < time_delta)
            timer->time_remaining_us = 0;
        else
            timer->time_remaining_us -= time_delta;

        if (timer->time_remaining_us == 0) {
            timer->cb(timer->args);
            timer->time_remaining_us = timer->delay_us;
        }
    }

    s_last_timer_update = current_time;
}

