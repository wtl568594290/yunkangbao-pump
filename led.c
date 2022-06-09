#include "led.h"
/**
 * @brief 点亮电源指标灯
 *
 */
void LED_Init()
{
    P3M1 &= ~(1 << 4);
    P3M0 |= 1 << 4;
    IO_LED = 0;
}