#include "stm32l0xx.h"

#ifndef __RADIO_TIMERS_H
#define __RADIO_TIMERS_H

void systick_delay_init(void);
void systick_delay_ms(uint32_t duration);

#endif
