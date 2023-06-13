/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:18:46
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-12 10:40:25
 * @FilePath     : main.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
#include "app.h"

int main(void)
{
    App_Init();
    while (1)
    {
        App_Running();
    }
}