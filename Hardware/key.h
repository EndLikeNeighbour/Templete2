#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

typedef enum {
    KEY_NONE,
    KEY_AMP_UP,
    KEY_AMP_DOWN,
    KEY_FREQ_UP,
    KEY_FREQ_DOWN,
    KEY_MEASURE
} key_state;

void KEY_Init(void);
key_state KEY_Scan(void);

#endif