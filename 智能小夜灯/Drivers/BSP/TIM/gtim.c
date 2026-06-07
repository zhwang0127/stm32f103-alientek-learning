/**
 ****************************************************************************************************
 * @file        gtim.c
 * @author      正点原子团队(ALIENTEK) / 智能小夜灯
 * @version     V1.0
 * @date        2026-01-15
 * @brief       TIM3 三路PWM驱动 (RGB LED用)
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 引脚分配: TIM3_CH1(PA6)=R, TIM3_CH2(PA7)=G, TIM3_CH3(PB0)=B
 * PWM参数: ARR=999, PSC=0 → 72MHz/1000=72kHz
 * 说明: 三路独立PWM，共用TIM3时基，用PWM模式1，极性LOW(占空比越小越暗)
 ****************************************************************************************************
 */

#include "./BSP/TIM/gtim.h"

TIM_HandleTypeDef g_tim3_pwm_handle;

/**
 * @brief       初始化TIM3三路PWM输出
 * @param       arr: 自动重装值 (推荐999)
 * @param       psc: 时钟预分频 (推荐0 → 72MHz不分频)
 * @retval      无
 */
void gtim_rgb_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef tim_oc_pwm = {0};

    g_tim3_pwm_handle.Instance = TIM3;
    g_tim3_pwm_handle.Init.Prescaler = psc;
    g_tim3_pwm_handle.Init.Period = arr;
    g_tim3_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_tim3_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_tim3_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&g_tim3_pwm_handle);

    /* 通用PWM配置 */
    tim_oc_pwm.OCMode = TIM_OCMODE_PWM1;
    tim_oc_pwm.OCPolarity = TIM_OCPOLARITY_LOW;     /* 低电平有效 */
    tim_oc_pwm.OCFastMode = TIM_OCFAST_DISABLE;
    tim_oc_pwm.Pulse = 0;                           /* 初始占空比 = 0 (全灭) */

    /* 配置三个通道 */
    HAL_TIM_PWM_ConfigChannel(&g_tim3_pwm_handle, &tim_oc_pwm, TIM_CHANNEL_1);  /* PA6: R */
    HAL_TIM_PWM_ConfigChannel(&g_tim3_pwm_handle, &tim_oc_pwm, TIM_CHANNEL_2);  /* PA7: G */
    HAL_TIM_PWM_ConfigChannel(&g_tim3_pwm_handle, &tim_oc_pwm, TIM_CHANNEL_3);  /* PB0: B */

    /* 启动三路PWM输出 */
    HAL_TIM_PWM_Start(&g_tim3_pwm_handle, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&g_tim3_pwm_handle, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&g_tim3_pwm_handle, TIM_CHANNEL_3);
}

/**
 * @brief       TIM3 PWM MSP初始化 (GPIO引脚 + 时钟)
 * @param       htim: 定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        GPIO_InitTypeDef gpio_init_struct = {0};

        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();       /* PA6, PA7 */
        __HAL_RCC_GPIOB_CLK_ENABLE();       /* PB0 */

        /* PA6: TIM3_CH1 (R) */
        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        /* PA7: TIM3_CH2 (G) */
        gpio_init_struct.Pin = GPIO_PIN_7;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        /* PB0: TIM3_CH3 (B) */
        gpio_init_struct.Pin = GPIO_PIN_0;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);
    }
}

/**
 * @brief       设置红色通道PWM值
 * @param       val: 比较值 (0~999, 越大越亮)
 * @retval      无
 */
void gtim_rgb_set_r(uint16_t val)
{
    if (val > GTIM_PWM_MAX) val = GTIM_PWM_MAX;
    __HAL_TIM_SET_COMPARE(&g_tim3_pwm_handle, TIM_CHANNEL_1, val);
}

/**
 * @brief       设置绿色通道PWM值
 * @param       val: 比较值 (0~999, 越大越亮)
 * @retval      无
 */
void gtim_rgb_set_g(uint16_t val)
{
    if (val > GTIM_PWM_MAX) val = GTIM_PWM_MAX;
    __HAL_TIM_SET_COMPARE(&g_tim3_pwm_handle, TIM_CHANNEL_2, val);
}

/**
 * @brief       设置蓝色通道PWM值
 * @param       val: 比较值 (0~999, 越大越亮)
 * @retval      无
 */
void gtim_rgb_set_b(uint16_t val)
{
    if (val > GTIM_PWM_MAX) val = GTIM_PWM_MAX;
    __HAL_TIM_SET_COMPARE(&g_tim3_pwm_handle, TIM_CHANNEL_3, val);
}

/**
 * @brief       同时设置RGB三通道
 * @param       r/g/b: 各通道比较值 (0~999)
 * @retval      无
 */
void gtim_rgb_set_all(uint16_t r, uint16_t g, uint16_t b)
{
    gtim_rgb_set_r(r);
    gtim_rgb_set_g(g);
    gtim_rgb_set_b(b);
}
