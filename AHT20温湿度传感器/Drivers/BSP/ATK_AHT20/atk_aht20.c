#include "./BSP/ATK_AHT20/atk_aht20.h"
#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"

void atk_aht20_write_nbyte(uint8_t * data,uint8_t len)
{
    iic_start();
    iic_send_byte(0x70);
    iic_wait_ack();
    while(len--)
    {
        iic_send_byte(*data++);
        iic_wait_ack();
    }
    iic_stop();
    
}

void atk_aht20_read_nbyte(uint8_t * data,uint8_t len)
{
    iic_start();
    iic_send_byte(0x71);
    iic_wait_ack();
    while(len--)
    { 
        *data++=iic_read_byte(len>1?1:0);
    } 
    iic_stop();
}

void atk_aht20_init(void)
{
    uint8_t commend[3]={0xBE,0x08,0x00};
    delay_ms(40);
    uint8_t sta=0;
    
    atk_aht20_read_nbyte(&sta,1);
    if((sta & 0x08) != 0x08)
    {
        atk_aht20_write_nbyte(commend,sizeof(commend));
    }
    delay_ms(40);
}

void atk_aht20_read_data(float * temp,float * humi)
{
    uint32_t temp_data=0,humi_data=0;
    uint8_t commend[3]={0xAC,0x33,0x00};
    uint8_t data[7]={0};

    atk_aht20_write_nbyte(commend,sizeof(commend));
    delay_ms(80);
    atk_aht20_read_nbyte(data,7);
    
    if((data[0] & 0x80) ==0)
    {
        
        humi_data = data[1];
        humi_data<<=8;
        humi_data |= data[2];
        humi_data<<=8;
        humi_data |= data[3];
        humi_data>>=4;

        *humi = (float)humi_data / 1048576*100;
        
        humi_data |= data[3];
        humi_data<<=8;
        humi_data |= data[4];
        humi_data<<=8;
        humi_data |= data[5];
        humi_data &= 0x0fffff;
        
        *temp = (float)temp_data / 1048576 *200 -50;   
    }
}

