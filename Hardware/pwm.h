#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

void PWM_Init(uint16_t freq, uint16_t amp_mv);
void PWM_SetFrequency(uint16_t freq);
void PWM_SetAmplitude(uint16_t amp_mv);
uint16_t PWM_GetFrequency(void);

#endif