#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"

#define OLED_I2C_ADDR 0x78

void OLED_Init(void);
void OLED_Clear(void);
void OLED_Refresh(void);
void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch);
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index);

#endif
