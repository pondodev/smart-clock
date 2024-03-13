#ifndef PIEZO_BUZZER_H
#define PIEZO_BUZZER_H

#include "pico/stdlib.h"

int piezo_buzzer_init(uint pin);
void piezo_buzzer_beep(void);

#endif

