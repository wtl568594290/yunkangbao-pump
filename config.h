#ifndef CONFIG_H
#define CONFIG_H

#include "STC8H.H"
#include "intrins.h"
#include "delay.h"
#include "string.h"

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define FOSC 24000000UL // ϵͳƵ��
#define INTERVAL_T1 1   // ms
// ��ŷ� �͵�ƽ��Ч
sbit IO_TAP_RIGHT_INTO = P2 ^ 3;
sbit IO_TAP_RIGHT_LEAVE = P2 ^ 2;
sbit IO_TAP_LEFT_INTO = P2 ^ 1;
sbit IO_TAP_LEFT_LEAVE = P2 ^ 0;
// ���� �͵�ƽ��Ч
sbit IO_AIR = P5 ^ 4;

// ������
sbit IO_BUZZER = P3 ^ 6;
// ��Դָʾ��
sbit IO_LED = P3 ^ 4;

#endif