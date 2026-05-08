#ifndef __FONT_INDEX_H
#define __FONT_INDEX_H

/*
 * ── ASCII 字库 (OLED_F8x16) ──
 * 定义于 Hardware/font.c:  const uint8_t OLED_F8x16[95][16]
 * 数组索引 = 字符ASCII码 - 0x20
 * 由 OLED_ShowChar() 自动转换，调用时直接传 ASCII 字符即可：
 *   OLED_ShowChar(x, y, 'A');   // 显示大写A
 *   OLED_ShowString(x, y, "Hello"); // 显示字符串
 *   OLED_ShowNum(x, y, 123, 3);     // 显示数字123
 *
 * 字库范围：索引  0 = 空格 (0x20)
 *          索引 94 = '~'  (0x7E)
 *          共 95 个可打印字符
 */

/* ── 常用符号别名（方便阅读） ── */
#define CH_SPACE    0x20    // 空格
#define CH_COLON_A  0x3A    // : (半角冒号)
#define CH_DOT      0x2E    // .
#define CH_MINUS    0x2D    // -
#define CH_PLUS     0x2B    // +
#define CH_SLASH    0x2F    // /

/*
 * ── 中文字库 (ChineseFont) ──
 * 定义于 Hardware/font.c:  uint8_t ChineseFont[17][32]
 * 配合 OLED_ShowChinese(x, y, index) 使用
 * 每个汉字 16×16 像素，占 2 个页行
 */

#define CH_FU       0   // 幅
#define CH_ZHI      1   // 值
#define CH_PIN      2   // 频
#define CH_LV       3   // 率
#define CH_AN       4   // 按
#define CH_JIAN     5   // 键
#define CH_CI       6   // 次
#define CH_SHU      7   // 数
#define CH_ZHI2     8   // 直
#define CH_LIU      9   // 流
#define CH_DIAN    10   // 电
#define CH_YA      11   // 压
#define CH_COLON   12   // ：(全角冒号)

/* 以下为索引 4~7 的副本，保留仅因字库中存在 */
#define CH_AN2     13   // 按 (同 CH_AN)
#define CH_JIAN2   14   // 键 (同 CH_JIAN)
#define CH_CI2     15   // 次 (同 CH_CI)
#define CH_SHU2    16   // 数 (同 CH_SHU)

#endif
