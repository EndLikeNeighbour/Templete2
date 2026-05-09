#ifndef __FREQMETER_H
#define __FREQMETER_H

#include "stm32f10x.h"

void FreqMeter_Init(void);
uint16_t FreqMeter_GetFrequency(void);

#endif
