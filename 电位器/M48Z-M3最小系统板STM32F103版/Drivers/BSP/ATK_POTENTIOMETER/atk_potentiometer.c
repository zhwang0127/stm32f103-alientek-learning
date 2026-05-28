/**
 ****************************************************************************************************
 * @file        atk_potentiometer.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       电位器模块 驱动代码
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

#include "./BSP/ATK_POTENTIOMETER/atk_potentiometer.h"
#include "./BSP/ADC/adc.h"


/**
 * @brief       电位器模块初始化函数
 * @param       无
 * @retval      无
 */
void atk_potentiometer_init(void)
{
    adc_init();     /* 初始化ADC */
}

/**
 * @brief       读取AO端电压转换值
 * @param       无
 * @retval      AO端电压转换值
 */
uint32_t get_voltage_value(void)
{
    return adc_get_result_average(ADC_ADCX_CHY, 10);
}



