#include "encoder.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define ENCODER_A_PIN GPIO_Pin_2
#define ENCODER_B_PIN GPIO_Pin_3
#define ENCODER_PORT GPIOA

static int16_t encoder_count = 0;
static uint8_t last_state = 0;

void Encoder_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = ENCODER_A_PIN | ENCODER_B_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ENCODER_PORT, &GPIO_InitStructure);
    
    last_state = ((GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_A_PIN) << 1) | 
                  GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_B_PIN));
}

int16_t Encoder_GetDelta(void)
{
    uint8_t current_state = ((GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_A_PIN) << 1) | 
                             GPIO_ReadInputDataBit(ENCODER_PORT, ENCODER_B_PIN));
    
    uint8_t transition = (last_state << 2) | current_state;
    
    switch(transition)
    {
        case 0x03: case 0x04: case 0x0B: case 0x0C:
            encoder_count++;
            break;
        case 0x01: case 0x02: case 0x0D: case 0x0E:
            encoder_count--;
            break;
        default:
            break;
    }
    
    last_state = current_state;
    return encoder_count;
}

void Encoder_Reset(void)
{
    encoder_count = 0;
}
