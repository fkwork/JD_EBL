/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:36:07
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-08 14:29:15
 * @FilePath     : app.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include "app.h"
#include "port.h"
#include <stdio.h>

/**
    ==================================================================================
    4.1开锁逻辑
        4.1.1上电时，无门磁反馈
            条件：上电时，无门磁反馈
            动作：电子锁自动开锁。
        4.1.2门关时，接收到开锁信号
            条件：电子锁供电正常，有门磁反馈（门关），无锁舌反馈。将开锁信号接地。
            动作：锁舌收回。
    ==================================================================================
    ==================================================================================
    4.2关锁逻辑
        4.2.1开锁后，门打开，在关门
            条件：电子锁打开，且打开门（门磁无反馈）时，将门磁片对准电子锁，确保有门磁反馈
            动作：维持3s后，电子锁自动关锁。
        4.2.2开锁后，门未开
            条件：将电子锁开锁，且门磁反馈一直有（门未打开）
            动作：维持10s，电子锁自动关锁。
    ==================================================================================
**/

LOCK_STATE Global_Lock_State = {RESET, RESET, RESET, 0, 0, 0};

static void Lock_Close(void)
{
    return;
}

static void Lock_Open(void)
{
    return;
}

void Get_Lock_State(void)
{
    FlagStatus Ret = RESET;
    //
    Ret = Get_DI_State(DOOR_PIN);
    //
    Global_Lock_State.DoorState = Ret ? ON : OFF;
    //
    Ret = Get_DI_State(TONGUE_PIN);
    //
    Global_Lock_State.TongueState = Ret ? ON : OFF;
    //
    Ret = Get_DI_State(OPEN_SIGNAL_PIN);
    //
    Global_Lock_State.OpenSignalState = Ret ? ON : OFF;
}

static void Lock_Init_Control(void)
{
    if (Get_DI_State(DOOR_PIN) == SET)
    {
        Delay_Ms(100);
        if (Get_DI_State(DOOR_PIN) == SET)
        {
            Lock_Open();
        }
    }
    return;
}

static void Lock_Run_Control(void)
{
        return;
}

void App_Init(void)
{
    Systick_Init();
    return;
}

void App_Running(void)
{
    Lock_Init_Control();
    for (;;)
    {
        Get_Lock_State();
    }
    return;
}