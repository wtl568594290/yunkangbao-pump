#include "adc.h"

/**
 * @brief adc初始化
 *
 */
void ADC_Init()
{
    P1M1 |= 1 << 7; // p1.7用作AD
    P1M0 &= ~(1 << 7);
    ADCCFG = 0x0F;    //设置 ADC 时钟为系统时钟/2/16/16,
    ADC_CONTR = 0x80; //使能 ADC 模块
}

/**
 * @brief 取8位ADC结果
 *
 * @param in
 * @return uchar
 */
uchar ADC_Get(ADC_IN in)
{
    ADC_CONTR |= 0x40 | in; //启动 AD 转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20))
        ;               //查询 ADC 完成标志
    ADC_CONTR &= ~0x20; //清完成标志
    return ADC_RES;     //返回 ADC 结果
}
