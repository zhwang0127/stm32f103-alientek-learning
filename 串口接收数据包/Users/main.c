/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    跑马灯实验
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

UART_HandleTypeDef g_uart_init_handle;
uint8_t rx_data;
 
void usart_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct = {0};

    gpio_init_struct.Pin = GPIO_PIN_9;                   /* USART1_TX */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;             /* 推挽复用输出 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;       /* 高速 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.Pin = GPIO_PIN_10;                  /* USART1_RX */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;              /* 浮空输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                  /* 上拉，空闲保持高电平 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
    
    g_uart_init_handle.Instance=USART1;
    g_uart_init_handle.Init.BaudRate=115200;
    g_uart_init_handle.Init.WordLength=UART_WORDLENGTH_8B;
    g_uart_init_handle.Init.StopBits=UART_STOPBITS_1;
    g_uart_init_handle.Init.Parity=UART_PARITY_NONE;
    g_uart_init_handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;
    g_uart_init_handle.Init.Mode=UART_MODE_TX_RX;
    
    HAL_UART_Init(&g_uart_init_handle);
    
    HAL_NVIC_EnableIRQ(USART1_IRQn);
    HAL_NVIC_SetPriority(USART1_IRQn,3,3);
    __HAL_UART_ENABLE_IT(&g_uart_init_handle,UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&g_uart_init_handle,UART_IT_IDLE);

}

uint8_t buf[200];
uint8_t num=0;
void USART1_IRQHandler(void)
{
    if(__HAL_UART_GET_FLAG(&g_uart_init_handle,UART_FLAG_RXNE)!=0)
    {
        rx_data = (uint8_t)(USART1->DR);
        buf[num] = rx_data;
        num++;
    }
    
    if(__HAL_UART_GET_FLAG(&g_uart_init_handle,UART_FLAG_IDLE)!=0)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&g_uart_init_handle);
        LED0_TOGGLE();
        HAL_UART_Transmit(&g_uart_init_handle,buf,num,100);
        num=0;
    }

}

#include <stdio.h>
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0);     /* 等待上一个字符发送完成 */

    USART1->DR = (uint8_t)ch;             /* 将要发送的字符 ch 写入到DR寄存器 */
    return ch;
}

int main(void)
{
   
    uint8_t buf_a[5]={'H','E','L','L','O'};
    
    HAL_Init();                                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* 设置时钟, 72Mhz */
    delay_init(72);                                         /* 延时初始化 */
    usart_init();                                     /* 初始化串口 */
    led_init();                                             /* 初始化LED */
    
   
    while (1)
    {
        printf("num:%d",50);
        delay_ms(1000);
    }
}
