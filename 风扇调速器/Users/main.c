/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2026-05-30
 * @brief    串口风扇调速实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ******************************************************************************
 * @attention
 *
 * 实验平台:正点原子 M48Z-M3最小系统板STM32F103版
 * 硬件模块:ATK-MB022 OLED + ATK-MB023 小风扇
 * 功能: 串口调速 + OLED中文显示 + 串口中文回显
 ******************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/ATK_FAN/atk_fan.h"
#include "./BSP/ATK_OLED/atk_oled.h"
#include "./BSP/ATK_OLED/chinese_font.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    uint16_t len;
    int val;
    int32_t speed = 0;
    uint8_t dir = 0;                                /* 0=正转, 1=反转 */
    uint8_t percent;
    char buf[16];

    HAL_Init();                                     /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);             /* 设置时钟, 72Mhz */
    delay_init(72);                                 /* 延时初始化 */
    usart_init(115200);                             /* 串口初始化为115200bps */
    led_init();                                     /* 初始化LED */
    atk_oled_init();                                /* 初始化OLED */
    atk_fan_init();                                 /* 初始化风扇 */

    /* OLED 欢迎界面 */
    {
        uint8_t idx1[] = {CN_FENG, CN_SHAN};       /* 风扇 */
        uint8_t idx2[] = {CN_ZHUAN, CN_SU};        /* 转速 */
        uint8_t idx3[] = {CN_TING};                /* 停 */

        atk_oled_clear();
        atk_oled_show_chinese_str(0, 0, idx1, 2);
        atk_oled_show_string(32, 0, "UART Ctrl", 16);
        atk_oled_show_chinese_str(0, 24, idx2, 2);
        atk_oled_show_string(32, 24, ": --", 16);
        atk_oled_show_chinese_str(72, 48, idx3, 1);
        atk_oled_refresh_gram();
    }

    printf("\r\n风扇串口调速系统 V1.0\r\n");
    printf("命令: 0~50 / SPD:XX / STOP / FWD / REV\r\n\r\n");

    while (1)
    {
        /* 检查串口命令 */
        if (g_usart_rx_sta & 0x8000)
        {
            len = g_usart_rx_sta & 0x3FFF;
            g_usart_rx_buf[len] = '\0';

            printf("收到: %s\r\n", g_usart_rx_buf);

            /* 命令解析 */
            if (strncmp((char *)g_usart_rx_buf, "STOP", 4) == 0
             || strncmp((char *)g_usart_rx_buf, "stop", 4) == 0)
            {
                speed = 0;
                atk_fan_stop();
                printf(">> 已停止\r\n");
            }
            else if (strncmp((char *)g_usart_rx_buf, "FWD", 3) == 0
                  || strncmp((char *)g_usart_rx_buf, "fwd", 3) == 0)
            {
                dir = 0;
                if (speed == 0) speed = 10;
                atk_fan_pwm_set(speed);
                printf(">> 正转\r\n");
            }
            else if (strncmp((char *)g_usart_rx_buf, "REV", 3) == 0
                  || strncmp((char *)g_usart_rx_buf, "rev", 3) == 0)
            {
                dir = 1;
                if (speed >= 0) speed = -speed;
                if (speed == 0) speed = -10;
                atk_fan_pwm_set(speed);
                printf(">> 反转\r\n");
            }
            else if (strncmp((char *)g_usart_rx_buf, "SPD:", 4) == 0
                  || strncmp((char *)g_usart_rx_buf, "spd:", 4) == 0)
            {
                val = atoi((char *)g_usart_rx_buf + 4);
                if (val < 0) val = 0;
                if (val > 50) val = 50;
                speed = (dir == 1) ? -val : val;
                atk_fan_pwm_set(speed);
                printf(">> 转速: %d/50\r\n", val);
            }
            else if (g_usart_rx_buf[0] >= '0' && g_usart_rx_buf[0] <= '9')
            {
                val = atoi((char *)g_usart_rx_buf);
                if (val < 0) val = 0;
                if (val > 50) val = 50;
                speed = (dir == 1) ? -val : val;
                atk_fan_pwm_set(speed);
                printf(">> 转速: %d/50\r\n", val);
            }
            else
            {
                printf(">> 未知命令\r\n");
            }

            /* 串口中文回显 */
            printf("===== 状态 =====\r\n");
            if (speed == 0)
            {
                printf("状态: 已停止\r\n");
            }
            else
            {
                printf("方向: %s\r\n", dir == 0 ? "正转" : "反转");
                val = (speed >= 0) ? speed : -speed;
                printf("转速: %d%% (PWM=%d/50)\r\n", val * 100 / 50, val);
            }
            printf("================\r\n\r\n");

            /* OLED 更新 */
            {
                uint8_t idx_speed[] = {CN_ZHUAN, CN_SU};
                uint8_t x, y;

                val = (speed >= 0) ? speed : -speed;
                percent = val * 100 / 50;

                /* 清除可变区域 */
                for (x = 32; x <= 127; x++)
                    for (y = 24; y <= 39; y++)
                        atk_oled_draw_point(x, y, 0);
                for (x = 0; x <= 71; x++)
                    for (y = 48; y <= 63; y++)
                        atk_oled_draw_point(x, y, 0);
                for (x = 72; x <= 127; x++)
                    for (y = 48; y <= 63; y++)
                        atk_oled_draw_point(x, y, 0);

                /* 绘制 */
                atk_oled_show_chinese_str(0, 24, idx_speed, 2);
                atk_oled_show_num(32, 24, percent, 3, 16);
                atk_oled_show_string(56, 24, "%", 16);
                sprintf(buf, "PWM:%d", val);
                atk_oled_show_string(0, 48, buf, 12);

                if (speed == 0)
                {
                    uint8_t idx[] = {CN_TING};
                    atk_oled_show_chinese_str(72, 48, idx, 1);
                }
                else if (dir == 0)
                {
                    uint8_t idx[] = {CN_ZHENG, CN_ZHUAN};
                    atk_oled_show_chinese_str(72, 48, idx, 2);
                }
                else
                {
                    uint8_t idx[] = {CN_FAN, CN_ZHUAN};
                    atk_oled_show_chinese_str(72, 48, idx, 2);
                }

                atk_oled_refresh_gram();
            }

            g_usart_rx_sta = 0;
            memset(g_usart_rx_buf, 0, USART_REC_LEN);
        }

        delay_ms(10);
    }
}
