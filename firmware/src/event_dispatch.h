#ifndef EVENT_DISPATCH_H
#define EVENT_DISPATCH_H

#include "pico/stdlib.h"

typedef void (*EventDispatchMainCallback)(void);
typedef void (*EventDispatchTimerCallback)(void* args);

#define INVALID_TIMER_ID 0xFF
typedef uint8_t TimerID;

void event_dispatch_init(EventDispatchMainCallback main_cb);
int event_dispatch_run(void);
void event_dispatch_stop(void);
TimerID install_timer(EventDispatchTimerCallback cb, void* args, uint32_t delay_ms);
void remove_timer(TimerID timer_id);

#endif

