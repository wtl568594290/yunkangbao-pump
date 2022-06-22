#include "adc.h"

/**
 * @brief adc初始化
 *
 */
void ADC_Init()
{
    P1M1 &= ~(1 << 7); // p1.7用作AD
    P1M0 |= ~(1 << 7);
    ADCCFG = 0x2F;    //设置 ADC 时钟为系统时钟/2/16/16,
                      // 结果右对齐 (res存高4位,resl存低8位)
    ADC_CONTR = 0x80; //使能 ADC 模块
}

/**
 * @brief 取12位ADC结果
 *
 * @param in
 * @return uint
 */
uint ADC_Get(ADC_IN in)
{
    uint result;
    ADC_CONTR |= 0x40 | in; //启动 AD 转换
    _nop_();
    _nop_();
    while (!(ADC_CONTR & 0x20))
        ;               //查询 ADC 完成标志
    ADC_CONTR &= ~0x20; //清完成标志
    result = (uint)ADC_RES;
    result = (result << 8) | ADC_RESL;
    return result;
}