/**
 ****************************************************************************************************
 * @file        atk_encoder.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       ATK_EC11编码器模块 驱动代码
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

#ifndef __ATK_ENCODER_H
#define __ATK_ENCODER_H

#include "./SYSTEM/sys/sys.h"


/**************************************************************************************************/
/* TIMX 编码器接口定义 */

/* 编码器A相引脚定义 */
#define ATK_ENCODER_TIMX_CH1_GPIO_PORT         GPIOA
#define ATK_ENCODER_TIMX_CH1_GPIO_PIN          GPIO_PIN_6
#define ATK_ENCODER_TIMX_CH1_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA口时钟使能 */

/* 编码器B相引脚定义 */
#define ATK_ENCODER_TIMX_CH2_GPIO_PORT         GPIOA
#define ATK_ENCODER_TIMX_CH2_GPIO_PIN          GPIO_PIN_7
#define ATK_ENCODER_TIMX_CH2_GPIO_CLK_ENABLE() do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA口时钟使能 */

#define ATK_ENCODER_TIMX                       TIM3                                                    /* TIM3 */
#define ATK_ENCODER_TIMX_INT_IRQn              TIM3_IRQn
#define ATK_ENCODER_TIMX_INT_IRQHandler        TIM3_IRQHandler
#define ATK_ENCODER_TIMX_CH1                   TIM_CHANNEL_1                                           /* 通道1*/
#define ATK_ENCODER_TIMX_CH1_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)              /* TIM3 时钟使能 */

#define ATK_ENCODER_TIMX_CH2                   TIM_CHANNEL_2                                           /* 通道2*/
#define ATK_ENCODER_TIMX_CH2_CLK_ENABLE()      do{ __HAL_RCC_TIM3_CLK_ENABLE(); }while(0)              /* TIM3 时钟使能 */


/**************************************************************************************************/
/* 编码器按键引脚定义 */

#define ATK_ENCODER_KEY_GPIO_PORT                  GPIOA
#define ATK_ENCODER_KEY_GPIO_PIN                   GPIO_PIN_5
#define ATK_ENCODER_KEY_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)          /* PA口时钟使能 */

#define ATK_ENCODER_KEY         HAL_GPIO_ReadPin(ATK_ENCODER_KEY_GPIO_PORT, ATK_ENCODER_KEY_GPIO_PIN)   /* 读取编码器按键引脚 */

#define ENCODER_KEY_PRESSED     1                               /* 编码器按键引脚被按下 */
#define ENCODER_KEY_LOOSE       2                               /* 编码器按键引脚被松开 */

/******************************************************************************************/

void atk_encoder_timx_init(uint16_t arr, uint16_t psc);         /* 编码器定时器初始化 */
int32_t atk_encoder_get_count(void);                            /* 获取编码器总计数值 */
void atk_encoder_key_init(void);                                /* 编码器按键初始化函数 */
uint8_t atk_encoder_key_scan(void);                             /* 编码器按键扫描函数 */

#endif





