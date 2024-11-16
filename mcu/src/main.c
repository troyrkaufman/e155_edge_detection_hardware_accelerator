/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include <stdio.h>
#include "main.h"

#define addr 0x30 // write address to camera module
#define length 13

char reg[length] =  {0x12, 0xda, 0xc2, 0x17, 0x18, 0x32, 0x19, 0x1a, 0x03, 0xc0, 0xc1, 0x11, 0x40};
char data[length] = {0x80, 0x10, 0x01, 0x11, 0x75, 0x36, 0x01, 0x97, 0x0a, 0x00, 0x10, 0x01, 0xd0};

  
int main(void){

    // Buffers for PLL initialization
    configureFlash();
    configureClock();

    // Enable the GPIOA port
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);

    ////////////////////////////////
    // I2C configuration
    ////////////////////////////////
    
    init_I2C();

    //write_I2C(addr, reg[0], data[0]);

    for (volatile int i = 0; i < length; i++){
      write_I2C(addr, reg[i], data[i]);
    }
/*
    ////////////////////////////////
    // DMA configuration
    ////////////////////////////////

    // TODO: Reset DMA channel configuration
    DMA1_Channel2->CCR  &= ~(0xFFFFFFFF);
    DMA1_Channel2->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b10) |
                            _VAL2FLD(DMA_CCR_MINC, 0b1) |
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) |
                            _VAL2FLD(DMA_CCR_DIR, 0b1)
                            );
    // TODO: Set DMA Channel configuration
    
    // Set DMA source and destination addresses.
    // TODO: Source: Address of the character array buffer in memory.
    

    // TODO: Dest.: USART data register
    

    // TODO: Set DMA data transfer length (# of samples).
    
    
    // TODO: Select correct selection for DMA channel
    

    // TODO: Enable DMA channel.
    
*/

    while(1);

}