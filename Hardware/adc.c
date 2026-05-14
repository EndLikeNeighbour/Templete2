#include "adc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

#define ADC_SAMPLE_COUNT      16    // 采样点数
#define ADC_FILTER_WEIGHT     15    // 滑动平均权重

static uint32_t adc_filtered_value = 0;
static uint16_t vpp_max_value = 0;
static uint16_t vpp_min_value = 4095;

void MYADC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    for(uint8_t i = 0; i < 64; i++)
    {
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        adc_filtered_value = ADC_GetConversionValue(ADC1);
        ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    }
}

static uint16_t get_median(uint16_t *data, uint8_t size)
{
    uint8_t i, j;
    uint16_t temp;
    
    for(i = 0; i < size - 1; i++)
    {
        for(j = 0; j < size - i - 1; j++)
        {
            if(data[j] > data[j + 1])
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
    
    return data[size / 2];
}

static uint16_t ADC_ReadChannel(uint8_t channel)
{
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_239Cycles5);
    
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    
    return ADC_GetConversionValue(ADC1);
}

float MYADC_GetVoltage(void)
{
    uint16_t samples[ADC_SAMPLE_COUNT];
    uint32_t sum = 0;
    
    for(uint8_t i = 0; i < ADC_SAMPLE_COUNT; i++)
    {
        samples[i] = ADC_ReadChannel(ADC_Channel_1);
        sum += samples[i];
    }
    
    uint16_t median = get_median(samples, ADC_SAMPLE_COUNT);
    uint16_t average = sum / ADC_SAMPLE_COUNT;
    
    uint16_t adc_raw = (median * 3 + average * 1) / 4;
    
    adc_filtered_value = (adc_filtered_value * ADC_FILTER_WEIGHT + adc_raw * 1) / (ADC_FILTER_WEIGHT + 1);
    
    return (adc_filtered_value * 3.3f) / 4095.0f;
}

float MYADC_GetConvertedVoltage(void)
{
    float vin = MYADC_GetVoltage();
    return 0.3f * vin + 0.65f;
}

float MYADC_GetVpp(void)
{
    uint16_t samples[32];
    
    vpp_max_value = 0;
    vpp_min_value = 4095;
    
    for(uint8_t i = 0; i < 32; i++)
    {
        samples[i] = ADC_ReadChannel(ADC_Channel_8);
        
        if(samples[i] > vpp_max_value) vpp_max_value = samples[i];
        if(samples[i] < vpp_min_value) vpp_min_value = samples[i];
    }
    
    float vpp_adc = (vpp_max_value - vpp_min_value) * 3.3f / 4095.0f;
    float vin = (vpp_adc - 1.65f) / 0.3f * 2.0f;
    
    return vin;
}