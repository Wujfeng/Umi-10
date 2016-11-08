/****************************************************************
 * Copyright (C) 2016, Yagro, all right reserved.
 * File name:    Delay.h
 * Date:         2016.03.22
 * Description:  Delay Driver
*****************************************************************/

#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"


void Delay_Init(void);
void Delay_us(uint32_t nus);
void Delay_ms(uint16_t nms);
void os_time_init(void);
float os_time(void);

#endif	/* __DELAY_H */
