/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:36:16
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-09 08:47:35
 * @FilePath     : timer.h
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
#ifndef _TIMER_H_
#define _TIMER_H_

#include "gd32f3x0.h"
#include "systick.h"

void Timer15_Init(void);
uint32_t Get_Current_TC(void);
uint8_t Try_Time_Out(uint32_t cur_time, uint32_t last_time, uint32_t u_tick);

void Lock_PWM_Init(void);
void Lock_ON_Out(void);
void Lock_OFF_Out(void);

#endif
