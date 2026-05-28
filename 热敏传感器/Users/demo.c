/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       热敏传感器 测试代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "demo.h"
#include "./BSP/ATK_NTC/atk_ntc.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/ADC/adc.h"


/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint16_t adcx;
    float temp;
    
    atk_ntc_init();                                                             /* 初始化热敏传感器 */
    
    while (1)
    {
        /* 显示热敏电阻的ADC数值 */
        adcx = adc_get_result_average(ADC_ADCX_CHY, 10);                        /* 获取ADC通道的转换值，10次取平均 */
        temp = (float)adcx * (3.3 / 4096);                                      /* 获取计算后的带小数的实际电压值，比如3.1111 */
        printf("热敏电阻传感器 AO模拟量：%d, 电压值：%.2fV\r\n", adcx, temp);   /* 打印AO处电压值 */
        printf("NTC温度值：%.2f℃\r\n", atk_ntc_get_temp(adcx));                 /* 打印温度值 */
        
        /* 温度超过预设阈值时的操作 */
        if(ATK_NTC_DO == GPIO_PIN_RESET)
        {
            printf("温度超过阈值！\r\n");
        }else
        {
            printf("温度未超过阈值！\r\n");
        }
        printf("\r\n");
        
        delay_ms(500);
        LED0_TOGGLE();
    }
}


