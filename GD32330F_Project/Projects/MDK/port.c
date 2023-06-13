/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:54:34
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-13 15:54:50
 * @FilePath     : port.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include "port.h"

void Port_DI_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOF);
    // DOOR STATE
    gpio_mode_set(GPIOF, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_1);

    // TONGUE STATE
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    // OPEN SIGNAL STATE
    gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    return;
}

void Port_DO_Init(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1 | GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_9);
    gpio_bit_reset(GPIOA, GPIO_PIN_1 | GPIO_PIN_9);

    rcu_periph_clock_enable(RCU_GPIOB);
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_bit_reset(GPIOB, GPIO_PIN_1);
    return;
}

void Port_Init(void)
{
    Port_DI_Init();
    Port_DO_Init();
}

FlagStatus Get_DI_State(LOCK_DI_PIN port)
{
    switch (port)
    {
    case DOOR_PIN:
        if (gpio_input_bit_get(GPIOF, GPIO_PIN_1) == 0)
        {
            Delay_Ms(100);
            if (gpio_input_bit_get(GPIOF, GPIO_PIN_1) == 0)
            {
                return RESET;
            }
        }
        return SET;
    case TONGUE_PIN:
        if (gpio_input_bit_get(GPIOA, GPIO_PIN_10) == 0)
        {
            Delay_Ms(100);
            if (gpio_input_bit_get(GPIOA, GPIO_PIN_10) == 0)
            {
                return RESET;
            }
        }
        return SET;
    case OPEN_SIGNAL_PIN:
        if (gpio_input_bit_get(GPIOA, GPIO_PIN_2) == 0)
        {
            Delay_Ms(100);
            if (gpio_input_bit_get(GPIOA, GPIO_PIN_2) == 0)
            {
                return RESET;
            }
        }
        return SET;
    default:
        break;
    }
    return RESET;
}

void Set_DO_State(LOCK_DO_PIN port, FlagStatus state)
{
    switch (port)
    {
    case DOOR_OUT_PIN:
        state ? gpio_bit_set(GPIOA, GPIO_PIN_1) : gpio_bit_reset(GPIOA, GPIO_PIN_1);
        break;
    case LED_OUT_PIN:
        state ? gpio_bit_set(GPIOA, GPIO_PIN_9) : gpio_bit_reset(GPIOA, GPIO_PIN_9);
        break;
    default:
        break;
    }
    return;
}
