/**
 ****************************************************************************************************
 * @file        key.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       按键输入 驱动代码
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

#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 引脚 定义 */

#define KEY0                 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_7)
#define KEY1                 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6)
#define KEY2                 HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)
#define KEY3                 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2)


#define KEY0_PRES    1              /* KEY0按下 */
#define KEY1_PRES    2              /* KEY1按下 */
#define KEY2_PRES    3              /* KEY2按下 */
#define KEY3_PRES    4              /* KEY3按下 */

void key_init(void);                /* 按键初始化函数 */
uint8_t key_scan(uint8_t mode);     /* 按键扫描函数 */

#endif


















