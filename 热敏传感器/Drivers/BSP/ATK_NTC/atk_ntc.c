/**
 ****************************************************************************************************
 * @file        atk_ntc.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       热敏传感器 驱动代码
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

#include "./BSP/ATK_NTC/atk_ntc.h"
#include "./BSP/ADC/adc.h"
#include "math.h"


/**
 * @brief       热敏传感器模块初始化函数
 * @param       无
 * @retval      无
 */
void atk_ntc_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    ATK_NTC_DO_GPIO_CLK_ENABLE();                               /* DO时钟使能 */

    gpio_init_struct.Pin = ATK_NTC_DO_GPIO_PIN;                 /* DO引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* 输入 */
    gpio_init_struct.Pull = GPIO_NOPULL;                        /* 无上下拉 */
    HAL_GPIO_Init(ATK_NTC_DO_GPIO_PORT, &gpio_init_struct);     /* DO引脚初始化 */

    adc_init();                                                 /* 初始化ADC */
}

/*
    Rb = Ra / exp(B*(1/Ta-1/Tb))
    Rb 是热敏电阻在Tb温度下的阻值；
    Ra 是热敏电阻在Ta常温下的标称阻值；
    exp是e的n次方，e是自然常数，就是自然对数的底数，近似等于 2.7182818；
    B值是热敏电阻的重要参数，教程中用到的热敏电阻B值为3950；
    这里Ta和Tb指的是开尔文温度，Ta是常温25℃，即(273.15+25)K
    Tb就是所求的温度
*/

const float Ra = 10000.0f;                  /* 25℃下的标称阻值10K */
const float Ta = (273.15f + 25.0f);         /* 25℃下的开尔文温度Ta */
const float Bx = 3950.0f;                   /* B */
const float Ka = 273.15f;

/**
 * @brief       计算温度值
 * @param       para: 温度采集对应ADC通道的值（已滤波）
 * @note        计算温度分为两步：
                1.根据ADC采集到的值计算当前对应的Rb
                2.根据Rb计算对应的温度值
 * @retval      温度值
 */
float atk_ntc_get_temp(uint16_t para)
{
    float Rb;
    float temp;
    float v1, v2;
    v1 = (para * 3.3f / 4096.0f);   /* 热敏电阻的分压 */
    v2 = 3.3f - v1;                 /* 得到上拉电阻10K的分压 */
    Rb = v1 / (v2 / 10000.0f);      /* 计算出热敏电阻的阻值 */    

    temp = Ra/Rb;
    temp = log(temp);               /* ln(Ra/Rb) */
    temp /= Bx;                     /* ln(Ra/Rb)/B */
    temp = (1.0f / Ta) - temp;           
    temp = 1.0f / temp;
    temp -= Ka;                     /* 转成摄氏度 */
    return temp;
}

