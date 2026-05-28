/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       外部中断 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./BSP/LED/led.h"
#include "./BSP/EXTI/exti.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    /* 循迹模块DO端外部中断检查初始化 */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_3;
    gpio_init_struct.Mode = GPIO_MODE_IT_FALLING;   /* 下降沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;            /* 上拉（配合外部上拉更稳定触发） */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 2);         /* 抢占1，子优先级2 */
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);                 /* 使能中断线1 */
    
    
    /* PB7按键外部中断检查初始化 */
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    gpio_init_struct.Pin = GPIO_PIN_7;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;    /* 上升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLDOWN;          /* 下拉 */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 2);       /* 抢占2，子优先级2 */
    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);               /* 使能中断线1 */
}

/**
 * @brief       EXTI3外部中断服务程序
 * @param       无
 * @retval      无
 */
void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);   /* 调用中断处理公用函数 清除EXTI3 的中断标志位 */
}

/**
 * @brief       EXTI9_5外部中断服务程序
 * @param       无
 * @retval      无
 */
void EXTI9_5_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);   /* 调用中断处理公用函数 清除EXTI7 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_7);   /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);    /* 退出中断前熄灭LED灯 */
}

/**
 * @brief       中断服务程序中需要做的事情
                在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    }
    
    delay_ms(10);      /* 消抖 */
    
    if (GPIO_Pin == GPIO_PIN_7)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);    /* 点亮LED灯 */
        delay_ms(5000);      /* 亮5秒，实际应用尽量不要在中断程序使用延时函数，此处为了演示 */
    }
}

