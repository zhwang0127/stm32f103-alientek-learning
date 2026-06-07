/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       ATK_EC11编码器模块 测试代码
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
#include "./BSP/ATK_ENCODER/atk_encoder.h"
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
    uint8_t key, t = 0;
    int32_t last_count = 0, new_count = 0;

    atk_encoder_timx_init(0XFFFF, 0);           /* 初始化编码器相关定时器，不分频 */
    atk_encoder_key_init();                     /* 初始化编码器按键 */
    
    while (1)
    {
        key = atk_encoder_key_scan();

        if (key == ENCODER_KEY_PRESSED)         /* 编码器按键被按下 */
        {
            printf("按键被按下\r\n");
        }
        else if(key == ENCODER_KEY_LOOSE)       /* 编码器按键被松开 */
        {
            printf("按键被释放\r\n");
        }
        
        new_count = atk_encoder_get_count();    /* 读取编码器计数值 */
        
        if(new_count != last_count)             /* 当计数值与上一次不同才打印到串口 */
        {
            last_count = new_count;             /* 记录当前计数值 */
            printf("编码器当前的计数值为：%d\r\n", new_count);
            printf("编码器当前旋转的圈数为：%0.2f\r\n", (float)new_count / 80);
        }

        t++;
        delay_ms(10);

        if (t == 20)
        {
            LED0_TOGGLE();                      /* LED0闪烁, 提示系统正在运行 */
            t = 0;
        }
    }
}


