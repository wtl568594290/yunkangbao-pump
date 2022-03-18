#include "pump.h"

enum
{
    ON,
    OFF,
    LEFT = 0,
    RIGHT,
};
typedef struct
{
    uint startInto; // 进气时间点
    uint into;      // 进气时长
    uint keep;      // 保持时长
} param;

static uchar Mode;
static uint Count;
// TODO 多维结构体数组 无结构体需要补{0}
static param Param[4][2][5] = {
    {
        {{5000, 200, 3000}, {15000, 200, 3000}, {25000, 200, 3000}, {0}, {0}},
        {{10000, 200, 3000}, {20000, 200, 3000}, {30000, 200, 3000}, {0}, {0}},
    },
    {
        {{8000, 100, 1500}, {16000, 100, 1500}, {30000, 100, 1500}, {0}, {0}},
        {{16000, 100, 1500}, {24000, 100, 1500}, {30000, 100, 1500}, {0}, {0}},
    },
    {
        {{6000, 100, 1000}, {12000, 100, 1000}, {18000, 100, 1000}, {24000, 100, 1000}, {30000, 100, 1000}},
        {{6000, 100, 1000}, {12000, 100, 1000}, {18000, 100, 1000}, {24000, 100, 1000}, {30000, 100, 1000}},
    },
    {
        {{8000, 100, 1000}, {16000, 100, 1000}, {30000, 100, 1000}, {0}, {0}},
        {{16000, 100, 1000}, {24000, 100, 1000}, {30000, 100, 1000}, {0}, {0}},
    },
};

/**
 * @brief 足泵监视器
 *
 */
void Pump_5ms()
{
    uchar i, j;
    if (Mode)
    {
        if (++Count > (33 * 200))
        {
            Count = 0;
        }
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < 5; j++)
            {
                if (Param[Mode - 1][i][j].startInto > 1)
                {
                    if (Count * 5 == Param[Mode - 1][i][j].startInto - 1000)
                    {
                        if (i == LEFT)
                        {
                            IO_TAP_LEFT_LEAVE = ON;
                        }
                        else
                        {
                            IO_TAP_RIGHT_LEAVE = ON;
                        }
                    }
                    else if (Count * 5 == Param[Mode - 1][i][j].startInto)
                    {
                        if (i == LEFT)
                        {
                            IO_TAP_LEFT_INTO = ON;
                        }
                        else
                        {
                            IO_TAP_RIGHT_INTO = ON;
                        }
                    }
                    else if (Count * 5 == Param[Mode - 1][i][j].startInto + Param[Mode - 1][i][j].into)
                    {
                        if (i == LEFT)
                        {
                            IO_TAP_LEFT_INTO = OFF;
                        }
                        else
                        {
                            IO_TAP_RIGHT_INTO = OFF;
                        }
                    }
                    else if (Count * 5 == Param[Mode - 1][i][j].startInto + Param[Mode - 1][i][j].keep)
                    {
                        if (i == LEFT)
                        {
                            IO_TAP_LEFT_LEAVE = OFF;
                        }
                        else
                        {
                            IO_TAP_RIGHT_LEAVE = OFF;
                        }
                    }
                }
            }
        }
    }
    else
    {
        Count = 0;
    }
}
/**
 * @brief 气泵初始化
 *
 */
void Pump_Init()
{
    P4M1 &= ~(1 << 5);
    P4M0 &= ~(1 << 5);
    P2M1 &= ~(1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
    P2M0 &= ~(1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
    IO_AIR = OFF;
    IO_TAP_LEFT_LEAVE = OFF;
    IO_TAP_RIGHT_LEAVE = OFF;
    // 开机放气
    IO_TAP_LEFT_INTO = ON;
    IO_TAP_RIGHT_INTO = ON;
    Delay3000ms();
    IO_TAP_LEFT_INTO = OFF;
    IO_TAP_RIGHT_INTO = OFF;

    Mode = 0;
    Count = 0;
}

/**
 * @brief 设置足泵MODE
 *
 * @param mode
 */
void Pump_SetMode(uchar mode)
{
    Mode = mode;
    Count = 0;
    if (Mode)
    {
        IO_TAP_LEFT_INTO = OFF;
        IO_TAP_LEFT_LEAVE = OFF;
        IO_TAP_RIGHT_INTO = OFF;
        IO_TAP_RIGHT_LEAVE = OFF;
    }
}

bit t;
/**
 * @brief 足泵主循环
 *
 */
void Pump()
{
    if (Mode)
    {
        IO_AIR = ON;
    }
    else
    {
        IO_AIR = OFF;
        IO_TAP_LEFT_INTO = OFF;
        IO_TAP_LEFT_LEAVE = OFF;
        IO_TAP_RIGHT_INTO = OFF;
        IO_TAP_RIGHT_LEAVE = OFF;
    }
}