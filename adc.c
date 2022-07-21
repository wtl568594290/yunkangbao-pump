#include "adc.h"

/**
 * @brief adc��ʼ��
 *
 */
void ADC_Init()
{
    P1M1 |= 1 << 7; // p1.7����AD
    P1M0 &= ~(1 << 7);
    ADCCFG = 0x0F;    //���� ADC ʱ��Ϊϵͳʱ��/2/16/16,
    ADC_CONTR = 0x80; //ʹ�� ADC ģ��
}

/**
 * @brief ȡ8λADC���
 *
 * @param in
 * @return uchar
 */
uchar ADC_Get(ADC_IN in)
{
    ADC_CONTR |= 0x40 | in; //���� AD ת��
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20))
        ;               //��ѯ ADC ��ɱ�־
    ADC_CONTR &= ~0x20; //����ɱ�־
    return ADC_RES;     //���� ADC ���
}
