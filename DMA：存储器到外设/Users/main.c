/**
 ******************************************************************************
 * @file     main.c
 * @author   正点原子团队(ALIENTEK)
 * @version  V1.0
 * @date     2023-08-01
 * @brief    按键输入实验
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

uint8_t buf_a[5]={'S','T','M','3','2'};


DMA_HandleTypeDef g_dma_handle;
extern UART_HandleTypeDef g_uart1_handle;  /* UART句柄 */


void dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    __HAL_LINKDMA(&g_uart1_handle,hdmatx,g_dma_handle);
    
    g_dma_handle.Instance=DMA1_Channel4;
    g_dma_handle.Init.Priority=DMA_PRIORITY_MEDIUM;
    g_dma_handle.Init.Direction=DMA_MEMORY_TO_PERIPH;
    g_dma_handle.Init.PeriphInc=DMA_PINC_DISABLE;
    g_dma_handle.Init.MemInc=DMA_MINC_ENABLE;
    g_dma_handle.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;
    g_dma_handle.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;
    g_dma_handle.Init.Mode=DMA_NORMAL;
    HAL_DMA_Init(&g_dma_handle);
    
//    HAL_DMA_Start(&g_dma_handle,(uint32_t)buf_a,(uint32_t)&USART1->DR,5);
//    USART1->CR3|=1<<7;

}

//void dma_t_enable(void)
//{
//    __HAL_DMA_DISABLE(&g_dma_handle);
//    g_dma_handle.Instance->CNDTR = 5;
//    __HAL_DMA_ENABLE(&g_dma_handle);
//}

int main(void)
{    
    HAL_Init();                                                 /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                         /* 设置时钟, 72Mhz */
    delay_init(72);                                             /* 延时初始化 */
    usart_init(115200);                                         /* 初始化串口 */
    led_init();                                                 /* 初始化LED */
    key_init();                                                 /* 初始化按键 */

    dma_init();
    
    while (1)
    {
        if(key_scan(0)==KEY0_PRES)
        {
            HAL_UART_Transmit_DMA(&g_uart1_handle,buf_a,5);
//            dma_t_enable();
        }
        if(__HAL_DMA_GET_FLAG(&g_dma_handle,DMA_FLAG_TC4))
        {
            HAL_UART_DMAStop(&g_uart1_handle);
            __HAL_DMA_CLEAR_FLAG(&g_dma_handle,DMA_FLAG_TC4);
            
        }
        delay_ms(10);
    }
}
