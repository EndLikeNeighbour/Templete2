#ifndef __X9C_H
#define __X9C_H

#include "stm32f10x.h"

#define X9C_UD_PIN     GPIO_Pin_3
#define X9C_CS_PIN     GPIO_Pin_5
#define X9C_CLK_PIN    GPIO_Pin_6
#define X9C_PORT       GPIOA

#define X9C_MAX_RESISTANCE    10000  // 10kΩ
#define X9C_MIN_RESISTANCE    0      // 0Ω
#define X9C_WIPER_STEPS       100    // 100个抽头

void X9C_Init(void);
void X9C_SetResistance(uint16_t resistance);
void X9C_Increase(uint16_t delta_resistance);
void X9C_Decrease(uint16_t delta_resistance);
uint16_t X9C_GetResistance(void);

#endif
