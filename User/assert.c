#include "stm32f10x.h"

void assert_failed(uint8_t* file, uint32_t line)
{
    (void)file;
    (void)line;
    while(1);
}