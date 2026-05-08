#ifndef __MCP4131_H
#define __MCP4131_H

#include "stm32f10x.h"

void MCP4131_Init(void);
void MCP4131_SetLevel(uint8_t level);
void MCP4131_Increase(void);
void MCP4131_Decrease(void);
uint8_t MCP4131_GetLevel(void);
uint8_t MCP4131_GetVoltageVpp(void);

#endif
