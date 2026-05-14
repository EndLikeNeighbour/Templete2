#include "x9c.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

static uint16_t current_resistance = 5500;  // 初始电阻5.5kΩ

void X9C_DelayUs(uint32_t n)
{
    for(uint32_t i = 0; i < n * 8; i++);
}

void X9C_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = X9C_UD_PIN | X9C_CS_PIN | X9C_CLK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(X9C_PORT, &GPIO_InitStructure);

    GPIO_SetBits(X9C_PORT, X9C_CS_PIN);
    GPIO_SetBits(X9C_PORT, X9C_UD_PIN);
    GPIO_ResetBits(X9C_PORT, X9C_CLK_PIN);

    X9C_SetResistance(5500);
}

void X9C_SetResistance(uint16_t resistance)
{
    if(resistance > X9C_MAX_RESISTANCE)
        resistance = X9C_MAX_RESISTANCE;
    if(resistance < X9C_MIN_RESISTANCE)
        resistance = X9C_MIN_RESISTANCE;

    uint16_t target_resistance = resistance;
    uint16_t delta = target_resistance > current_resistance ?
                     (target_resistance - current_resistance) :
                     (current_resistance - target_resistance);

    uint8_t steps = (uint8_t)(delta / (X9C_MAX_RESISTANCE / X9C_WIPER_STEPS));
    if(steps == 0 && delta > 0) steps = 1;

    if(target_resistance > current_resistance)
    {
        GPIO_SetBits(X9C_PORT, X9C_UD_PIN);
    }
    else
    {
        GPIO_ResetBits(X9C_PORT, X9C_UD_PIN);
    }

    GPIO_ResetBits(X9C_PORT, X9C_CS_PIN);
    X9C_DelayUs(10);

    for(uint8_t i = 0; i < steps; i++)
    {
        GPIO_SetBits(X9C_PORT, X9C_CLK_PIN);
        X9C_DelayUs(10);
        GPIO_ResetBits(X9C_PORT, X9C_CLK_PIN);
        X9C_DelayUs(10);
    }

    GPIO_SetBits(X9C_PORT, X9C_CS_PIN);
    X9C_DelayUs(10);

    current_resistance = target_resistance;
}

void X9C_Increase(uint16_t delta_resistance)
{
    if(current_resistance + delta_resistance <= X9C_MAX_RESISTANCE)
    {
        GPIO_SetBits(X9C_PORT, X9C_UD_PIN);

        GPIO_ResetBits(X9C_PORT, X9C_CS_PIN);
        X9C_DelayUs(10);

        uint8_t steps = (uint8_t)(delta_resistance / (X9C_MAX_RESISTANCE / X9C_WIPER_STEPS));
        if(steps == 0 && delta_resistance > 0) steps = 1;

        for(uint8_t i = 0; i < steps; i++)
        {
            GPIO_SetBits(X9C_PORT, X9C_CLK_PIN);
            X9C_DelayUs(10);
            GPIO_ResetBits(X9C_PORT, X9C_CLK_PIN);
            X9C_DelayUs(10);
        }

        GPIO_SetBits(X9C_PORT, X9C_CS_PIN);
        X9C_DelayUs(10);

        current_resistance += delta_resistance;
    }
}

void X9C_Decrease(uint16_t delta_resistance)
{
    if(current_resistance >= delta_resistance)
    {
        GPIO_ResetBits(X9C_PORT, X9C_UD_PIN);

        GPIO_ResetBits(X9C_PORT, X9C_CS_PIN);
        X9C_DelayUs(10);

        uint8_t steps = (uint8_t)(delta_resistance / (X9C_MAX_RESISTANCE / X9C_WIPER_STEPS));
        if(steps == 0 && delta_resistance > 0) steps = 1;

        for(uint8_t i = 0; i < steps; i++)
        {
            GPIO_SetBits(X9C_PORT, X9C_CLK_PIN);
            X9C_DelayUs(10);
            GPIO_ResetBits(X9C_PORT, X9C_CLK_PIN);
            X9C_DelayUs(10);
        }

        GPIO_SetBits(X9C_PORT, X9C_CS_PIN);
        X9C_DelayUs(10);

        current_resistance -= delta_resistance;
    }
}

uint16_t X9C_GetResistance(void)
{
    return current_resistance;
}
