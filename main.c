#include "config.h"
#include "uart.h"
#include "led.h"
#include "pump.h"
#include "master.h"
#include "adc.h"
#include "temperature.h"
/**
 * @brief 定时器1初始化
 *
 */
void T1_Init()
{
    AUXR |= 0x40;                                   //定时器时钟1T模式
    TMOD &= 0x0F;                                   //设置定时器模式
    TL1 = (65536 - FOSC * INTERVAL_T1 / 1000);      //设置定时初始值
    TH1 = (65536 - FOSC * INTERVAL_T1 / 1000) >> 8; //设置定时初始值
    TF1 = 0;                                        //清除TF1标志
    TR1 = 1;                                        //定时器1开始计时
    ET1 = 1;
}

void T1() interrupt 3
{
    static uchar count5ms = 0;
    static uint count100ms = 0;
    static uint count1s = 0;
    if (++count5ms >= 5)
    {
        count5ms = 0;
        // 5ms
        Uart_5ms();
        Pump_5ms();
    }
    if (++count100ms >= 100)
    {
        count100ms = 0;
        // 100ms
        Temperature_100ms();
    }
    if (++count1s >= 1000)
    {
        count1s = 0;
        // 1s
    }
}

void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P4M0 = 0x00;
    P4M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    P6M0 = 0x00;
    P6M1 = 0x00;
    P7M0 = 0x00;
    P7M1 = 0x00;
    T1_Init();
    Uart_Init();
    LED_Init();
    Pump_Init();
    ADC_Init();
    Temperature_Init();
    EA = 1;
    while (1)
    {
        Pump();
        Master();
        Temperature();
    }
}