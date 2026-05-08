#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "delay.h"
#include "font_index.h"
#include "pwm.h"
#include "mcp4131.h"
#include "adc.h"

int main(void)
{
    uint16_t cnt_total = 0;
    uint8_t level;
    uint16_t freq;
    float voltage;
    uint16_t adc_int;

    OLED_Init();
    KEY_Init();
    PWM_Init(500, 1650);
    MCP4131_Init();
    MYADC_Init();

    level = MCP4131_GetLevel();
    freq = PWM_GetFrequency();

    /* ── 初始显示 ── */
    OLED_Clear();

    /* y=0: 幅值：xxx */
    OLED_ShowChinese(0, 0, CH_FU);
    OLED_ShowChinese(16, 0, CH_ZHI);
    OLED_ShowChinese(32, 0, CH_COLON);
    OLED_ShowNum(48, 0, level, 3);

    /* y=2: 频率：xxxxHz */
    OLED_ShowChinese(0, 2, CH_PIN);
    OLED_ShowChinese(16, 2, CH_LV);
    OLED_ShowChinese(32, 2, CH_COLON);
    OLED_ShowNum(48, 2, freq, 4);
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

            if(key != KEY_NONE)
            {
                switch(key)
                {
                    case KEY_AMP_UP:
                        MCP4131_Increase();
                        cnt_total++;
                        break;

                    case KEY_AMP_DOWN:
                        MCP4131_Decrease();
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
                break;  /* 有按键 → 提前退出延时循环，立即刷新 */
            }

            Delay_ms(10);
        }

        /* 读取当前值（不依赖按键） */
        level   = MCP4131_GetLevel();
        freq    = PWM_GetFrequency();
        voltage = MYADC_GetVoltage();
        adc_int = (uint16_t)(voltage * 100.0f + 0.5f);

        /* ── 刷新 OLED ── */
        OLED_Clear();

        /* y=0: 幅值：xxx */
        OLED_ShowChinese(0, 0, CH_FU);
        OLED_ShowChinese(16, 0, CH_ZHI);
        OLED_ShowChinese(32, 0, CH_COLON);
        OLED_ShowNum(48, 0, level, 3);

        /* y=2: 频率：xxxxHz */
        OLED_ShowChinese(0, 2, CH_PIN);
        OLED_ShowChinese(16, 2, CH_LV);
        OLED_ShowChinese(32, 2, CH_COLON);
        OLED_ShowNum(48, 2, freq, 4);
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
