#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "delay.h"
#include "font_index.h"
#include "pwm.h"
#include "x9c.h"
#include "adc.h"
#include "freqmeter.h"

uint16_t current_amp = 300;

int main(void)
{
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
	
		uint16_t cnt_total = 0;
    uint16_t freq;
    uint16_t meas_freq;
    float voltage;
    uint16_t adc_int;
    uint8_t adc_update_counter = 0;
    float current_vpp = 0.0f;
    uint16_t current_resistance = 5500;

    OLED_Init();
    KEY_Init();
    PWM_Init(1000, 1650);
    X9C_Init();
    MYADC_Init();
    FreqMeter_Init();

    freq    = PWM_GetFrequency();
    meas_freq = FreqMeter_GetFrequency();

    /* ── 初始显示 ── */
    OLED_Clear();

    /* y=0: 幅值：xxV (Vpp:x.xx) */
    OLED_ShowChinese(0, 0, CH_FU);
    OLED_ShowChinese(16, 0, CH_ZHI);
    OLED_ShowChinese(32, 0, CH_COLON);
		OLED_ShowNum(48, 0, current_amp / 100, 1);
    OLED_ShowChar(56, 0, '.');
    OLED_ShowNum(64, 0, current_amp % 100, 2);
    OLED_ShowString(72, 0, "V");
    current_vpp = MYADC_GetVpp();
    uint16_t vpp_int = (uint16_t)(current_vpp * 100.0f + 0.5f);
    OLED_ShowString(80, 0, "(");
    OLED_ShowNum(88, 0, vpp_int / 100, 2);
    OLED_ShowChar(104, 0, '.');
    OLED_ShowNum(112, 0, (vpp_int % 100) / 10, 1);
    OLED_ShowString(120, 0, ")");
		
    /* y=2: 频率：xxxxHz */
    OLED_ShowChinese(0, 2, CH_PIN);
    OLED_ShowChinese(16, 2, CH_LV);
    OLED_ShowChinese(32, 2, CH_COLON);
    OLED_ShowNum(48, 2, meas_freq, 4);
    OLED_ShowString(80, 2, "Hz");

    /* y=4: 直流电压：x.xxV */
    OLED_ShowChinese(0, 4, CH_ZHI2);
    OLED_ShowChinese(16, 4, CH_LIU);
    OLED_ShowChinese(32, 4, CH_DIAN);
    OLED_ShowChinese(48, 4, CH_YA);
    OLED_ShowChinese(64, 4, CH_COLON);
    voltage = MYADC_GetVoltage();
    adc_int = (uint16_t)(voltage * 100.0f + 0.5f);
    OLED_ShowNum(80, 4, adc_int / 100, 1);
    OLED_ShowChar(88, 4, '.');
    OLED_ShowNum(96, 4, adc_int % 100, 2);
    OLED_ShowString(112, 4, "V");

    /* y=6: 按键次数：xxx */
    OLED_ShowChinese(0, 6, CH_AN);
    OLED_ShowChinese(16, 6, CH_JIAN);
    OLED_ShowChinese(32, 6, CH_CI);
    OLED_ShowChinese(48, 6, CH_SHU);
    OLED_ShowChinese(64, 6, CH_COLON);
    OLED_ShowNum(80, 6, cnt_total, 3);

    OLED_Refresh();

    /* ── 主循环 ── */
    while(1)
    {
        /* 延时 500ms，分段检测按键（每 10ms 扫一次） */
        for(uint8_t i = 0; i < 50; i++)
        {
            key_state key = KEY_Scan();
						uint8_t updated = 0;

            if(key != KEY_NONE)
            {
                switch(key)
                {
                    case KEY_AMP_UP:
											if(current_amp <= 450 && current_resistance <= 9100)
                      {
                        current_amp += 50;  // 每次加0.5V
                        current_resistance += 900;  // 每次加0.9kΩ
                        X9C_Increase(900);
                        
                        OLED_ShowNum(48, 0, current_amp / 100, 1);
                        OLED_ShowChar(56, 0, '.');
                        OLED_ShowNum(64, 0, current_amp % 100, 2);
                        OLED_ShowString(72, 0, "V");
                        
                        OLED_ShowNum(48, 4, current_resistance / 1000, 1);
                        OLED_ShowChar(56, 4, '.');
                        OLED_ShowNum(64, 4, (current_resistance % 1000) / 100, 1);
                        OLED_ShowString(72, 4, "k");
                        OLED_ShowString(80, 4, "Ohm");
                        
                        OLED_Refresh();
                        updated = 1;
                      }
                      cnt_total++;
                        break;

                    case KEY_AMP_DOWN:
											if(current_amp >= 100 && current_resistance >= 900)
                      {
                        current_amp -= 50;  // 每次减0.5V
                        current_resistance -= 900;  // 每次减0.9kΩ
                        X9C_Decrease(900);
                        
                        OLED_ShowNum(48, 0, current_amp / 100, 1);
                        OLED_ShowChar(56, 0, '.');
                        OLED_ShowNum(64, 0, current_amp % 100, 2);
                        OLED_ShowString(72, 0, "V");
                        
                        OLED_ShowNum(48, 4, current_resistance / 1000, 1);
                        OLED_ShowChar(56, 4, '.');
                        OLED_ShowNum(64, 4, (current_resistance % 1000) / 100, 1);
                        OLED_ShowString(72, 4, "k");
                        OLED_ShowString(80, 4, "Ohm");
                        
                        OLED_Refresh();
                        updated = 1;
                      }
                        cnt_total++;
                        break;

                    case KEY_FREQ_UP:
                        if(freq < 1000)
                        {
                            freq += 100;
                            PWM_SetFrequency(freq);
                        }
                        cnt_total++;
                        break;

                    case KEY_FREQ_DOWN:
                        if(freq > 100)
                        {
                            freq -= 100;
                            PWM_SetFrequency(freq);
                        }
                        cnt_total++;
                        break;

                    case KEY_MEASURE:
                        cnt_total++;
                        break;

                    default:
                        break;
                }
                 /* 有按键 → 提前退出延时循环，立即刷新 */
								break;
            }

            Delay_ms(5);
        }

        /* 读取当前值（不依赖按键） */
        freq      = PWM_GetFrequency();
        meas_freq = FreqMeter_GetFrequency();
        
        adc_update_counter++;
        if(adc_update_counter >= 3)
        {
            adc_update_counter = 0;
            voltage   = MYADC_GetVoltage();
            current_vpp = MYADC_GetVpp();
            adc_int = (uint16_t)(voltage * 100.0f + 0.5f);
        }

        /* ── 刷新 OLED ── */
        OLED_Clear();

        /* y=0: 幅值：xxV (Vpp:x.xx) */
        OLED_ShowChinese(0, 0, CH_FU);
        OLED_ShowChinese(16, 0, CH_ZHI);
        OLED_ShowChinese(32, 0, CH_COLON);
        OLED_ShowNum(48, 0, current_amp / 100, 1);
				OLED_ShowChar(56, 0, '.');
				OLED_ShowNum(64, 0, current_amp % 100, 2);
				OLED_ShowString(72, 0, "V");
        uint16_t vpp_int = (uint16_t)(current_vpp * 100.0f + 0.5f);
        OLED_ShowString(80, 0, "(");
        OLED_ShowNum(88, 0, vpp_int / 100, 2);
        OLED_ShowChar(104, 0, '.');
        OLED_ShowNum(112, 0, (vpp_int % 100) / 10, 1);
        OLED_ShowString(120, 0, ")");

        /* y=2: 频率：xxxxHz */
        OLED_ShowChinese(0, 2, CH_PIN);
        OLED_ShowChinese(16, 2, CH_LV);
        OLED_ShowChinese(32, 2, CH_COLON);
        OLED_ShowNum(48, 2, meas_freq, 4);
        OLED_ShowString(80, 2, "Hz");

        /* y=4: 直流电压：x.xxV */
        OLED_ShowChinese(0, 4, CH_ZHI2);
        OLED_ShowChinese(16, 4, CH_LIU);
        OLED_ShowChinese(32, 4, CH_DIAN);
        OLED_ShowChinese(48, 4, CH_YA);
        OLED_ShowChinese(64, 4, CH_COLON);
        OLED_ShowNum(80, 4, adc_int / 100, 1);
        OLED_ShowChar(88, 4, '.');
        OLED_ShowNum(96, 4, adc_int % 100, 2);
        OLED_ShowString(112, 4, "V");

        /* y=6: 按键次数：xxx */
        OLED_ShowChinese(0, 6, CH_AN);
        OLED_ShowChinese(16, 6, CH_JIAN);
        OLED_ShowChinese(32, 6, CH_CI);
        OLED_ShowChinese(48, 6, CH_SHU);
        OLED_ShowChinese(64, 6, CH_COLON);
        OLED_ShowNum(80, 6, cnt_total, 3);

        OLED_Refresh();
    }
}
