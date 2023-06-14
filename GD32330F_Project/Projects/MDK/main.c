/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:18:46
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-13 15:50:18
 * @FilePath     : main.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
#include "app.h"

/**
 *  ------------------------------------------------------------
 *                        | 智能电插锁程序 |
 *  ------------------------------------------------------------
 *  单片机型号：GD32F330F4P6TR - 84MHz - 4K SRAM - 16K Flash
 *  当前程序使用：
 *  Total RO  Size (Code + RO Data)                 5596 (5.46kB)
 *  Total RW  Size (RW Data + ZI Data)              2272 (2.22kB)
 *  Total ROM Size (Code + RO Data + RW Data)       5632 (5.50kB)
 */

int main(void)
{
    App_Init();
    while (1)
    {
        App_Running();
    }
}
