/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       OLED模块 测试代码
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
#include "./BSP/ATK_OLED/atk_oled.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"


/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{    
    uint8_t t = 0;
    
    atk_oled_init();                            /* 初始化OLED */
    
    atk_oled_show_string(0, 0, "ALIENTEK", 24);
    atk_oled_show_string(0, 24, "0.96' OLED TEST", 16);
    atk_oled_show_string(0, 52, "ASCII:", 12);
    atk_oled_show_string(64, 52, "CODE:", 12);
    atk_oled_refresh_gram();                    /* 更新显示到OLED */

    t = ' ';
    while (1)
    {
        atk_oled_show_char(36, 52, t, 12, 1);   /* 显示ASCII字符 */
        atk_oled_show_num(94, 52, t, 3, 12);    /* 显示ASCII字符的码值 */
        atk_oled_refresh_gram();                /* 更新显示到OLED */
        t++;

        if (t > '~')
        {
            t = ' ';
        }

        delay_ms(500);
        LED0_TOGGLE();                      /* LED0闪烁 */
    }
}


