#ifndef ADC_H
#define ADC_H
#include "config.h"

typedef enum
{
    ADC_IN_10 = 0,
    ADC_IN_11,
    ADC_IN_12,
    ADC_IN_13,
    ADC_IN_14,
    ADC_IN_15,
    ADC_IN_16,
    ADC_IN_17,
    ADC_IN_00,
    ADC_IN_01,
    ADC_IN_02,
    ADC_IN_03,
    ADC_IN_04,
    ADC_IN_05,
    ADC_IN_06,
} ADC_IN;

void ADC_Init();
uchar ADC_Get(ADC_IN in);

#endif