/**
 ****************************************************************************************************
 * @file        atk_soil_humidity.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       土壤湿度传感器 驱动代码
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


#ifndef __ATK_SOIL_HUMIDITY_H
#define __ATK_SOIL_HUMIDITY_H
#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* 外部接口函数*/
void atk_soil_humidity_init(void);                   /* 初始化 */
uint8_t atk_soil_humidity_get_val(void);             /* 读取湿度百分比 */

#endif
