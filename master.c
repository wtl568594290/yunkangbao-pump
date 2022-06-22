#include "master.h"

typedef enum
{
    Type_ReportTemp = 0xB1, // 发送温度
} CMDType;

/**
 * @brief 通信主循环
 *
 */
void Master()
{
    uchar buf[5] = {0};
    if (Uart_Rx(Uart_PORT_1, buf))
    {
        if (buf[0] == 0xEE && buf[1] == 0xB5 && buf[3] == 0xFC && buf[4] == 0xFF)
        {
            if (buf[2] > 0 && buf[2] <= 4)
            {
                Pump_SetMode(buf[2]);
            }
            else
            {
                Pump_SetMode(0);
            }
        }
    }
}

/**
 * @brief 报告温度
 *
 * @param over
 */
void Master_ReportTemperature(bit over)
{
    uchar buf[7] = {0};
    buf[0] = 0xEE;
    buf[1] = Type_ReportTemp;
    buf[2] = over;
    buf[3] = 0xFF;
    buf[4] = 0xFC;
    buf[5] = 0xFF;
    buf[6] = 0xFF;
    Uart_Tx(Uart_PORT_1, buf, 7);
}