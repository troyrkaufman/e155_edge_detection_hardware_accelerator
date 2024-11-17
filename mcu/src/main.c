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
#define BUFFER_SIZE_R 1280
#define BUFFER_SIZE_T 640
#define PIXEL_BYTES 2

char reg[LENGTH] =  {0x12, 0xda, 0xc2, 0x17, 0x18, 0x32, 0x19, 0x1a, 0x03, 0xc0, 0xc1, 0x11, 0x40};
char data[LENGTH] = {0x80, 0x10, 0x01, 0x11, 0x75, 0x36, 0x01, 0x97, 0x0a, 0x00, 0x10, 0x01, 0xd0};


uint8_t rxBuffer1[BUFFER_SIZE_R];
uint8_t rxBuffer2[BUFFER_SIZE_R];

uint8_t txBuffer1[BUFFER_SIZE_T];
uint8_t txBuffer2[BUFFER_SIZE_T];

volatile uint8_t * currentBufferR = rxBuffer1; // Active DMA buffer 
volatile uint8_t * processBufferR = NULL;      // Buffer whose data is ready to be processed

volatile uint8_t * currentBufferT = txBuffer1; // ACtive DMA buffer receiving processed data
volatile uint8_t * processBufferT = NULL;      // Buffer whose data is read to be sent over SPI

volatile uint8_t bufferFullR = 0;              // Flag to indicate that buffer is ready

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
    // SPI Configuration
    ///////////////////////////////

    // Designate the SPI pins for Camera Module and MCU communication
    pinMode(PA5, GPIO_ALT);    // SCK
    pinMode(PA7, GPIO_ALT);    // MOSI
    pinMode(PA6, GPIO_ALT);    // MISO
    pinMode(PA8, GPIO_OUTPUT); // CE

    // Designate the SPI pins for MCU and FPGA communication
    pinMode(PB3, GPIO_ALT);    // SCK
    pinMode(PB5, GPIO_ALT);    // MOSI
    pinMode(PB4, GPIO_ALT);    // MISO
    pinMode(PC15, GPIO_OUTPUT); // CE

    // Give SPI pins proper ALT functinos
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SCK PA5
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL7, 5); // SDI PA7
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5); // SDO PA6

    initSPI(SPI1, 7, 0, 0);

    ////////////////////////////////
    // DMA configuration
    ////////////////////////////////

    // Turn on DMA controller in RCC
    RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN);
    RCC->AHB1ENR |= (RCC_AHB1ENR_DMA2EN);

    init(DMA1_Channel1_BASE, SPI1, true, rxBuffer1);
    init(DMA2_Channel2_BASE, SPI3, false, txBuffer1);

    ////////////////////////////
    // SPI Transmission
    ////////////////////////////
    

    while(1){
        
        digitalWrite(PA8, 1);
        spiSendReceive(SPI1, 0x61);
        spiSendReceive(SPI1, 0x00);
        digitalWrite(PA8, 0);
    }

}

/*
// Interrupt handler for DMA1Channel2 for RECEPTION
void DMA1_Channel2_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF2) {   // Transfer Complete Interrupt
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Clear the interrupt flag

        // Update buffers
        if (currentBufferR == rxBuffer1) {
            processBufferR = rxBuffer1;      // Assign the filled buffer for processing
            currentBufferR = rxBuffer2;      // Switch DMA to the second buffer
            DMA1_Channel2->CMAR = (uint32_t)rxBuffer2;
        } else {
            processBufferR = rxBuffer2;
            currentBufferR = rxBuffer1;
            DMA1_Channel2->CMAR = (uint32_t)rxBuffer1;
        }

        bufferFullR = 1; // Signal processor to process the buffer
    }
}
*/
/*

uint8_t grayscaleConversion(uint16_t pixel) {
    // Extract RGB components from the 16-bit pixel (RGB565 format)
    uint8_t red = (pixel & 0xF800) >> 11;      // Top 5 bits
    uint8_t green = (pixel & 0x07E0) >> 5;     // Middle 6 bits
    uint8_t blue = pixel & 0x001F;             // Bottom 5 bits

    // Scale the values to 8-bit range
    red = (red * 255) / 31;
    green = (green * 255) / 63;
    blue = (blue * 255) / 31;

    // Compute the grayscale value using standard formula
    return (red * 30 + green * 59 + blue * 11) / 100;
}

void processData(void){
    if (bufferFullR && processBufferR){
        for (int i = 0; i < BUFFER_SIZE_R; i += PIXEL_BYTES){
            uint16_t pixel = (processBufferR[i] << 8) | processBufferR[i+1];

        }
    }
}


*/