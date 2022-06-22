#include "temperature.h"

#define TEMP_AD_IN ADC_IN_17               // �¶ȼ���
#define TEMP_CHANGE_MAX 6000               // �¶ȱ仯����ʱʱ�䣬*5ms
#define TEMP_HIGH ((uint)(2.5 * 4096 / 5)) // �����±���
#define TEMP_LOW ((uint)(1.5 * 4096 / 5))  // ����

static bit CheckNow;

/**
 * @brief ���³�ʼ��
 *
 */
void Temperature_Init()
{
    CheckNow = 0;
}

/**
 * @brief �¶ȼ���־������
 *
 */
void Temperature_5ms()
{
    CheckNow = 1;
}

/**
 * @brief �¶ȼ����ѭ��
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