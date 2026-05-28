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
    KEY0_GPIO_CLK_ENABLE();                                     /* KEY0ʱ��ʹ�� */
    WKUP_GPIO_CLK_ENABLE();                                     /* WKUPʱ��ʹ�� */

    gpio_init_struct.Pin = KEY0_GPIO_PIN;                       /* KEY0���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(KEY0_GPIO_PORT, &gpio_init_struct);           /* KEY0����ģʽ����,�������� */

    gpio_init_struct.Pin = WKUP_GPIO_PIN;                       /* WKUP���� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                    /* ���� */
    gpio_init_struct.Pull = GPIO_PULLDOWN;                      /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;              /* ���� */
    HAL_GPIO_Init(WKUP_GPIO_PORT, &gpio_init_struct);           /* WKUP����ģʽ����,�������� */

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
    
    if ((key_up == 1) && ((KEY0 == 0) || (WK_UP == 1))) /* �����ɿ���־Ϊ1�����а������� */
    {
        key_up = 0;
        delay_ms(10);                                   /* ȥ���� */
        if (KEY0 == 0)
        {
            key_val = KEY0_PRES;
        }
        if (WK_UP == 1)
        {
            key_val = WKUP_PRES;
        }
    }
    else if ((KEY0 == 1) && (WK_UP == 0))               /* û�а������£���ǰ����ɿ� */
    {
        key_up = 1;
    }
    
    return key_val;                                     /* ���ؼ�ֵ */
}
