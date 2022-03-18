#ifndef PUMP_H
#define PUMP_H
#include "config.h"
#include "uart.h"
void Pump_Init();
void Pump_5ms();
void Pump_SetMode(uchar mode);
void Pump();
#endif