#ifndef CONFIG_H
#define CONFIG_H

#include "STC8H.H"
#include "intrins.h"
#include "delay.h"
#include "string.h"

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define FOSC 24000000UL // 系统频率
#define INTERVAL_T1 1   // ms
// 电磁阀 低电平有效
sbit IO_TAP_RIGHT_INTO = P2 ^ 3;
sbit IO_TAP_RIGHT_LEAVE = P2 ^ 2;
sbit IO_TAP_LEFT_INTO = P2 ^ 1;
sbit IO_TAP_LEFT_LEAVE = P2 ^ 0;
// 气泵 低电平有效
sbit IO_AIR = P5 ^ 4;

// 蜂鸣器
sbit IO_BUZZER = P3 ^ 6;
// 电源指示灯
sbit IO_LED = P3 ^ 4;

#endif