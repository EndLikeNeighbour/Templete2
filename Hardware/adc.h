#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"

void MYADC_Init(void);
float MYADC_GetVoltage(void);
float MYADC_GetConvertedVoltage(void);
float MYADC_GetVpp(void);

#endif