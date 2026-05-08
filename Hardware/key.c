#include "key.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

void Key_DelayMs(uint32_t n)
{
    for(uint32_t i = 0; i < n * 5000; i++);
}

void KEY_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

key_state KEY_Scan(void)
{
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET)
    {
        Key_DelayMs(5);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == RESET);
        Key_DelayMs(5);
        return KEY_AMP_UP;
    }

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == RESET)
    {
        Key_DelayMs(5);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9) == RESET);
        Key_DelayMs(5);
        return KEY_AMP_DOWN;
    }

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == RESET)
    {
        Key_DelayMs(5);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) == RESET);
        Key_DelayMs(5);
        return KEY_FREQ_UP;
    }

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == RESET)
    {
        Key_DelayMs(5);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11) == RESET);
        Key_DelayMs(5);
        return KEY_FREQ_DOWN;
    }

    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == RESET)
    {
        Key_DelayMs(5);
        while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == RESET);
        Key_DelayMs(5);
        return KEY_MEASURE;
    }

    return KEY_NONE;
}