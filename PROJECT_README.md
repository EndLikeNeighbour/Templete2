# STM32F103 简易波形发生器项目

## 项目结构

```
./
├── Library/           # STM32标准库
│   ├── misc.c/h
│   ├── stm32f10x_adc.c/h
│   ├── stm32f10x_gpio.c/h
│   ├── stm32f10x_i2c.c/h
│   ├── stm32f10x_rcc.c/h
│   └── stm32f10x_tim.c/h
├── Startup/           # 启动文件
│   ├── core_cm3.c/h
│   ├── stm32f10x.h
│   └── startup_stm32f103xb.s
├── User/           # 用户主程序
│   ├── main.c
│   └── stm32f10x_conf.h
├── Hardware/       # 硬件驱动
│   ├── adc.c/h
│   ├── font.c/h
│   ├── key.c/h
│   ├── oled.c/h
│   ├── pwm.c/h
│   └── sine_table.h
├── stm32f103c8t6_flash.ld  # 链接脚本
├── PROJECT_README.md        # 项目说明
└── 可调正弦波说明.md         # 波形说明
```

## 功能说明

1. **最小系统功能**
   - 0.96英寸OLED屏幕显示
   - ADC采集直流电压并显示

2. **正弦波输出**
   - **外部峰峰值(Vpp)：1V-10V，步进1V
   - **内部振幅**：10V→1.65V, 9V→1.485V, 8V→1.32V, ..., 1V→0.165V
   - **频率**：100Hz-1000Hz，步进100Hz

3. **按键定义**
   - K1(PC0)：Vpp+1V
   - K2(PC1)：Vpp-1V
   - K3(PC2)：Freq+100Hz
   - K4(PC3)：Freq-100Hz
   - K5(PC4)：测量更新

4. **OLED显示**
   - 第1行：设置Vpp: XXV
   - 第2行：设置Freq: XXXXHz
   - 第3行：Vpp: XXV
   - 第4行：Freq: XXXXHz

## 硬件连接

| 模块 | STM32引脚 |
|------|----------|
| OLED SCL | PB6 |
| OLED SDA | PB7 |
| PWM输出 | PA0 |
| ADC输入 | PA1 |
| K1(Vpp+) | PC0 |
| K2(Vpp-) | PC1 |
| K3(Freq+) | PC2 |
| K4(Freq-) | PC3 |
| K5(测量) | PC4 |

## 外部电路说明

STM32输出以1.65V为中心，外部运放放大后：
- Vpp=10V → 内部振幅1.65V
- Vpp=9V → 内部振幅1.485V
- Vpp=8V → 内部振幅1.32V
- ...
- Vpp=1V → 内部振幅0.165V

## Keil项目设置

### 步骤1：创建新项目
1. 打开Keil MDK
2. 新建项目，选择STM32F103C8T6
3. 增加Startup文件：Startup/startup_stm32f103xb.s

### 步骤2：添加源文件
- Library组：stm32f10x_adc.c, stm32f10x_gpio.c, stm32f10x_i2c.c, stm32f10x_rcc.c, stm32f10x_tim.c, misc.c, core_cm3.c
- Hardware组：oled.c, pwm.c, adc.c, key.c, font.c
- User组：main.c

### 步骤3：配置头文件路径
```
./Library
./Startup
./User
./Hardware
```

### 步骤4：配置链接脚本
选择 stm32f103c8t6_flash.ld

### 步骤5：配置编译选项
- 优化等级：-Os
- 语言标准：C99

## 使用说明

1. 编译项目并下载到STM32开发板
2. 连接外部运放电路实现-5V~+5V输出
3. 使用按键调节参数：
   - K1/K2：调节Vpp(1V-10V)
   - K3/K4：调节频率(100Hz-1000Hz)
   - K5：按下测量更新，刷新显示
