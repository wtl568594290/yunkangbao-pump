#include "adc.h"

/**
 * @brief adc��ʼ��
 *
 */
void ADC_Init()
{
    P1M1 &= ~(1 << 7); // p1.7����AD
    P1M0 |= ~(1 << 7);
    ADCCFG = 0x2F;    //���� ADC ʱ��Ϊϵͳʱ��/2/16/16,
                      // ����Ҷ��� (res���4λ,resl���8λ)
    ADC_CONTR = 0x80; //ʹ�� ADC ģ��
}

/**
 * @brief ȡ12λADC���
 *
 * @param in
 * @return uint
 */
uint ADC_Get(ADC_IN in)
{
    uint result;
    ADC_CONTR |= 0x40 | in; //���� AD ת��
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20))
        ;               //��ѯ ADC ��ɱ�־
    ADC_CONTR &= ~0x20; //����ɱ�־
    result = (uint)ADC_RES;
    result = (result << 8) | ADC_RESL;
    return result;
}