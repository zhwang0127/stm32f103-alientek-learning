#ifndef _ATK_24C02_H
#define _ATK_24C02_H
#include "./SYSTEM/sys/sys.h"
void atk_24cxx_write_one_byte(uint16_t addr,uint8_t data);
uint8_t atk_24cxx_read_one_byte(uint16_t addr);
void atk_24c02_init(void);

                          
#endif