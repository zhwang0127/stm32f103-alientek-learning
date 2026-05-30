/**
 ****************************************************************************************************
 * @file        atk_fan.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       风扇模块 驱动代码
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

#include "./BSP/ATK_FAN/atk_fan.h"
#include "./BSP/TIM/atim.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       风扇模块初始化函数
 * @param       无
 * @retval      无
 */
void atk_fan_init(void)
{
    atim_timx_cplm_pwm_init(50 - 1, 72 - 1);     /* 1Mhz的计数频率,20Khz的PWM */
    atk_fan_stop();
}

/**
 * @brief       电机停止
 * @param       无
 * @retval      无
 */
void atk_fan_stop(void)
{
    HAL_TIM_PWM_Stop(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
    HAL_TIMEx_PWMN_Stop(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */
}

/**
 * @brief       电机旋转方向设置
 * @param       para:方向 0正转，1反转
 * @note        以电机正面，顺时针方向旋转为正转
 * @retval      无
 */
void atk_fan_dir(uint8_t para)
{
    HAL_TIM_PWM_Stop(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);          /* 关闭主通道输出 */
    HAL_TIMEx_PWMN_Stop(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);       /* 关闭互补通道输出 */

    if (para == 0)                /* 正转 */
    {
        HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);     /* 开启主通道输出 */
    } 
    else if (para == 1)           /* 反转 */
    {
        HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1);  /* 开启互补通道输出 */
    }
}

/**
 * @brief       电机速度设置
 * @param       para:比较寄存器值
 * @retval      无
 */
void atk_fan_set_speed(uint16_t para)
{
    uint16_t arr = __HAL_TIM_GetAutoreload(&g_timx_cplm_pwm_handle);

    if (para > arr) para = arr;                 /* 上限为ARR (49), 对应~98%占空比 */
    __HAL_TIM_SetCompare(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1, para);
}

/**
 * @brief       电机控制 (带低速启动补偿)
 * @param       para: pwm比较值 ,正数电机为正转，负数为反转
 * @note        低于15的值会先短暂输出20占空比起动, 然后降到目标值
 *              解决有刷电机低速时静摩擦力过大无法起动的问题
 * @retval      无
 */
void atk_fan_pwm_set(float para)
{
    int val = (int)para;
    int abs_val = (val >= 0) ? val : -val;

    if (val >= 0) 
    {
        atk_fan_dir(0);           /* 正转 */
    } 
    else 
    {
        atk_fan_dir(1);           /* 反转 */
    }

    /* 低速启动补偿: 目标<15时先给一个20的起动脉冲 */
    if (abs_val > 0 && abs_val < 15)
    {
        __HAL_TIM_SetCompare(&g_timx_cplm_pwm_handle, TIM_CHANNEL_1, 20);
        delay_ms(80);                               /* 持续80ms让电机转起来 */
    }

    atk_fan_set_speed(abs_val);                     /* 降到目标速度 */
}

