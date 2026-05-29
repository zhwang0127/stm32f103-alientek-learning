/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       土壤湿度传感器 测试代码
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
#include "./BSP/ATK_SOIL_HUMIDITY/atk_soil_humidity.h"
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
    
    atk_soil_humidity_init();                                                 /* 初始化土壤湿度传感器 */
        
    while (1)   
    {
        adcx = adc_get_result_average(ADC_ADCX_CHY, 10);                      /* 获取ADC通道的转换值，10次取平均 */
        temp = (float)adcx * (3.3 / 4096);                                    /* 获取计算后的带小数的实际电压值，比如3.1111 */
        printf("土壤湿度传感器 AO模拟量：%d, 电压值：%.2fV\r\n", adcx, temp);   /* 打印AO处电压值 */
        printf("土壤湿度占比：%d%%\r\n", atk_soil_humidity_get_val());         /* 打印土壤湿度占比 */   
        printf("\r\n");
        
        delay_ms(500);
        LED0_TOGGLE();
    }
}


