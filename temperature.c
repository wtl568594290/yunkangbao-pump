#include "temperature.h"

#define TEMP_AD_IN ADC_IN_17                // 温度检测口
#define TEMP_CHANGE_MAX 300                 // 温度变化后延时时间(100ms)
#define TEMP_HIGH ((uint)(1.37 * 4096 / 5)) // 过高温保护 40C
#define TEMP_LOW ((uint)(1.00 * 4096 / 5))  // 低温 30C

static bit CheckNow;

/**
 * @brief 测温初始化
 *
 */
void Temperature_Init()
{
    CheckNow = 0;
}

/**
 * @brief 温度检测标志监视器
 *
 */
void Temperature_100ms()
{
    CheckNow = 1;
}

/**
 * @brief 温度检测主循环
 *
 */
void Temperature()
{
    static overflow = 0;
    static uint count = 0;
    static bit report = 0;
    uint temp;

    if (CheckNow)
    {
        CheckNow = 0;
        temp = ADC_Get(TEMP_AD_IN);
        if (overflow == 0 && temp > TEMP_HIGH)
        {
            if (count < TEMP_CHANGE_MAX)
            {
                count++;
            }
            else
            {
                overflow = 1;
                if (report == 0)
                {
                    report = 1;
                    Master_ReportTemperature(1);
                }
            }
        }
        else if (overflow == 1 && temp < TEMP_LOW)
        {
            if (count < TEMP_CHANGE_MAX)
            {
                count++;
            }
            else
            {
                overflow = 0;
                if (report == 0)
                {
                    report = 1;
                    Master_ReportTemperature(0);
                }
            }
        }
        else
        {
            count = 0;
            report = 0;
        }
    }
}