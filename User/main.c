#include "stm32f10x.h"
#include "oled.h"
#include "key.h"
#include "delay.h"
#include "font_index.h"

int main(void)
{
    uint16_t cnt_amp_up = 0;
    uint16_t cnt_amp_dn = 0;
    uint16_t cnt_freq_up = 0;
    uint16_t cnt_freq_dn = 0;
    uint16_t cnt_measure = 0;
    uint16_t cnt_total = 0;

    OLED_Init();
    KEY_Init();

    OLED_Clear();
    OLED_ShowChinese(0, 0, CH_AN);    // 按
    OLED_ShowChinese(16, 0, CH_JIAN); // 键
    OLED_ShowChinese(32, 0, CH_CI);   // 次
    OLED_ShowChinese(48, 0, CH_SHU);  // 数
    OLED_ShowChinese(64, 0, CH_COLON);// ：
    OLED_ShowNum(80, 0, cnt_total, 3);
    OLED_Refresh();

    while(1)
    {
        key_state key = KEY_Scan();
        uint8_t updated = 0;

        switch(key)
        {
            case KEY_AMP_UP:    cnt_amp_up++;    cnt_total++; updated = 1; break;
            case KEY_AMP_DOWN:  cnt_amp_dn++;    cnt_total++; updated = 1; break;
            case KEY_FREQ_UP:   cnt_freq_up++;   cnt_total++; updated = 1; break;
            case KEY_FREQ_DOWN: cnt_freq_dn++;   cnt_total++; updated = 1; break;
            case KEY_MEASURE:   cnt_measure++;   cnt_total++; updated = 1; break;
            default: break;
        }

        if(updated)
        {
            OLED_Clear();
            OLED_ShowChinese(0, 0, CH_AN);    // 按
            OLED_ShowChinese(16, 0, CH_JIAN); // 键
            OLED_ShowChinese(32, 0, CH_CI);   // 次
            OLED_ShowChinese(48, 0, CH_SHU);  // 数
            OLED_ShowChinese(64, 0, CH_COLON);// ：
            OLED_ShowNum(80, 0, cnt_total, 3);

            OLED_ShowString(0, 2, "AMP+");
            OLED_ShowNum(36, 2, cnt_amp_up, 3);
            OLED_ShowString(64, 2, "AMP-");
            OLED_ShowNum(100, 2, cnt_amp_dn, 3);

            OLED_ShowString(0, 4, "FRE+");
            OLED_ShowNum(36, 4, cnt_freq_up, 3);
            OLED_ShowString(64, 4, "FRE-");
            OLED_ShowNum(100, 4, cnt_freq_dn, 3);

            OLED_ShowString(0, 6, "MEAS");
            OLED_ShowNum(36, 6, cnt_measure, 3);

            OLED_Refresh();
        }
    }
}
