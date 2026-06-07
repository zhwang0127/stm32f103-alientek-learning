/**
 ****************************************************************************************************
 * @file        gtim.h
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
 * PWM频率: 72MHz / 1 / 1000 = 72kHz, 1000级精度
 ****************************************************************************************************
 */

#ifndef __GTIM_H
#define __GTIM_H

#include "./SYSTEM/sys/sys.h"

#define GTIM_PWM_MAX    999     /* PWM最大比较值 (ARR=999, 0~999共1000级) */

extern TIM_HandleTypeDef g_tim3_pwm_handle;

void gtim_rgb_init(uint16_t arr, uint16_t psc);     /* 初始化TIM3三路PWM */
void gtim_rgb_set_r(uint16_t val);                   /* 设置红色通道 (0~999) */
void gtim_rgb_set_g(uint16_t val);                   /* 设置绿色通道 (0~999) */
void gtim_rgb_set_b(uint16_t val);                   /* 设置蓝色通道 (0~999) */
void gtim_rgb_set_all(uint16_t r, uint16_t g, uint16_t b); /* 同时设置RGB */

#endif
