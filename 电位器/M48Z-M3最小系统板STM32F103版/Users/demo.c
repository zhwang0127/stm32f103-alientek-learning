/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       电位器模块 测试代码
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

#include "demo.h"
#include "./BSP/ATK_POTENTIOMETER/atk_potentiometer.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include <stdio.h>


/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    atk_potentiometer_init();               /* 初始化电位器模块 */
    
    while (1)
    {
        printf("电位器模块 AO端ADC值为：%d\r\n", get_voltage_value());
        printf("电位器模块 AO端电压值为：%.2fV\r\n", (float)get_voltage_value() * (3.3 / 4096));
        
        printf("\r\n\r\n");
        
        LED0_TOGGLE();                      /* 闪烁LED,提示系统正在运行 */
        
        delay_ms(500);
    }
}


