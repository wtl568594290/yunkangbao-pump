#include "uart.h"

#define BAUD 9600     //串口波特率
#define TIMEOUT_NUM 1 //串口接收超时计数 1*5 ms
enum
{
    RX_LEN_1 = 5,
};
static uchar RxBuf1[RX_LEN_1] = {0}; // 串口1接收缓冲区
static uchar RxIndex1 = 0;           // 串口1接收计数
static bit RxOver1 = 0;              // 串口1 一帧接收完成标志
static bit TxOver1 = 0;              // 串口1单字节发送完成标志

/**
 * @brief 串口初始化
 *
 */
void Uart_Init()
{
    P3M1 &= ~(1 << 6 | 1 << 7); // 串口1 p3.6,p3.7
    P3M0 &= ~(1 << 6 | 1 << 7);
    P_SW1 = 0x40; // RXD_2/P3.6, TXD_2/P3.7

    T2L = (65536 - (FOSC / 4 / BAUD)); //设置波特率重装值
    T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;
    AUXR |= 0x14; // T2为1T模式, 并启动定时器2

    SCON = 0x50;  // 串口1
    AUXR |= 0x01; // 选择定时器2为串口1的波特率发生器
    ES = 1;       // 使能串口1中断
}

/**
 * @brief 串口1发送数据
 *
 * @param buf 数据指针
 * @param len 数据长度
 */
static void uart1Tx(uchar *buf, uchar len)
{

    while (len--)
    {
        TxOver1 = 0;
        SBUF = *buf++;
        while (!TxOver1)
            ;
    }
}

/**
 * @brief 串口1接收函数
 *
 * @param buf 存放接收数据的数组
 * @return bit 是否接收到数据 1接收到,0没接收
 */
static bit uart1Rx(uchar *buf)
{
    bit over = 0;
    if (RxOver1)
    {
        RxOver1 = 0;
        memcpy(buf, RxBuf1, RX_LEN_1);
        over = 1;
    }
    return over;
}

/**
 * @brief 串口1中断
 *
 */
static void uart1Int() interrupt 4
{
    if (RI)
    {
        RI = 0;
        if (RxIndex1 < RX_LEN_1)
        {
            RxBuf1[RxIndex1++] = SBUF;
        }
    }
    if (TI)
    {
        TI = 0;
        TxOver1 = 1;
    }
}

/**
 * @brief 串口1接收监控程序
 * 在定时器中调用,判定在空闲 TIMEOUT(ms) 之后结束一帧接收
 */
static void uart1_5ms()
{
    static uchar index = 0;
    static uchar countRx = 0;
    // Rx监视器
    if (RxIndex1 > 0)
    {
        if (index != RxIndex1)
        {
            index = RxIndex1;
            countRx = 0;
        }
        else
        {
            if (++countRx >= TIMEOUT_NUM)
            {
                RxIndex1 = 0;
                RxOver1 = 1;
            }
        }
    }
    else
    {
        index = 0;
    }
}

/**
 * @brief 串口监视器
 * 在t1中调用
 */
void Uart_5ms()
{
    uart1_5ms();
}

/**
 * @brief 串口发送数据
 *
 * @param select 选择发送数据的串口
 * @param buf 发送的数据指针
 * @param len 发送的数据长度
 */
void Uart_Tx(Uart_port port, uchar *buf, uchar len)
{
    switch (port)
    {
    case Uart_PORT_1:
        uart1Tx(buf, len);
        // 串口1发送之后,延时30ms
        Delay_ms(30);
        break;
    }
}

/**
 * @brief 串口接收程序
 *
 * @param port 选择接收的串口
 * @param buf 存放数据的数组
 */
bit Uart_Rx(Uart_port port, uchar *buf)
{
    bit over = 0;
    switch (port)
    {
    case Uart_PORT_1:
        over = uart1Rx(buf);
        break;
    }
    return over;
}
