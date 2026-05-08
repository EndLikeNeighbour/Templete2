#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f10x.h"

static inline void Delay_ms(uint32_t n)
{
    for(uint32_t i = 0; i < n * 10000; i++);
}

#endif
