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



/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_3;
    gpio_init_struct.Mode  = GPIO_MODE_IT_FALLING;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    
    HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 2);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

///* EXTI3外部中断服务函数 */
//void EXTI3_IRQHandler(void)
//{
//    if (__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3) != 0)
//    {
//        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
//        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
//    }
//}

/* EXTI3外部中断服务函数 */
void EXTI3_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

/* 外部中断回调函数 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_3)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
    }
}
