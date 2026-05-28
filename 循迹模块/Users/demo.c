/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       寻迹模块 测试代码
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
#include "./BSP/ATK_TRACK/atk_track.h"
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
    atk_track_init();                       /* 初始化寻迹模块 */
    
    while (1)
    {
        printf("循迹模块 DO端输出为：%d\r\n", ATK_TRACK_DO); 
        printf("循迹模块 AO端电压转换值为：%d\r\n", atk_track_adc_converted_value());
        
        if(ATK_TRACK_DO == 0)
        {
            printf("检测到物体\r\n");
        }
        else
        {
            printf("检测到黑线或者没有检测到物体\r\n");
        }
        
        printf("\r\n\r\n");
        
        LED0_TOGGLE(); /* 闪烁LED,提示系统正在运行. */
        
        delay_ms(500);
    }
}


