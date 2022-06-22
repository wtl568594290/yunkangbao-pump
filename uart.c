#include "uart.h"

#define BAUD 9600     //���ڲ�����
#define TIMEOUT_NUM 1 //���ڽ��ճ�ʱ���� 1*5 ms
enum
{
    RX_LEN_1 = 5,
};
static uchar RxBuf1[RX_LEN_1] = {0}; // ����1���ջ�����
static uchar RxIndex1 = 0;           // ����1���ռ���
static bit RxOver1 = 0;              // ����1 һ֡������ɱ�־
static bit TxOver1 = 0;              // ����1���ֽڷ�����ɱ�־

/**
 * @brief ���ڳ�ʼ��
 *
 */
void Uart_Init()
{
    P3M1 &= ~(1 << 6 | 1 << 7); // ����1 p3.6,p3.7
    P3M0 &= ~(1 << 6 | 1 << 7);
    P_SW1 = 0x40; // RXD_2/P3.6, TXD_2/P3.7

    T2L = (65536 - (FOSC / 4 / BAUD)); //���ò�������װֵ
    T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;
    AUXR |= 0x14; // T2Ϊ1Tģʽ, ��������ʱ��2

    SCON = 0x50;  // ����1
    AUXR |= 0x01; // ѡ��ʱ��2Ϊ����1�Ĳ����ʷ�����
    ES = 1;       // ʹ�ܴ���1�ж�
}

/**
 * @brief ����1��������
 *
 * @param buf ����ָ��
 * @param len ���ݳ���
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
 * @brief ����1���պ���
 *
 * @param buf ��Ž������ݵ�����
 * @return bit �Ƿ���յ����� 1���յ�,0û����
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
 * @brief ����1�ж�
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
 * @brief ����1���ռ�س���
 * �ڶ�ʱ���е���,�ж��ڿ��� TIMEOUT(ms) ֮�����һ֡����
 */
static void uart1_5ms()
{
    static uchar index = 0;
    static uchar countRx = 0;
    // Rx������
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
 * @brief ���ڼ�����
 * ��t1�е���
 */
void Uart_5ms()
{
    uart1_5ms();
}

/**
 * @brief ���ڷ�������
 *
 * @param select ѡ�������ݵĴ���
 * @param buf ���͵�����ָ��
 * @param len ���͵����ݳ���
 */
void Uart_Tx(Uart_port port, uchar *buf, uchar len)
{
    switch (port)
    {
    case Uart_PORT_1:
        uart1Tx(buf, len);
        // ����1����֮��,��ʱ30ms
        Delay_ms(30);
        break;
    }
}

/**
 * @brief ���ڽ��ճ���
 *
 * @param port ѡ����յĴ���
 * @param buf ������ݵ�����
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
