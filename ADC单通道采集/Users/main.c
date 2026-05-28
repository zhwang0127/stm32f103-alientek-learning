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


ADC_HandleTypeDef adc_init_handle;
ADC_ChannelConfTypeDef adc_ch_config;

void adc_init(void)
{
    RCC_PeriphCLKInitTypeDef adc_clk_config;
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();
    
    adc_clk_config.PeriphClockSelection=RCC_PERIPHCLK_ADC;
    adc_clk_config.AdcClockSelection=RCC_ADCPCLK2_DIV6;

    HAL_RCCEx_PeriphCLKConfig(&adc_clk_config);
    
    GPIO_InitTypeDef gpio_init_struct;
;                                 

    gpio_init_struct.Pin = GPIO_PIN_3;                   
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;            
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);     
    
    adc_init_handle.Instance=ADC1;
    adc_init_handle.Init.DataAlign=ADC_DATAALIGN_RIGHT;
    adc_init_handle.Init.ScanConvMode=ADC_SCAN_DISABLE;
    adc_init_handle.Init.ContinuousConvMode=DISABLE;
    adc_init_handle.Init.NbrOfDiscConversion=1;
    adc_init_handle.Init.ExternalTrigConv=ADC_SOFTWARE_START;
    HAL_ADC_Init(&adc_init_handle);
    
    HAL_ADCEx_Calibration_Start(&adc_init_handle);
    
    adc_ch_config.Channel=ADC_CHANNEL_3;
    adc_ch_config.Rank=1;
    adc_ch_config.SamplingTime=ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&adc_init_handle,&adc_ch_config);
}
uint16_t adc_get_data(void)
{
    HAL_ADC_Start(&adc_init_handle);
    HAL_ADC_PollForConversion(&adc_init_handle,10);
    return (uint16_t)HAL_ADC_GetValue(&adc_init_handle);
    
}
    
    
int main(void)
{
    uint16_t adc_data;
    float adc_vol;
    HAL_Init();                                             /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9);                     /* 设置时钟, 72Mhz */
    delay_init(72);                                         /* 延时初始化 */
    usart_init(115200);                                     /* 初始化串口 */
    led_init();                                             /* 初始化LED */
    adc_init();
    
    while (1)
    {
        adc_data=adc_get_data();
        printf("ADC原始数据：%d\r\n",adc_data);
        
        adc_vol=(adc_data*3.3)/4095;
        printf("ADC电压值：%.3f\r\n",adc_vol);
        delay_ms(100);
    }
}
