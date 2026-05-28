/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2024-11-01
 * @brief    EEPROM 实验
 * @license  Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
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
#include "./BSP/ATK_24C02/atk_24c02.h"

/* 中英文显示字符数组 */
uint8_t *const language_str[2][3] =
{
    {
        (uint8_t *)"语言选择: ",
        (uint8_t *)"按键0    - 中文 ",
        (uint8_t *)"按键WKUP - 英文 ",
    },
    {
        (uint8_t *)"Language: ",
        (uint8_t *)"KEY0 - Chinese ",
        (uint8_t *)"WKUP - English ",
    },
};

/* 定义语言枚举 */ 
typedef enum 
{
    CHINESE = 0,    /* 中文 */
    ENGLISH = 1,    /* 英文 */
} Language;

Language current_language;                  /* 记录当前语言变量 */
#define EEPROM_LANGUAGE_ADDR    0x00        /* EEPROM中存储语言设置的地址 */

int main(void)
{
    uint8_t key;
    uint8_t stored_language;
    uint16_t t = 0;  
    
    HAL_Init();                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);     /* 设置时钟, 72Mhz */
    delay_init(72);                         /* 延时初始化 */
    usart_init(115200);                     /* 串口初始化为115200 */
    led_init();                             /* 初始化LED */
    key_init();                             /* 初始化按键 */
    atk_24c02_init();                       /* 初始化24C02 */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)               /* KEY0按下，设置语言为中文 */
        {
            current_language = CHINESE;
            atk_24c02_write_one_byte(EEPROM_LANGUAGE_ADDR, current_language);
            printf("KEY0\r\n");
        }
        else if(key == WKUP_PRES)           /* WKUP按下，设置语言为英文 */
        {
            current_language = ENGLISH;
            atk_24c02_write_one_byte(EEPROM_LANGUAGE_ADDR, current_language);
            printf("WKUP\r\n");
        }
        
        /* 读取EEPROM中存储语言设置的参数 */
        stored_language = atk_24c02_read_one_byte(EEPROM_LANGUAGE_ADDR);
        
        /* 判断读取的值是否合法 */
        if (stored_language == CHINESE || stored_language == ENGLISH) 
        {
            current_language = (Language)stored_language;
        } 
        else 
        {
            /* 数值不合法，则默认设置为中文 */
            current_language = CHINESE;
            atk_24c02_write_one_byte(EEPROM_LANGUAGE_ADDR, current_language);
        }
        
        t++;
        if (t == 50)
        {
            /* 打印当前语言 */
            for(uint8_t i = 0; i < 3; i++)
            {
                printf("%s \r\n", language_str[current_language][i]);
            }
            printf("\r\n");
            t = 0;
        }

        delay_ms(10);
    }
}

