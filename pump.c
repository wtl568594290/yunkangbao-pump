#include "pump.h"

#define CYCLIC 5
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
static uchar CyclicSec[4] = {12, 18, 16, 24}; // 每个模式的周期
// TODO 多维结构体数组 无结构体需要补{0}
static param Param[4][2][CYCLIC] = {
    {
        {{6000, 250, 2000}, {18000, 250, 2000}, {30000, 250, 2000}, {42000, 250, 2000}, {0}},
        {{0, 250, 2000}, {12000, 250, 2000}, {24000, 250, 2000}, {36000, 250, 2000}, {48000, 250, 2000}},
    },
    {
        {{4000, 150, 1000}, {7000, 150, 2000}, {16000, 150, 1000}, {0}, {0}},
        {{0, 150, 1000}, {8000, 150, 1000}, {12000, 150, 1000}, {15000, 150, 2000}, {0}},
    },
    {
        {{2000, 100, 0}, {3000, 50, 0}, {4000, 50, 0}, {0, 0, 5000}, {0}},
        {{10000, 100, 0}, {11000, 50, 0}, {12000, 50, 0}, {0, 0, 13000}, {0}},
    },
    {
        {{0, 250, 1000}, {6000, 120, 1000}, {12000, 160, 1000}, {18000, 200, 1000}, {24000, 250, 1000}},
        {{0, 250, 1000}, {6000, 120, 1000}, {12000, 160, 1000}, {18000, 200, 1000}, {24000, 250, 1000}},
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
        if (++Count > (CyclicSec[Mode - 1] * 200))
        {
            Count = 0;
        }
        for (i = 0; i < 2; i++)
        {
            for (j = 0; j < CYCLIC; j++)
            {
                // 提前憋气
                if (Param[Mode - 1][i][j].startInto > 100 && Count * 5 == Param[Mode - 1][i][j].startInto - 100)
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
                // 进气时长大于0  才会开启进气口
                else if (Param[Mode - 1][i][j].startInto && Param[Mode - 1][i][j].into && Count * 5 == Param[Mode - 1][i][j].startInto)
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
                else if (Param[Mode - 1][i][j].into && Count * 5 == Param[Mode - 1][i][j].startInto + Param[Mode - 1][i][j].into)
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
                else if (Param[Mode - 1][i][j].keep && Count * 5 == Param[Mode - 1][i][j].startInto + Param[Mode - 1][i][j].keep)
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