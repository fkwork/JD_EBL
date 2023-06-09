/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:36:07
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-09 09:07:15
 * @FilePath     : app.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include "app.h"
#include "port.h"
#include "timer.h"
#include <stdio.h>

DEVICE_STATE Global_LockDevice_State = {CLOSE, CLOSE, EXTEND, EXTEND, DO_CLOSE, DO_CLOSE};

/**
 *  func: 关闭锁
 */
static void Lock_Close(void)
{
    Set_DO_State(DOOR_OUT_PIN, RESET);
    Set_DO_State(LED_OUT_PIN, RESET);
    Lock_OFF_Out();
    return;
}

/**
 *  func: 打开锁
 */
static void Lock_Open(void)
{
    Set_DO_State(DOOR_OUT_PIN, SET);
    Set_DO_State(LED_OUT_PIN, SET);
    Lock_ON_Out();
    return;
}

/**
    func: 读取DI输入，获取锁状态
    ----------------------------------------------------------
    1)当门侦信号输出为低电平时, 门处于关闭状态。
    2)当门侦信号输出为高电平时, 门处于打开状态。
    ----------------------------------------------------------
    1)当锁舌侦测输出为低电平时, 锁舌处于伸出状态。
    2)当锁舌侦测输出为高电平时, 锁舌处于收回状态。
    ----------------------------------------------------------
    1)当开锁信号为 低电平 时,智能锁 开锁
    2)当开锁信号为 高电平 时,智能锁 落锁
 */
void Get_LockDevice_State(void)
{
    FlagStatus Ret = RESET;
    // 通过门磁状态判断：门开-门关
    Ret = Get_DI_State(DOOR_PIN);
    Global_LockDevice_State.DoorCurrentState = Ret ? CLOSE : OPEN;

    if (Global_LockDevice_State.DoorCurrentState != Global_LockDevice_State.LastDoorState)
    {
        if (Global_LockDevice_State.DoorCurrentState == OPEN)
            printf("Door State Change, Now Open.");
        else
            printf("Door State Change, Now Close.");
    }

    // 通过锁舌状态判断：锁开-锁关
    Ret = Get_DI_State(TONGUE_PIN);
    Global_LockDevice_State.TongueState = Ret ? INSERT : EXTEND;

    if (Global_LockDevice_State.TongueState != Global_LockDevice_State.LastTongueState)
    {
        if (Global_LockDevice_State.TongueState == INSERT)
            printf("Tongue State Change, Now Insert  [");
        else
            printf("Tongue State Change, Now Extend <[");
    }

    // 通过开门信号判断：外部开门
    Ret = Get_DI_State(OPEN_SIGNAL_PIN);
    Global_LockDevice_State.OpenSignalState = Ret ? DO_CLOSE : DO_OPEN;

    if (Global_LockDevice_State.OpenSignalState != Global_LockDevice_State.LastOpenSignalState)
    {
        if (Global_LockDevice_State.OpenSignalState == DO_OPEN)
            printf("Open Signal Change, Now Do Open.");
        else
            printf("Open Signal Change, Now Do Close.");
    }
    return;
}

static void Lock_Init_Control(void)
{
    uint8_t index = 0;
    for (index = 0; index < 50; index++)
    {
        Get_LockDevice_State();
        Delay_Ms(5);
    }

    // 上电时，无门磁反馈, 电子锁自动开锁
    if (Global_LockDevice_State.DoorCurrentState == OPEN)
    {
        Lock_Open();
    }
    return;
}

/**
    |==================================================================================|
        4.1开锁逻辑
            4.1.1上电时，无门磁反馈
                条件：上电时，无门磁反馈
                动作：电子锁自动开锁。
            4.1.2门关时，接收到开锁信号
                条件：电子锁供电正常，有门磁反馈（门关），无锁舌反馈。将开锁信号接地。
                动作：锁舌收回。
    |==================================================================================|
    |==================================================================================|
        4.2关锁逻辑
            4.2.1开锁后，门打开，在关门
                条件：电子锁打开，且打开门（门磁无反馈）时，将门磁片对准电子锁，确保有门磁反馈
                动作：维持3s后，电子锁自动关锁。
            4.2.2开锁后，门未开
                条件：将电子锁开锁，且门磁反馈一直有（门未打开）
                动作：维持10s，电子锁自动关锁。
    |==================================================================================|
**/

bool DoorHaveOpenState = FALSE;
bool DoorOpenToCloseState = FALSE;
CTRL_TIMESTAMP CtrlTimestamp = {0, 0};

static void Lock_Run_Control(void)
{
    // 接收到开锁信号, 锁舌收回, 开锁
    if (Global_LockDevice_State.OpenSignalState == DO_OPEN)
    {
        if (Global_LockDevice_State.TongueState == EXTEND)
        {
            Lock_Open();
            CtrlTimestamp.LastValue = Get_Current_TC();
        }
    }

    // 开锁后，门未开
    if (DoorHaveOpenState == FALSE && DoorOpenToCloseState == FALSE && Global_LockDevice_State.TongueState == INSERT && Global_LockDevice_State.DoorCurrentState == CLOSE)
    {
        CtrlTimestamp.CurValue = Get_Current_TC();
        if (Try_Time_Out(CtrlTimestamp.CurValue, CtrlTimestamp.LastValue, TIME_DELAY_DOOR_CLOSE_NO_OPEN))
        {
            Lock_Close();
        }
    }

    if (Global_LockDevice_State.TongueState == INSERT && Global_LockDevice_State.DoorCurrentState == OPEN)
    {
        DoorHaveOpenState = TRUE;
    }

    if (DoorHaveOpenState == TRUE && Global_LockDevice_State.TongueState == INSERT && Global_LockDevice_State.DoorCurrentState == CLOSE)
    {
        CtrlTimestamp.LastValue = Get_Current_TC();
        DoorHaveOpenState = FALSE;
        DoorOpenToCloseState = TRUE;
    }

    if (DoorOpenToCloseState == TRUE)
    {
        CtrlTimestamp.CurValue = Get_Current_TC();
        if (Try_Time_Out(CtrlTimestamp.CurValue, CtrlTimestamp.LastValue, TIME_DELAY_DOOR_OPEN_THEN_CLOSE))
        {
            Lock_Close();
            DoorOpenToCloseState = FALSE;
        }
    }

    return;
}

void App_Init(void)
{
    Systick_Init();
    Port_Init();
    Timer15_Init();
    Lock_PWM_Init();
    return;
}

void App_Running(void)
{
    Lock_Init_Control();
    for (;;)
    {
        Get_LockDevice_State();
        Lock_Run_Control();
    }
    return;
}
