/**
 ****************************************************************************************************
 * @file        atk_encoder.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-11-01
 * @brief       ATK_EC11编码器模块 驱动代码
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

#include "./BSP/ATK_ENCODER/atk_encoder.h"
#include "./SYSTEM/delay/delay.h"


TIM_HandleTypeDef g_timx_encode_chy_handle;         /* 定时器x句柄 */

/**
 * @brief       编码器定时器初始化
 * @param       arr: 自动重装值
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void atk_encoder_timx_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_Encoder_InitTypeDef timx_encoder_chy_handle;
    
    /******************************** 时钟使能 *****************************************/
    ATK_ENCODER_TIMX_CH1_GPIO_CLK_ENABLE();                                    /* 开启通道1的IO时钟 */
    ATK_ENCODER_TIMX_CH2_GPIO_CLK_ENABLE();                                    /* 开启通道2的IO时钟 */
    ATK_ENCODER_TIMX_CH1_CLK_ENABLE();                                         /* 开启通道1定时器时钟 */
    ATK_ENCODER_TIMX_CH2_CLK_ENABLE();                                         /* 开启通道2定时器时钟 */
    
    /******************************** GPIO配置 *****************************************/
    gpio_init_struct.Pin = ATK_ENCODER_TIMX_CH1_GPIO_PIN;                      /* 通道1的IO口，即编码器的A相 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                                   /* 输入模式 */
    gpio_init_struct.Pull = GPIO_PULLUP;                                       /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                             /* 高速 */
    HAL_GPIO_Init(ATK_ENCODER_TIMX_CH1_GPIO_PORT, &gpio_init_struct);  
    
    gpio_init_struct.Pin = ATK_ENCODER_TIMX_CH2_GPIO_PIN;                      /* 通道2的IO口，即编码器的B相 */
    HAL_GPIO_Init(ATK_ENCODER_TIMX_CH2_GPIO_PORT, &gpio_init_struct);  
    
    /******************************** 定时器基本配置 *****************************************/
    g_timx_encode_chy_handle.Instance = ATK_ENCODER_TIMX;                      /* 定时器3 */
    g_timx_encode_chy_handle.Init.Prescaler = psc;                             /* 定时器分频 */
    g_timx_encode_chy_handle.Init.Period = arr;                                /* 自动重装载值 */
    g_timx_encode_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;            /* 递增计数模式 */
    g_timx_encode_chy_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;      /* 不分频 */
    
    /******************************** 编码器接口配置 *****************************************/
    timx_encoder_chy_handle.EncoderMode = TIM_ENCODERMODE_TI12;                /* TI1,TI2都检测 */
    timx_encoder_chy_handle.IC1Polarity = TIM_ICPOLARITY_RISING;               /* 边沿检测器设置，非反向 */
    timx_encoder_chy_handle.IC1Prescaler = TIM_ICPSC_DIV1;                     /* 不分频 */
    timx_encoder_chy_handle.IC1Filter = 10;                                    /* 滤波器设置 */
    
    timx_encoder_chy_handle.IC2Polarity = TIM_ICPOLARITY_RISING;               /* 边沿检测器设置，非反向 */
    timx_encoder_chy_handle.IC2Prescaler = TIM_ICPSC_DIV1;                     /* 不分频 */
    timx_encoder_chy_handle.IC2Filter = 10;                                    /* 滤波器设置 */
    HAL_TIM_Encoder_Init(&g_timx_encode_chy_handle, &timx_encoder_chy_handle);

    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle, ATK_ENCODER_TIMX_CH1);    /* 开启编码器通道1 */
    HAL_TIM_Encoder_Start(&g_timx_encode_chy_handle, ATK_ENCODER_TIMX_CH2);    /* 开启编码器通道2 */
    
    /******************************** 定时器中断配置 *****************************************/
    HAL_NVIC_SetPriority(ATK_ENCODER_TIMX_INT_IRQn, 2, 0);                     /* 抢占优先级2，响应优先级0 */
    HAL_NVIC_EnableIRQ(ATK_ENCODER_TIMX_INT_IRQn);                             /* 开启定时器3中断 */
    __HAL_TIM_CLEAR_FLAG(&g_timx_encode_chy_handle,TIM_IT_UPDATE);             /* 清除更新中断 */
    __HAL_TIM_ENABLE_IT(&g_timx_encode_chy_handle,TIM_IT_UPDATE);              /* 开启更新中断 */
    
    __HAL_TIM_SET_COUNTER(&g_timx_encode_chy_handle, 0);
}

/**
 * @brief       定时器3中断服务函数
 * @param       无
 * @retval      无
 */
void ATK_ENCODER_TIMX_INT_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&g_timx_encode_chy_handle);
}

volatile int g_timx_encode_count = 0;                               /* 用于记录溢出次数，向下溢出就-1，向上溢出就+1 */

/**
 * @brief       定时器中断回调函数
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&g_timx_encode_chy_handle))   /* 判断CR1的DIR位 */
    {
        g_timx_encode_count--;                                      /* DIR位为1，也就是递减计数 */
    }
    else
    {
        g_timx_encode_count++;                                      /* DIR位为0，也就是递增计数 */
    }
}

/**
 * @brief       编码器计数
 * @param       无
 * @retval      当前总计数值
 */
int32_t atk_encoder_get_count(void)
{
    /* 计算当前总计数值，当前总计数值 = 计数器当前值 + 溢出次数*65536  */
    return ( int32_t )(__HAL_TIM_GET_COUNTER(&g_timx_encode_chy_handle) + g_timx_encode_count * 65536);
}


/**
 * @brief       编码器按键初始化函数
 * @param       无
 * @retval      无
 */
void atk_encoder_key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    ATK_ENCODER_KEY_GPIO_CLK_ENABLE();                              /* 时钟使能 */

    gpio_init_struct.Pin = ATK_ENCODER_KEY_GPIO_PIN;                /* 编码器引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                        /* 输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                            /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                  /* 高速 */
    HAL_GPIO_Init(ATK_ENCODER_KEY_GPIO_PORT, &gpio_init_struct);    /* 编码器引脚模式设置,上拉输入 */
}

/**
 * @brief       编码器按键扫描函数
 * @param       无
 * @retval      0：无按键按下
 *              ENCODER_KEY_PRESSED:按键被按下
 *              ENCODER_KEY_LOOSE:按键被松开
 */
uint8_t atk_encoder_key_scan(void)
{
    static uint8_t key_up = 1;                          /* 按键松开标志 */
    uint8_t key_val = 0;
    
    if ((key_up == 1) && (ATK_ENCODER_KEY == 0))        /* 按键松开标志为1，且有按键按下 */
    {
        key_up = 0;                                     /* 标记按键被按下 */
        delay_ms(10);                                   /* 去抖动 */
        
        if (ATK_ENCODER_KEY == 0)
        {
            key_val = ENCODER_KEY_PRESSED;              /* 返回按键被按下状态 */
        }
    }
    else if ((key_up == 0) && (ATK_ENCODER_KEY == 1))   /* 检测到按键被松开 */
    {
        key_up = 1;                                     /* 标记按键被松开 */
        key_val = ENCODER_KEY_LOOSE;                    /* 返回按键被松开状态 */
    }
    
    return key_val;                                     /* 返回键值 */
}

