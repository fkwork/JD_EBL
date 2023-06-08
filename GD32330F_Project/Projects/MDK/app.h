/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:23:49
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-08 14:24:24
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
    OFF = 0,
    ON  = !OFF
} LockStatus;
typedef struct
{
    LockStatus DoorState;
    LockStatus TongueState;
    LockStatus OpenSignalState
} LOCK_STATE;

extern LOCK_STATE Global_Lock_State;

extern void App_Init(void);
extern void App_Running(void);

#endif
