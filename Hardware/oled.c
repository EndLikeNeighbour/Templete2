#include "oled.h"
#include "font.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#define OLED_SCL_PIN    GPIO_Pin_8
#define OLED_SDA_PIN    GPIO_Pin_9
#define OLED_PORT       GPIOB

static uint8_t oled_buffer[128][8];

void I2C_Delay(void)
{
    for(uint32_t i = 0; i < 100; i++);
}

void OLED_SCL_Clr(void)
{
    GPIO_ResetBits(OLED_PORT, OLED_SCL_PIN);
    I2C_Delay();
}

void OLED_SCL_Set(void)
{
    GPIO_SetBits(OLED_PORT, OLED_SCL_PIN);
    I2C_Delay();
}

void OLED_SDA_Clr(void)
{
    GPIO_ResetBits(OLED_PORT, OLED_SDA_PIN);
    I2C_Delay();
}

void OLED_SDA_Set(void)
{
    GPIO_SetBits(OLED_PORT, OLED_SDA_PIN);
    I2C_Delay();
}

void I2C_Start(void)
{
    OLED_SDA_Set();
    OLED_SCL_Set();
    OLED_SDA_Clr();
    OLED_SCL_Clr();
}

void I2C_Stop(void)
{
    OLED_SDA_Clr();
    OLED_SCL_Set();
    OLED_SDA_Set();
    I2C_Delay();
}

void I2C_SendByte(uint8_t byte)
{
    uint8_t i;
    for(i = 0; i < 8; i++)
    {
        OLED_SCL_Clr();
        
        if(byte & 0x80)
            OLED_SDA_Set();
        else
            OLED_SDA_Clr();
        
        OLED_SCL_Set();
        byte <<= 1;
    }
    
    OLED_SCL_Clr();
    OLED_SDA_Set();
    OLED_SCL_Set();
    OLED_SCL_Clr();
}

void OLED_WriteCmd(uint8_t cmd)
{
    I2C_Start();
    I2C_SendByte(OLED_I2C_ADDR);
    I2C_SendByte(0x00);
    I2C_SendByte(cmd);
    I2C_Stop();
}

void OLED_WriteData(uint8_t data)
{
    I2C_Start();
    I2C_SendByte(OLED_I2C_ADDR);
    I2C_SendByte(0x40);
    I2C_SendByte(data);
    I2C_Stop();
}

void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_6);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_7);
    
    GPIO_InitStructure.GPIO_Pin = OLED_SCL_PIN | OLED_SDA_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_PORT, &GPIO_InitStructure);
    
    OLED_SCL_Set();
    OLED_SDA_Set();
    
    for(uint32_t i = 0; i < 500000; i++);
    
    OLED_WriteCmd(0xAE);
    OLED_WriteCmd(0xD5);
    OLED_WriteCmd(0x80);
    OLED_WriteCmd(0xA8);
    OLED_WriteCmd(0x3F);
    OLED_WriteCmd(0xD3);
    OLED_WriteCmd(0x00);
    OLED_WriteCmd(0x40);
    OLED_WriteCmd(0x8D);
    OLED_WriteCmd(0x14);
    OLED_WriteCmd(0x20);
    OLED_WriteCmd(0x02);
    OLED_WriteCmd(0xA1);
    OLED_WriteCmd(0xC8);
    OLED_WriteCmd(0xDA);
    OLED_WriteCmd(0x12);
    OLED_WriteCmd(0x81);
    OLED_WriteCmd(0xFF);
    OLED_WriteCmd(0xD9);
    OLED_WriteCmd(0xF1);
    OLED_WriteCmd(0xDB);
    OLED_WriteCmd(0x30);
    OLED_WriteCmd(0xA4);
    OLED_WriteCmd(0xA6);
    OLED_WriteCmd(0xAF);
    
    for(uint32_t i = 0; i < 500000; i++);
    
    OLED_Clear();
}

void OLED_Clear(void)
{
    uint8_t i, j;
    for(i = 0; i < 128; i++)
        for(j = 0; j < 8; j++)
            oled_buffer[i][j] = 0;
}

void OLED_Refresh(void)
{
    uint8_t i, j;
    for(j = 0; j < 8; j++)
    {
        OLED_WriteCmd(0xB0 + j);
        OLED_WriteCmd(0x00);
        OLED_WriteCmd(0x10);
        
        for(i = 0; i < 128; i++)
            OLED_WriteData(oled_buffer[i][j]);
    }
}

void OLED_SetCursor(uint8_t x, uint8_t y)
{
    OLED_WriteCmd(0xB0 + y);
    OLED_WriteCmd((x & 0x0F));
    OLED_WriteCmd((x & 0xF0) >> 4);
}

void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t ch)
{
    uint8_t i;
    uint8_t c = ch - ' ';
    
    if(c > 95) c = 0;
    
    for(i = 0; i < 8; i++)
    {
        oled_buffer[x + i][y] = OLED_F8x16[c][i];
        oled_buffer[x + i][y + 1] = OLED_F8x16[c][i + 8];
    }
}

void OLED_ShowString(uint8_t x, uint8_t y, char *str)
{
    while(*str)
    {
        OLED_ShowChar(x, y, *str);
        x += 8;
        str++;
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t num, uint8_t len)
{
    uint8_t i;
    uint8_t str[10] = {0};
    
    for(i = len; i > 0; i--)
    {
        str[i-1] = (num % 10) + '0';
        num /= 10;
    }
    
    for(i = 0; i < len; i++)
    {
        OLED_ShowChar(x + i * 8, y, str[i]);
    }
}

void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index)
{
    uint8_t i;
    
    for(i = 0; i < 16; i++)
    {
        oled_buffer[x + i][y] = ChineseFont[index][i];
        oled_buffer[x + i][y + 1] = ChineseFont[index][i + 16];
    }
}
