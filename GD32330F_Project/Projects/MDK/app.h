/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:23:49
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-14 08:45:13
 * @FilePath     : app.h
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#ifndef _APP_H_
#define _APP_H_

#include "gd32f3x0.h"
#include "systick.h"
#include "SEGGER_RTT.h"

typedef enum
{
    CLOSE = 0,
    OPEN = !CLOSE
} DoorStatus;

typedef enum
{
    INSERT = 0,
    EXTEND = !INSERT
} TongueStatus;

typedef enum
{
    DO_OPEN = 0,
    DO_CLOSE = !DO_OPEN
} OpenSignalStatus;

typedef enum
{
    WAIT_SIGNAL = 0,
    RESP_SIGNAL,
    HAVE_RESP_SIGNAL
} OpenSignalLongGiveStatus;

typedef struct
{
    DoorStatus DoorCurrentState;
    DoorStatus LastDoorState;
    TongueStatus TongueState;
    TongueStatus LastTongueState;
    OpenSignalStatus OpenSignalState;
    OpenSignalStatus LastOpenSignalState;
    OpenSignalLongGiveStatus OpenSignalLongGive;
} DEVICE_STATE;

typedef struct
{
    uint32_t CurValue;
    uint32_t LastValue;
} CTRL_TIMESTAMP;

#define TIME_DELAY_DOOR_CLOSE_NO_OPEN 10u
#define TIME_DELAY_DOOR_OPEN_THEN_CLOSE 3u

extern DEVICE_STATE Global_LockDevice_State;
extern void OpenSignalLongGiveProcess(void);

extern void App_Init(void);
extern void App_Running(void);

#endif
