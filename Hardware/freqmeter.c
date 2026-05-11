#include "freqmeter.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"

void FreqMeter_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* PA7 as TIM3_CH2 input (floating) */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* TIM3: counter at 1MHz (72MHz / 72) */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* CH2 input capture, rising edge */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

uint16_t FreqMeter_GetFrequency(void)
{
    uint16_t cap1, cap2;
    uint16_t period;

    /* 等第一个上升沿（超时 25ms） */
    TIM_ClearFlag(TIM3, TIM_FLAG_CC2);
    {
        uint16_t start = TIM3->CNT;
        while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_CC2))
        {
            if((uint16_t)(TIM3->CNT - start) > 25000)
                return 0;  /* 无信号 */
        }
    }
    cap1 = TIM_GetCapture2(TIM3);
    TIM_ClearFlag(TIM3, TIM_FLAG_CC2);

    /* 等第二个上升沿（超时 25ms） */
    {
        uint16_t start = TIM3->CNT;
        while(!TIM_GetFlagStatus(TIM3, TIM_FLAG_CC2))
        {
            if((uint16_t)(TIM3->CNT - start) > 25000)
                return 0;
        }
    }
    cap2 = TIM_GetCapture2(TIM3);
    TIM_ClearFlag(TIM3, TIM_FLAG_CC2);

    period = cap2 - cap1;
    if(period > 0)
        return 1000000 / period;
    return 0;
}
