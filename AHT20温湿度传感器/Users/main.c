/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    EEPROM读写实验 — 24C02 按键写入/读取数据
 * @copyright Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 * 
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/ATK_AHT20/atk_aht20.h"


int main(void)
{                                    
    float temp = 0,humi = 0;
    HAL_Init();                                         /* STM32 HAL 库初始化 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                 /* 系统时钟: 72MHz */
    delay_init(72);                                     /* 滴答定时器初始化 */
    usart_init(115200);                                 /* 串口1初始化 */
    led_init();                                         /* 板载 LED (PA8) 初始化 */
    key_init();                                         /* 按键 GPIO 初始化 */        
    atk_aht20_init();
    
    while (1)
    {
        atk_aht20_read_data(&temp,&humi);
        printf("温度：%.2f 摄氏度， 湿度：%.2f%%\r\n",temp,humi);
        delay_ms(10);
    }

}
