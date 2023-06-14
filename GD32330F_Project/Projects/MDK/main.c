/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:18:46
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-14 09:56:12
 * @FilePath     : main.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
#include "app.h"

/**
 *  ------------------------------------------------------------
 *                        < 智能电插锁程序 >
 *                     Build Times: 2023-06-14
 *  ------------------------------------------------------------
 *  单片机型号：GD32F330F4P6TR - 84MHz - 4K SRAM - 16K Flash
 *  ------------------------------------------------------------
 */

int main(void)
{
    App_Init();
    while (1)
    {
        App_Running();
    }
}
