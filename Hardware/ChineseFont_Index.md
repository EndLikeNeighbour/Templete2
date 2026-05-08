# ChineseFont 索引表

记录 `Hardware/font.c` 中 `ChineseFont[17][32]` 的字符映射关系。

## 索引映射

| 索引 | 字符 | Unicode | 备注 |
|------|------|---------|------|
| 0 | 幅 | U+5E45 | |
| 1 | 值 | U+503C | |
| 2 | 频 | U+9891 | |
| 3 | 率 | U+7387 | |
| 4 | 按 | U+6309 | |
| 5 | 键 | U+952E | |
| 6 | 次 | U+6B21 | |
| 7 | 数 | U+6570 | |
| 8 | 直 | U+76F4 | |
| 9 | 流 | U+6D41 | |
| 10 | 电 | U+7535 | |
| 11 | 压 | U+538B | |
| 12 | ： | U+FF1A | 全角冒号 |
| 13 | 按 | U+6309 | 与索引4重复 |
| 14 | 键 | U+952E | 与索引5重复 |
| 15 | 次 | U+6B21 | 与索引6重复 |
| 16 | 数 | U+6570 | 与索引7重复 |

## 使用示例

```c
// 显示"幅值："
OLED_ShowChinese(0, 0, 0);   // 幅
OLED_ShowChinese(16, 0, 1);  // 值
OLED_ShowChinese(32, 0, 12); // ：

// 显示"频率："
OLED_ShowChinese(0, 2, 2);   // 频
OLED_ShowChinese(16, 2, 3);  // 率
OLED_ShowChinese(32, 2, 12); // ：

// 显示"直流电压："
OLED_ShowChinese(0, 4, 8);   // 直
OLED_ShowChinese(16, 4, 9);  // 流
OLED_ShowChinese(32, 4, 10); // 电
OLED_ShowChinese(48, 4, 11); // 压
OLED_ShowChinese(64, 4, 12); // ：

// 显示"按键次数："
OLED_ShowChinese(0, 6, 4);   // 按
OLED_ShowChinese(16, 6, 5);  // 键
OLED_ShowChinese(32, 6, 6);  // 次
OLED_ShowChinese(48, 6, 7);  // 数
OLED_ShowChinese(64, 6, 12); // ：
```

## 注意

- 每个中文字符宽 **16 像素**，高 **16 像素**（占2个页行）
- 支持的有效 y 坐标：0、2、4、6（每页行16像素，共64像素）
- 索引4~7 与 13~16 内容重复，实际有效字符为13个
