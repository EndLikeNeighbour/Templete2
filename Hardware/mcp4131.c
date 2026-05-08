#include "mcp4131.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define MCP4131_CS_PIN    GPIO_Pin_4
#define MCP4131_CLK_PIN   GPIO_Pin_5
#define MCP4131_DI_PIN    GPIO_Pin_6
#define MCP4131_PORT      GPIOA

static uint8_t current_level = 64;

void MCP4131_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin = MCP4131_CS_PIN | MCP4131_CLK_PIN | MCP4131_DI_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MCP4131_PORT, &GPIO_InitStructure);
    
    GPIO_SetBits(MCP4131_PORT, MCP4131_CS_PIN);
    GPIO_ResetBits(MCP4131_PORT, MCP4131_CLK_PIN);
    GPIO_ResetBits(MCP4131_PORT, MCP4131_DI_PIN);
    
    MCP4131_SetLevel(64);
}

static void MCP4131_WriteByte(uint8_t data)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        if(data & 0x80)
            GPIO_SetBits(MCP4131_PORT, MCP4131_DI_PIN);
        else
            GPIO_ResetBits(MCP4131_PORT, MCP4131_DI_PIN);
        
        GPIO_SetBits(MCP4131_PORT, MCP4131_CLK_PIN);
        data <<= 1;
        GPIO_ResetBits(MCP4131_PORT, MCP4131_CLK_PIN);
    }
}

void MCP4131_SetLevel(uint8_t level)
{
    if(level > 127) level = 127;
    if(level < 1) level = 1;
    
    current_level = level;
    
    GPIO_ResetBits(MCP4131_PORT, MCP4131_CS_PIN);
    MCP4131_WriteByte(0x00);
    MCP4131_WriteByte(level);
    GPIO_SetBits(MCP4131_PORT, MCP4131_CS_PIN);
}

void MCP4131_Increase(void)
{
    if(current_level < 127)
        MCP4131_SetLevel(current_level + 13);
}

void MCP4131_Decrease(void)
{
    if(current_level > 13)
        MCP4131_SetLevel(current_level - 13);
}

uint8_t MCP4131_GetLevel(void)
{
    return current_level;
}

uint8_t MCP4131_GetVoltageVpp(void)
{
    return (current_level * 10) / 127;
}
