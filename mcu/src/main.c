/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include <stdio.h>
#include "main.h"

uint8_t rxBuffer1[BUFFER_SIZE_R];
uint8_t rxBuffer2[BUFFER_SIZE_R];

uint8_t txBuffer1[BUFFER_SIZE_T];
uint8_t txBuffer2[BUFFER_SIZE_T];

volatile uint8_t * currentBufferR = rxBuffer1; // Active DMA buffer 
volatile uint8_t * processBufferR = NULL;      // Buffer whose data is ready to be processed

volatile uint8_t * currentBufferT = txBuffer1; // ACtive DMA buffer receiving processed data
volatile uint8_t * processBufferT = NULL;      // Buffer whose data is read to be sent over SPI

volatile uint8_t bufferFullR = 0;              // Flag to indicate that buffer is ready

void initDMA1Ch2(){
// Reset DMA1 Channel 2
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA1EN);
    DMA1_Channel2->CCR  &= ~(0xFFFFFFFF);
    DMA1_Channel2->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b10) |   // Priority is set to medium
                            _VAL2FLD(DMA_CCR_MINC, 0b1) | // memory address updates after every reception
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) | // DMA NBYTE count will update to declared value
                            _VAL2FLD(DMA_CCR_DIR, 0b0) |  // Peripheral to memory transfer
                            _VAL2FLD(DMA_CCR_MSIZE, 0b00) | // Set to byte length
                            _VAL2FLD(DMA_CCR_PSIZE, 0b00)   // Set to byte length
                            );
    
    // Set DMA source and destination addresses.
    // Source: Address of the data from peripheral
    DMA1_Channel2->CPAR = _VAL2FLD(DMA_CPAR_PA, (uint32_t) &(SPI1->DR));

    // DEST.: Address of the current buffer in use in memory
    DMA1_Channel2->CMAR = _VAL2FLD(DMA_CMAR_MA, (uint32_t) &currentBufferR);

    // Set DMA data transfer length (# of samples)
    DMA1_Channel2->CNDTR |= _VAL2FLD(DMA_CNDTR_NDT, 3840); // # pix per row * pixel width * # rows : 640 * 2 * 3
    
    // Select the 1st option for mux to channel 2
    DMA1_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 1);

    // Enable interrupt bit for channel 2
    DMA1_Channel2->CCR |=  _VAL2FLD(DMA_CCR_TCIE, 1);

    // Enable DMA1 channel.
    DMA1_Channel2->CCR  |= DMA_CCR_EN;

    // Enable the interrupt for DMA1 Channel2
    NVIC->ISER[0] |= (1<<DMA1_Channel2_IRQn);
}

void initDMA2Ch2(){
// Reset DMA1 Channel 2
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA2EN);
    DMA1_Channel2->CCR  &= ~(0xFFFFFFFF);
    DMA1_Channel2->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b11) |   // Priority is set to high
                            _VAL2FLD(DMA_CCR_MINC, 0b1) | // memory address updates after every transmission
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) | // DMA NBYTE count will update to declared value
                            _VAL2FLD(DMA_CCR_DIR, 0b1) |  // Peripheral to memory transfer
                            _VAL2FLD(DMA_CCR_MSIZE, 0b00) | // Set to byte length
                            _VAL2FLD(DMA_CCR_PSIZE, 0b00)   // Set to byte length
                            );
    
    // Set DMA source and destination addresses.
    // Source: Address of the data from memory
    DMA1_Channel2->CPAR = _VAL2FLD(DMA_CPAR_PA, (uint32_t) &currentBufferT);

    // DEST.: Address of the SPI3 data register
    DMA1_Channel2->CMAR = _VAL2FLD(DMA_CMAR_MA, (uint32_t) &(SPI3->DR));

    // Set DMA data transfer length (# of samples)
    DMA1_Channel2->CNDTR |= _VAL2FLD(DMA_CNDTR_NDT, 1); // # pix per row * pixel width * # rows : 640 * 2 * 3
    
    // Select the 3rd option for mux to channel 2
    DMA1_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 3);

    // Enable interrupt bit for channel 2
    DMA1_Channel2->CCR |=  _VAL2FLD(DMA_CCR_TCIE, 1);

    // Enable DMA1 channel.
    DMA1_Channel2->CCR  |= DMA_CCR_EN;

    // Enable the interrupt for DMA1 Channel2
    NVIC->ISER[0] |= (1<<DMA1_Channel2_IRQn);
}

// Make a new file for the processing stuff

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
        bufferFullR = 0; // Clear flag after processing
        processBufferR = NULL; // Reset buffer pointer
    }
}

int main(void){

    // Buffers for PLL initialization
    configureFlash();
    configureClock();

    // Enable interrupts globally
    __enable_irq();

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
    //pinMode(PA7, GPIO_ALT);    // MOSI We'll use PA7 in reality but for now we'll use PB
    pinMode(PB5, GPIO_ALT);    // MOSI
    pinMode(PA6, GPIO_ALT);    // MISO
    pinMode(PA8, GPIO_OUTPUT); // CE

    // Designate the SPI pins for MCU and FPGA communication
    //pinMode(PB3, GPIO_ALT);    // SCK
    //pinMode(PB5, GPIO_ALT);    // MOSI
    //pinMode(PB4, GPIO_ALT);    // MISO
    //pinMode(PC15, GPIO_OUTPUT); // CE

    // Give SPI pins proper ALT functinos
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SCK PA5
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // MOSI PA7
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5); // MISO PA6

    initSPI(SPI1, 4, 0, 0, true);

    ////////////////////////////////
    // DMA configuration
    ////////////////////////////////

    //RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA2EN);

    ////////////////////////////
    // SPI Transmission
    ////////////////////////////

    digitalWrite(PA8, 1);
    volatile uint16_t rgb565;
    
    // temporary spi communication in core for rn 
    while(1){  
        digitalWrite(PA8, 0);
        spiSendReceive(SPI1, 0x3d);
        rgb565 = spiSendReceive(SPI1, 0x00);
        printf("RGB565 current value is: %d\n", rgb565);
        digitalWrite(PA8, 1);
    }

}

// Interrupt handler for DMA1Channel2 for RECEPTION
void DMA1_Channel2_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF2) {   // Transfer Complete Interrupt
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Clear the interrupt flag

        // Update buffers
        if (currentBufferR == rxBuffer1) {
            processBufferR = rxBuffer1;      // Assign the filled buffer for processing
            currentBufferR = rxBuffer2;      // Switch DMA to the second buffer
            DMA1_Channel2->CMAR = (uint32_t)&rxBuffer2;
        } else {
            processBufferR = rxBuffer2;
            currentBufferR = rxBuffer1;
            DMA1_Channel2->CMAR = (uint32_t)&rxBuffer1;
        }

        bufferFullR = 1; // Signal processor to process the buffer
    }
}





