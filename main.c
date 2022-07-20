#include "config.h"
#include "uart.h"
#include "led.h"
#include "pump.h"
#include "master.h"
#include "adc.h"
#include "temperature.h"
/**
 * @brief ��ʱ��1��ʼ��
 *
 */
void T1_Init()
{
    AUXR |= 0x40;                                   //��ʱ��ʱ��1Tģʽ
    TMOD &= 0x0F;                                   //���ö�ʱ��ģʽ
    TL1 = (65536 - FOSC * INTERVAL_T1 / 1000);      //���ö�ʱ��ʼֵ
    TH1 = (65536 - FOSC * INTERVAL_T1 / 1000) >> 8; //���ö�ʱ��ʼֵ
    TF1 = 0;                                        //���TF1��־
    TR1 = 1;                                        //��ʱ��1��ʼ��ʱ
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