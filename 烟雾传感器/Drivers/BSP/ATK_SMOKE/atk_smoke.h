/**
 ****************************************************************************************************
 * @file        atk_smoke.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       烟雾传感器模块 驱动代码
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

#ifndef __ATK_SMOKE_H
#define __ATK_SMOKE_H

#include "./SYSTEM/sys/sys.h"


#define RL      10       /* 根据烟雾传感器模块原理图可知：RL = 10k */ 
#define R0      50       /* MQ2在洁净空气中的阻值，不同传感器个体都会存在差异，需预热后测量，这里取平均值得到 */
#define VC      5.0f     /* MQ2供电电压，接5V */
#define VREF    3.3f     /* STM32的ADC的参考电压 */
#define A       45.852   /* y = ax^b 的 a */
#define B       -1.649   /* y = ax^b 的 b */


/**************************************************************************************************/
/* 引脚 定义 */

#define ATK_SMOKE_DO_GPIO_PORT            GPIOA
#define ATK_SMOKE_DO_GPIO_PIN             GPIO_PIN_3
#define ATK_SMOKE_DO_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)

/**************************************************************************************************/
/* 读DO引脚 宏定义 */

#define ATK_SMOKE_DO    HAL_GPIO_ReadPin(ATK_SMOKE_DO_GPIO_PORT, ATK_SMOKE_DO_GPIO_PIN)

/**************************************************************************************************/

void atk_smoke_init(void);
uint32_t atk_smoke_adc_converted_value(void);
float atk_smoke_get_ppm(void);

#endif





