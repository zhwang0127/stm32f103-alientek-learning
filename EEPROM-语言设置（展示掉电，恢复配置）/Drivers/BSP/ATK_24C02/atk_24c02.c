/**
 ****************************************************************************************************
 * @file        atk_24c02.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       ATK_24C02 驱动代码
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
 * 修改说明
 * V1.0 20241101
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/IIC/myiic.h"
#include "./BSP/ATK_24C02/atk_24c02.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void atk_24c02_init(void)
{
    iic_init();
}

/**
 * @brief       在ATK_24C02指定地址读出一个数据
 * @param       readaddr: 开始读数的地址
 * @retval      读到的数据
 */
uint8_t atk_24c02_read_one_byte(uint16_t addr)
{
    uint8_t temp = 0;
    iic_start();                /* 发送起始信号 */
    
    iic_send_byte(0XA0);        /* 发送器件 0XA0 + 高位a8/a9/a10地址,写数据 */
    iic_wait_ack();             /* 每次发送完一个字节,都要等待ACK */
    iic_send_byte(addr);        /* 发送低位地址 */
    iic_wait_ack();             /* 等待ACK, 此时地址发送完成了 */
    
    iic_start();                /* 重新发送起始信号 */ 
    iic_send_byte(0XA1);        /* 进入接收模式, IIC规定最低位是0, 表示读取 */
    iic_wait_ack();             /* 每次发送完一个字节,都要等待ACK */
    temp = iic_read_byte(0);    /* 接收一个字节数据 */
    
    iic_stop();                 /* 产生一个停止条件 */
    return temp;
}

/**
 * @brief       在ATK_24C02指定地址写入一个数据
 * @param       addr: 写入数据的目的地址
 * @param       data: 要写入的数据
 * @retval      无
 */
void atk_24c02_write_one_byte(uint16_t addr, uint8_t data)
{
    /* 原理说明见:at24c02_read_one_byte函数, 本函数完全类似 */
    iic_start();                /* 发送起始信号 */
    iic_send_byte(0XA0);        /* 发送器件 0XA0 + 高位a8/a9/a10地址,写数据 */
    iic_wait_ack();             /* 每次发送完一个字节,都要等待ACK */
    iic_send_byte(addr);        /* 发送低位地址 */
    iic_wait_ack();             /* 等待ACK, 此时地址发送完成了 */
    
    /* 因为写数据的时候,不需要进入接收模式了,所以这里不用重新发送起始信号了 */
    iic_send_byte(data);        /* 发送1字节 */
    iic_wait_ack();             /* 等待ACK */
    iic_stop();                 /* 产生一个停止条件 */
    delay_ms(10);               /* 注意: EEPROM 写入比较慢,必须等到10ms后再写下一个字节 */
}
 
/**
 * @brief       检查ATK_24C02是否正常
 *   @note      检测原理: 在器件的末地址写如0X55, 然后再读取, 如果读取值为0X55
 *              则表示检测正常. 否则,则表示检测失败.
 *
 * @param       无
 * @retval      检测结果
 *              0: 检测成功
 *              1: 检测失败
 */
uint8_t atk_24c02_check(void)
{
    uint8_t temp;
    uint16_t addr = EE_TYPE;
    temp = atk_24c02_read_one_byte(addr); /* 避免每次开机都写AT24C02 */

    if (temp == 0X55)   /* 读取数据正常 */
    {
        return 0;
    }
    else    /* 排除第一次初始化的情况 */
    {
        atk_24c02_write_one_byte(addr, 0X55); /* 先写入数据 */
        temp = atk_24c02_read_one_byte(255);  /* 再读取数据 */

        if (temp == 0X55)return 0;
    }

    return 1;
}

/**
 * @brief       在ATK_24C02里面的指定地址开始读出指定个数的数据
 * @param       addr    : 开始读出的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要读出数据的个数
 * @retval      无
 */
void atk_24c02_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while (datalen--)
    {
        *pbuf++ = atk_24c02_read_one_byte(addr++);
    }
}

/**
 * @brief       在ATK_24C02里面的指定地址开始写入指定个数的数据
 * @param       addr    : 开始写入的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要写入数据的个数
 * @retval      无
 */
void atk_24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    while(datalen--)
    {
        atk_24c02_write_one_byte(addr++, *pbuf++);
    }
}






