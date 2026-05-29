#ifndef _ATK_AHT20_H
#define _ATK_AHT20_H
#include "./SYSTEM/sys/sys.h"

void atk_aht20_init(void);
void atk_aht20_read_data(float * temp,float * humi);
                         
#endif
