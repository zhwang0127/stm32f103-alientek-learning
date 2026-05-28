/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       空气质量传感器模块 测试代码
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
#include "./BSP/ATK_AIR/atk_air.h"
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
    float ppm;                /* 有害气体平均浓度 */
    
    atk_air_init();           /* 初始化空气质量传感器模块 */
    
    printf("空气质量传感器模块读取数据实验\r\n");
    printf("检测浓度范围：10~1000ppm\r\n");
    
    while (1)
    {
        printf("DO端输出为：%d\r\n", ATK_AIR_DO);
        
        ppm = atk_air_get_ppm();    /* 拟合函数换算出ppm */
        
        if(ppm < 10)
        {
            printf("有害气体平均浓度小于10ppm\r\n");
        }
        else if(ppm > 1000)
        {
            printf("有害气体平均浓度大于1000ppmr\n");
        }
        else
        {
            printf("有害气体的平均浓度：%0.3fppm\r\n", ppm);
        }
        
        if(ATK_AIR_DO == 0)
        {
            printf("有害气体浓度达到阈值\r\n");
        }
        else
        {
            printf("有害气体浓度未达到阈值\r\n");
        }
        
        printf("\r\n\r\n");
        
        LED0_TOGGLE(); /* 闪烁LED,提示系统正在运行. */
        
        delay_ms(500);
    }
}

