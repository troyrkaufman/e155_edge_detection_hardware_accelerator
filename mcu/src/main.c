/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include <stdio.h>
#include "main.h"

#define ADDR 0x30 // write ADDRess to camera module
#define LENGTH 13
//#define BUFFER_SIZE_R 1280
//#define BUFFER_SIZE_T 640

char reg[LENGTH] =  {0x12, 0xda, 0xc2, 0x17, 0x18, 0x32, 0x19, 0x1a, 0x03, 0xc0, 0xc1, 0x11, 0x40};
char data[LENGTH] = {0x80, 0x10, 0x01, 0x11, 0x75, 0x36, 0x01, 0x97, 0x0a, 0x00, 0x10, 0x01, 0xd0};

/*
uint8_t rxbuffer1[BUFFER_SIZE_R];
uint8_t rxbuffer2[BUFFER_SIZE_R];

uint8_t txbuffer1[BUFFER_SIZE_T];
uint8_t txbuffer2[BUFFER_SIZE_T];
*/

int main(void){

    // Buffers for PLL initialization
    configureFlash();
    configureClock();

    // Enable the GPIO ports
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOBEN, 1);
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOCEN, 1);

    ////////////////////////////////
    // I2C configuration
    ////////////////////////////////
    
    init_I2C();

    for (volatile int i = 0; i < LENGTH; i++){
      write_I2C(ADDR, reg[i], data[i]);
    }

    ///////////////////////////////
    // SPI Reception
    ///////////////////////////////

    ////////////////////////////////
    // DMA configuration
    ////////////////////////////////

    init(DMA1_Channel1_BASE, SPI1, true);
    init(DMA2_Channel2_BASE, SPI3, false);

    ////////////////////////////
    // SPI Transmission
    ////////////////////////////
    

    while(1);

}