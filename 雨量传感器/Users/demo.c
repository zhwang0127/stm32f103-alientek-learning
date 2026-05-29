/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       雨量传感器 测试代码
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
#include "./BSP/ATK_RAIN/atk_rain.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/ADC/adc.h"

   
#define RAIN_NO         0       /* 无雨阈值 */
#define RAIN_HEAVY      1500    /* 有雨阈值 */

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint16_t adcx;
    float temp;
    
    atk_rain_init();                                                          /* 初始化雨量传感器 */
        
    while (1)   
    {
        adcx = adc_get_result_average(ADC_ADCX_CHY, 10);                      /* 获取ADC通道的转换值，10次取平均 */
        temp = (float)adcx * (3.3 / 4096);                                    /* 获取计算后的带小数的实际电压值，比如3.1111 */
        printf("雨量传感器 AO模拟量：%d, 电压值：%.2fV\r\n", adcx, temp);     /* 打印AO处电压值 */
        
        if(adcx == RAIN_NO)
        {
            printf("没下雨\r\n"); 
        }
        else if(adcx <= RAIN_HEAVY)
        {
            printf("有少量雨水或未插干净\r\n"); 
        }
        else if(adcx > RAIN_HEAVY)
        {
            printf("有雨，雨量占比：%d%%\r\n", atk_rain_get_val()); /* 打印雨量占比 */  
        }
        printf("\r\n");
        
        delay_ms(500);
        LED0_TOGGLE();
    }
}


