#ifndef CONFIG_H
#define CONFIG_H

#include "STC15.H"
#include "delay.h"
#include "intrins.h"
#include "string.h"

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define FOSC 24000000UL // ϵͳƵ��
#define INTERVAL_T1 1   // ms
// ��ŷ� �͵�ƽ��Ч
sbit IO_TAP_RIGHT_INTO = P2 ^ 7;
sbit IO_TAP_RIGHT_LEAVE = P2 ^ 6;
sbit IO_TAP_LEFT_INTO = P2 ^ 5;
sbit IO_TAP_LEFT_LEAVE = P2 ^ 4;
// ���� �͵�ƽ��Ч
sbit IO_AIR = P4 ^ 5;

// ������
sbit IO_BUZZER = P3 ^ 6;
// ��Դָʾ��
sbit OUT_LED = P0 ^ 4;

#endif