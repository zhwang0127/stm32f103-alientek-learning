
#include "./BSP/IIC/myiic.h"
#include "./SYSTEM/delay/delay.h"


void iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    IIC_SDA_GPIO_CLK_ENABLE();                                
    IIC_SCL_GPIO_CLK_ENABLE(); 
    
    gpio_init_struct.Pin = IIC_SDA_GPIO_PIN;                   
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;            
    gpio_init_struct.Pull = GPIO_PULLUP;                    
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);       

    gpio_init_struct.Pin = IIC_SCL_GPIO_PIN;                   
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            
    HAL_GPIO_Init(IIC_SDA_GPIO_PORT, &gpio_init_struct);       
      
    IIC_SCL(1);
    IIC_SDA(1);    
}

void iic_delay(void)
{
    delay_us(2);
}

void iic_start(void)
{
    IIC_SDA(1);
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(0);
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

void iic_stop(void)
{
    IIC_SDA(0);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SDA(1);
    iic_delay();
}

void iic_ack(void)
{
    IIC_SDA(0);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SCL(0);
    iic_delay();
    IIC_SDA(1);
    iic_delay();
}

void iic_nack(void)
{
    IIC_SDA(1);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    IIC_SCL(0);
    iic_delay();
}

uint8_t iic_wait_ack(void)
{
    uint8_t rack = 0;
    uint8_t waittime = 0;
    IIC_SDA(1);
    iic_delay();
    IIC_SCL(1);
    iic_delay();
    while(IIC_READ_SDA)
    {
        waittime++;
      if(waittime>250)  
      {
        iic_stop();
        rack = 1;
        break;
      }
    }
    IIC_SCL(0);
    iic_delay();    
    return rack;
}

void iic_send_byte(uint8_t data)
{
    for(uint8_t i=0;i<8;i++)
    {
    IIC_SDA((data & 0x80) >> 7);
    iic_delay();    
    IIC_SCL(1);
    iic_delay();
    IIC_SCL(0);
    iic_delay();

    data <<= 1;
    }
    IIC_SDA(1);
    iic_delay();    
}

uint8_t iic_read_byte(uint8_t ack)
{
    uint8_t receive=0;
    for(uint8_t i=0;i<8;i++)    
    {
        receive <<= 1;
        IIC_SDA(1);
        iic_delay();    
        IIC_SCL(1);
        iic_delay();
        if(IIC_READ_SDA) receive |= 1;
        IIC_SCL(0);
        iic_delay();  
    }
    if(!ack)
    {
        iic_nack();
    }
    else
    {
        iic_ack();
    }
    return receive;
}














