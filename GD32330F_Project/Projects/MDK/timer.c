/*
 * @Author       : wang chao
 * @Date         : 2023-06-08 12:36:07
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-06-13 10:41:38
 * @FilePath     : timer.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */
/*
 * @Author       : wang chao
 * @Date         : 2023-05-22 08:51:56
 * @LastEditors  : wang chao
 * @LastEditTime : 2023-05-30 09:45:19
 * @FilePath     : timer.c
 * @Description  :
 * Copyright 2023 BingShan, All Rights Reserved.
 */

#include "timer.h"
#include <stdio.h>

static uint32_t t_count = 0;

void Timer15_Init(void)
{
    /* ----------------------------------------------------------------------------
    TIMER2 Configuration:
    TIMER2CLK = SystemCoreClock/7200 = 10KHz,the period is 1s(9999+1/10000).
    ---------------------------------------------------------------------------- */
    timer_oc_parameter_struct timer_ocinitpara;
    timer_parameter_struct timer_initpara;

    /* enable the peripherals clock */
    rcu_periph_clock_enable(RCU_TIMER15);

    /* deinit a TIMER */
    timer_deinit(TIMER15);
    /* initialize TIMER init parameter struct */
    timer_struct_para_init(&timer_initpara);
    /* TIMER2 configuration */
    timer_initpara.prescaler = 8399;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 9999;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_init(TIMER15, &timer_initpara);

    /* clear channel 0 interrupt bit */
    timer_interrupt_flag_clear(TIMER15, TIMER_INT_FLAG_UP);
    /* enable the TIMER interrupt */
    timer_interrupt_enable(TIMER15, TIMER_INT_UP);
    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER15);

    /* enable a TIMER */
    timer_enable(TIMER15);

    nvic_irq_enable(TIMER15_IRQn, 0, 2);
}

void TIMER15_IRQHandler(void)
{
    if (SET == timer_interrupt_flag_get(TIMER15, TIMER_INT_FLAG_UP))
    {
        t_count++;
        timer_interrupt_flag_clear(TIMER15, TIMER_INT_FLAG_UP);
    }
}

uint32_t Get_Current_TC(void)
{
    return t_count;
}

uint8_t Try_Time_Out(uint32_t cur_time, uint32_t last_time, uint32_t u_tick)
{
    if ((cur_time - last_time) > u_tick)
    {
        return 1;
    }
    return 0;
}

/**
 *  初始设定: 16Khz
 *
 *  PWM频率计算方法：200000/(timer_initpara.period + 1)
 *  ---------------------------------------------------
 *  例如：10Khz PWM => 200000/100000 = (199+1)
 *  占空比设定范围：[0 - 200] => [0% - 100%]
 *  ---------------------------------------------------
 */
void Lock_PWM_Init(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_0);

    rcu_periph_clock_enable(RCU_TIMER1);
    timer_deinit(TIMER1);
    /* TIMER1 configuration */
    timer_initpara.prescaler = 41;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 124;
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1, &timer_initpara);
    /* CH0 configuration in PWM mode1 */
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_HIGH;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER1, TIMER_CH_0, &timer_ocintpara);
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 0);
    timer_channel_output_mode_config(TIMER1, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_primary_output_config(TIMER1, ENABLE);
    /* auto-reload preload enable */
    timer_enable(TIMER1);
    return;
}

uint8_t pulse_value = 84;

/*
    PWM 占空比100时，锁舌吸合，锁开
    PWM 占空比  0时，锁舌弹出，锁关
*/
void Lock_ON_Out(void)
{
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, pulse_value);
    return;
}

void Lock_OFF_Out(void)
{
    timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_0, 13);
    return;
}
