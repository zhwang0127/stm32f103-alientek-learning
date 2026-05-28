/**
 ****************************************************************************************************
 * @file        key.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-08-01
 * @brief       �������� ��������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� M48Z-M3��Сϵͳ��STM32F103��
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ������ʼ������
 * @param       ��
 * @retval      ��
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_init_struct.Pin = GPIO_PIN_6 | GPIO_PIN_7;             /* KEY0(PA7), KEY1(PA6) */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* 低电平空闲，按下为高 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    gpio_init_struct.Pin = GPIO_PIN_7;                          /* KEY2(PB7) */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* 低电平空闲，按下为高 */
    HAL_GPIO_Init(GPIOB, &gpio_init_struct);

    gpio_init_struct.Pin = GPIO_PIN_2;                          /* KEY3(PA2) */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* 高电平空闲，按下为低 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);
}

/**
 * @brief       ����ɨ�躯��
 * @note        �ú�������Ӧ���ȼ�(ͬʱ���¶������): WK_UP > KEY0
 * @param       mode:0 / 1,���庬������:
 *   @arg       0,  ��֧��������(���������²���ʱ, ֻ�е�һ�ε��û᷵�ؼ�ֵ,
 *                  �����ɿ��Ժ�, �ٴΰ��²Ż᷵��������ֵ)
 *   @arg       1,  ֧��������(���������²���ʱ, ÿ�ε��øú������᷵�ؼ�ֵ)
 * @retval      ��ֵ,��������:
 *              KEY0_PRES, 1, KEY0����
 *              WKUP_PRES, 2, WK_UP����
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;                          /* �����ɿ���־ */
    uint8_t key_val = 0;
    
    if (mode == 1)                                      /* ֧������ */
    {
        key_up = 1;
    }
    
    if (key_up && (KEY0 == 1 || KEY1 == 1 || KEY2 == 1 || KEY3 == 0)) /* �����ɿ���־Ϊ1�����а������� */
    {
        key_up = 0;
        delay_ms(10);                                   /* ȥ���� */
        if (KEY0 == 1)
        {
            key_val = KEY0_PRES;
        }
        if (KEY1 == 1)
        {
            key_val = KEY1_PRES;
        }
        if (KEY2 == 1)
        {
            key_val = KEY2_PRES;
        }
        if (KEY3 == 0)
        {
            key_val = KEY3_PRES;
        }
    }
    else if (KEY0 == 0 && KEY1 == 0 && KEY2 == 0 && KEY3 == 1)               /* û�а������£���ǰ����ɿ� */
    {
        key_up = 1;
    }
    
    return key_val;                                     /* ���ؼ�ֵ */
}
