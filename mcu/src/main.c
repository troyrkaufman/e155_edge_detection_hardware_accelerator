/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include <stdio.h>
#include "main.h"

#define addr 0x30 // write address to camera module
#define length 6

//const uint8_t index[length] = {0xff, 0x34, 0x02, 0x44, 0x56, 0x41};
const uint8_t cfgr[length] = {0x301, 0x44, 0x65, 0x43, 0x21, 0x40};


int main(void){

    // Buffers for PLL initialization
    configureFlash();
    configureClock();

    // Enable the GPIOA port
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
    
    init_I2C();

    write_I2C(addr, cfgr, length);

    while(1);

}