#include "led.h"
/**
 * @brief 点亮电源指标灯
 * 
 */
void LED_Init()
{
    P0M1 &= ~(1 << 4);
    P0M0 |= 1 << 4;
    OUT_LED = 0;
}