#ifndef __DELAY_H
#define __DELAY_H 

//#include "main.h"  
#include <stdint.h>
#include <stdlib.h> 
#include <stm32l432xx.h>
#include "stm32l4xx.h"

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif




























