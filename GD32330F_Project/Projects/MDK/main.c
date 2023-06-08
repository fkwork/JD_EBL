/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 11:20:21
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-08 12:27:10
 * @FilePath     : main.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
#include "gd32f3x0.h"
#include "systick.h"
#include "SEGGER_RTT.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    systick_config();
    /* enable the LED GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* configure led GPIO port */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1);

    gpio_bit_reset(GPIOA, GPIO_PIN_1);

    while (1)
    {
        /* turn on led1, turn off led4 */
        gpio_bit_write(GPIOA, GPIO_PIN_1, SET);
        delay_1ms(1000);
    }
}
