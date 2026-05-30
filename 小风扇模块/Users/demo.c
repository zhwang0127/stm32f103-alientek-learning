/**
 ****************************************************************************************************
 * @file        demo.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       风扇模块 测试代码
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
#include "./BSP/ATK_FAN/atk_fan.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"


#define MAX_SPEED   50          /* 限制比较值 即最大速度 */

/**
 * @brief       例程演示入口函数
 * @param       无
 * @retval      无
 */
void demo_run(void)
{
    uint8_t key, t;
    int32_t pwm_val = 0;
    
    key_init();                 /* 初始化按键 */
    atk_fan_init();             /* 初始化风扇模块 */
    
    while (1)
    {
        key = key_scan(1);      /* 支持连按，效果更顺滑 */
        if(key == KEY0_PRES)                    /* 减速 */
        {
            pwm_val -= 1;
            if(pwm_val <= -MAX_SPEED)
            {
                pwm_val = -MAX_SPEED;
            }
            atk_fan_pwm_set(pwm_val);
        }
        else if(key == WKUP_PRES)               /* 加速 */
        {
            pwm_val += 1;
            if(pwm_val >= MAX_SPEED)
            {
                pwm_val = MAX_SPEED;
            }
            atk_fan_pwm_set(pwm_val);
        }
        
        delay_ms(10);
        t++;
        if(t % 50 == 0)
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}


