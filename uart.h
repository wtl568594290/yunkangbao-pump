#ifndef UART_H
#define UART_H
#include "config.h"
typedef enum
{
    Uart_PORT_1,
} Uart_port;

void Uart_Tx(Uart_port port, uchar *buf, uchar len);
bit Uart_Rx(Uart_port port, uchar *buf);
void Uart_Init();
void Uart_5ms();

#endif