/**
 ****************************************************************************************************
 * @file        atk_soil_humidity.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       土壤湿度传感器 驱动代码
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

#include "./BSP/ATK_SOIL_HUMIDITY/atk_soil_humidity.h"
#include "./BSP/ADC/adc.h"


/**
 * @brief       土壤湿度传感器模块初始化函数
 * @param       无
 * @retval      无
 */
void atk_soil_humidity_init(void)
{
    adc_init();                                             /* 初始化ADC */
}

/**
 * @brief       读取土壤湿度百分比
 * @param       无
 * @retval      0~100:0,湿度占比最小; 100,湿度占比最大
 */
uint8_t atk_soil_humidity_get_val(void)
{
    uint32_t temp_val = 0;
    temp_val = adc_get_result_average(ADC_ADCX_CHY, 10);    /* 读取平均值 */
    temp_val /= 40;

    if (temp_val > 100)temp_val = 100;

    return (uint8_t)(temp_val);
}
