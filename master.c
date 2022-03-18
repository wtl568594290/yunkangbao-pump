#include "master.h"

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