/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:54:41
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-08 13:46:52
 * @FilePath     : port.h
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#ifndef _PORT_H_
#define _PORT_H_

#include "gd32f3x0.h"
#include "systick.h"

typedef enum
{
    // 门磁反馈信号
    DOOR_PIN,
    // 锁舌反馈信号
    TONGUE_PIN,
    // 外部开门信号
    OPEN_SIGNAL_PIN
} LOCK_DI_PIN;

typedef enum
{
    // 对外输出信号
    DOOR_OUT_PIN,
    // 对外LED信号
    LED_OUT_PIN
} LOCK_DO_PIN;

//
extern void Port_Init(void);
//
extern FlagStatus Get_DI_State(LOCK_DI_PIN port);
//
extern void Set_DO_State(LOCK_DO_PIN port, FlagStatus state);

#endif
