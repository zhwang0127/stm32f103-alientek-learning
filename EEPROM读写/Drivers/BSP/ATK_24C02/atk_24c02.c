#include "./BSP/ATK_24C02/atk_24c02.h"
#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"

void atk_24c02_init(void)
{
    iic_init();
}

void atk_24cxx_write_one_byte(uint16_t addr,uint8_t data)
{
    iic_start();
    iic_send_byte(0XA0);
    iic_wait_ack();
    iic_send_byte(addr);
    iic_wait_ack();
    iic_send_byte(data);
    iic_wait_ack();
    iic_stop();
    delay_ms(10);

}

uint8_t atk_24cxx_read_one_byte(uint16_t addr)
{
    uint8_t temp=0;
    iic_start();
    iic_send_byte(0XA0);
    iic_wait_ack();
    iic_send_byte(addr);
    iic_wait_ack();
    iic_start();
    iic_send_byte(0XA1);
    iic_wait_ack();
    temp = iic_read_byte(0);
    iic_stop();
    return temp;

}

void atk_24cxx_write(uint16_t addr,uint8_t *pbuf, uint16_t datalen)
{
    while(datalen--)
    {
    atk_24cxx_write_one_byte(addr++,*pbuf++);
        
    }
}

void atk_24cxx_read(uint16_t addr,uint8_t *pbuf, uint16_t datalen)
{
    while(datalen--)
    {
    *pbuf++ =atk_24cxx_read_one_byte(addr++);
        
    }
}