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
static uchar CyclicSec[4] = {18, 32, 32, 24}; // 每个模式的周期
// 多维结构体数组 无结构体需要补{0}
// TODO 减小频率
static param Param[4][2][CYCLIC] = {
    {
        {{3000, 250, 2000}, {27000, 250, 2000}, {30000, 250, 2000}, {42000, 250, 2000}, {0}},
        {{0}, {12000, 250, 2000}, {24000, 250, 2000}, {36000, 250, 2000}, {48000, 250, 2000}},
    },
    {
        {{3000, 200, 1000}, {10000, 200, 2000}, {27000, 200, 1000}, {0}, {0}},
        {{0}, {11000, 200, 1000}, {19000, 200, 1000}, {26000, 200, 2000}, {0}},
    },
    {
        {{3000, 150, 1000}, {11000, 180, 1000}, {19000, 210, 1000}, {27000, 250, 1000}, {0}},
        {{3000, 150, 1000}, {11000, 180, 1000}, {19000, 210, 1000}, {27000, 250, 1000}, {0}},
    },
    {
        {{3000, 100, 0}, {4000, 50, 0}, {5000, 50, 0}, {0, 0, 6000}, {0}},
        {{15000, 100, 0}, {16000, 50, 0}, {17000, 50, 0}, {0, 0, 18000}, {0}},
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
    P2M1 &= ~(1 | 1 << 1 | 1 << 2 | 1 << 3);
    P2M0 &= ~(1 | 1 << 1 | 1 << 2 | 1 << 3);
    P5M1 &= ~(1 << 4);
    P5M0 &= ~(1 << 4);
    IO_AIR = OFF;
    IO_TAP_LEFT_LEAVE = OFF;
    IO_TAP_RIGHT_LEAVE = OFF;
    IO_TAP_LEFT_INTO = OFF;
    IO_TAP_RIGHT_INTO = OFF;
    // TODO 开机放气

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