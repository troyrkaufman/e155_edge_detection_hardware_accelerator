/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include <stdio.h>
#include "main.h"


uint8_t rxBuffer1[BUFFER_SIZE_R] __attribute__((section(".sram1_data")));
uint8_t rxBuffer2[BUFFER_SIZE_R] __attribute__((section(".sram1_data")));

uint8_t txBuffer1[BUFFER_SIZE_T] __attribute__((section(".sram1_data")));
uint8_t txBuffer2[BUFFER_SIZE_T] __attribute__((section(".sram1_data")));

// DMA1 Channels 2 and 3
 uint8_t * currentBufferR = rxBuffer1; // Active DMA buffer 
 uint8_t * processBufferR = NULL;      // Buffer whose data is ready to be processed
 uint8_t * regcnfgr = regBuf;         // Points at current register address in camera module to access pixel data

// DMA2 Channels 2 and 3
 uint8_t * currentBufferT = txBuffer1; // ACtive DMA buffer receiving processed data
 uint8_t * transmitBufferT = NULL;      // Buffer whose data is read to be sent over SPI

 uint8_t bufferFullR = 0;              // Flag to indicate that buffer is ready

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
        // Determine which output buffer to use
        uint8_t *outputBuffer = (currentBufferT == txBuffer1) ? txBuffer2 : txBuffer1;

        for (int i = 0; i < BUFFER_SIZE_R; i += PIXEL_BYTES){
            uint16_t pixel = (processBufferR[i] << 8) | processBufferR[i+1]; // Combine bytes to get RGB565 pixel
            outputBuffer[i / PIXEL_BYTES] = grayscaleConversion(pixel);     // Convert to grayscale and store
        }
        // Update flags and pointers
        transmitBufferT = outputBuffer;  // Assign output buffer for SPI transmission
        currentBufferT = outputBuffer; // Switch to the new output buffer
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
    RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_TIM15EN, 1);

    initTIM(TIM15);

    ////////////////////////////////
    // I2C configuration
    ////////////////////////////////
    
    init_I2C();
    //digitalWrite(PA8, 1);

    for (volatile int i = 0; i < LENGTH; i++){
      if (i == 3) {
        delay_millis(TIM15, 5);
      }
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
    //pinMode(PB0, GPIO_ALT); // NSS 

    // Designate the SPI pins for MCU and FPGA communication
    //pinMode(PB3, GPIO_ALT);    // SCK
    //pinMode(PB5, GPIO_ALT);    // MOSI
    //pinMode(PB4, GPIO_ALT);    // MISO
    //pinMode(PC15, GPIO_OUTPUT); // CE

    // Give SPI pins proper ALT functions
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // SCK PA5
    GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5); // MOSI PA7 --> PB5 For now
    GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5); // MISO PA6
    //GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL0, 5); // NSS PB0

    initSPI(SPI1, 7, 0, 0, true);

    ////////////////////////////////
    // DMA configuration
    ////////////////////////////////
    // Enable DMA Channels
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA1EN);
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA2EN);

    
    
    //while(1){
    //spiTransaction(SPI1, PA8, 0x3d); 
    //}
   

    initDMA1Ch2();
    initDMA1Ch3();

    digitalWrite(PA8, 0);
    spi_receive_dma(SPI1, currentBufferR, 40);
    spi_transfer_dma(SPI1, regcnfgr, 40);
    
    ////////////////////////////////
    // Main Loop
    ////////////////////////////////

      while(1){
        if (bufferFullR) {
          processData();
        }
      }
}

// Interrupt handler for DMA1Channel2 for RECEPTION
void DMA1_Channel2_IRQHandler(void) {
    if (DMA1->ISR & DMA_ISR_TCIF2) {   // Transfer Complete Interrupt
        SPI1->CR1 &= ~SPI_CR1_SPE; // Disable SPI after transfer
        DMA1->IFCR |= DMA_IFCR_CTCIF2; // Clear the interrupt flag
        //SPI1->CR1 &= ~SPI_CR1_SPE; // Disable SPI after transfer

        digitalWrite(PA8, 1);
        delay_millis(TIM15, 1);
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

        bufferFullR = 1; // Flag that allows core to process the buffer
    }
    //SPI1->CR1 |= SPI_CR1_SPE; // Re-enable SPI before starting the next transfer

    digitalWrite(PA8, 0);
}

/*
void DMA1_Channel3_IRQHandler(void){
  if (DMA1->ISR & DMA_ISR_TCIF3) {   // Transfer Complete Interrupt
        DMA1->IFCR |= DMA_IFCR_CTCIF3; // Clear the interrupt flag
        SPI1->CR1 &= ~SPI_CR1_SPE; // Disable SPI after transfer

        digitalWrite(PA8, 1);

        digitalWrite(PA8, 0);

        SPI1->CR1 |= SPI_CR1_SPE; // Re-enable SPI before starting the next transfer

        
  }
}

*/






















































































/*   // Reset DMA2 Channel 2
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA2EN);
    DMA2_Channel2->CCR  &= ~(0xFFFFFFFF);
    DMA2_Channel2->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b11) |   // Priority is set to high
                            _VAL2FLD(DMA_CCR_MINC, 0b1) | // memory address updates after every transmission
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) | // DMA NBYTE count will update to declared value
                            _VAL2FLD(DMA_CCR_DIR, 0b1) |  // Peripheral to memory transfer
                            _VAL2FLD(DMA_CCR_MSIZE, 0b00) | // Set to byte length
                            _VAL2FLD(DMA_CCR_PSIZE, 0b00)   // Set to byte length
                            );
    
    // Set DMA source and destination addresses.
    // Source: Address of the data from memory
    DMA2_Channel3->CPAR = _VAL2FLD(DMA_CPAR_PA, (uint32_t) &currentBufferT);

    // DEST.: Address of the SPI3 data register
    DMA2_Channel3->CMAR = _VAL2FLD(DMA_CMAR_MA, (uint32_t) &(SPI3->DR));

    // Set DMA data transfer length (# of samples)
    DMA2_Channel2->CNDTR |= _VAL2FLD(DMA_CNDTR_NDT, 1); // # pix per row * pixel width * # rows : 640 * 2 * 3
    
    // Select the 3rd option for mux to channel 2
    DMA2_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 3);

    // Enable interrupt bit for channel 2
    DMA2_Channel3->CCR |=  _VAL2FLD(DMA_CCR_TCIE, 1);

    // Enable DMA1 channel.
    DMA2_Channel3->CCR  |= DMA_CCR_EN;

    // Enable the interrupt for DMA1 Channel2
    NVIC->ISER[1] |= (1<<25); // 57...25 DMA2_Channel2_IRQn

    */











































/*

     // RECEPTION FOR SPI1 DMA1CH2
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
    DMA1_Channel2->CMAR = _VAL2FLD(DMA_CMAR_MA, (uint32_t) currentBufferR);

    // Set DMA data transfer length (# of samples)
    DMA1_Channel2->CNDTR |= _VAL2FLD(DMA_CNDTR_NDT, 3840); // # pix per row * pixel width * # rows : 640 * 2 * 3
    
    // Select the 1st option for mux to channel 2
    DMA1_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 0b0001);

    // Enable interrupt bit for channel 2
    DMA1_Channel2->CCR |=  _VAL2FLD(DMA_CCR_TCIE, 1);

    // Enable DMA1 channel.
    DMA1_Channel2->CCR  |= DMA_CCR_EN;

    // Enable the interrupt for DMA1 Channel2
    NVIC->ISER[1] |= (1<<DMA1_Channel2_IRQn);

    // TODO: TRANSMISSION FOR SPI1 DMA1CH3 
    // Reset DMA1 Channel 3 
    RCC->AHB1ENR  |= (RCC_AHB1ENR_DMA1EN);
    DMA1_Channel3->CCR  &= ~(0xFFFFFFFF);
    DMA1_Channel3->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b10) |   // Priority is set to medium
                            _VAL2FLD(DMA_CCR_MINC, 0b1) | // memory address updates after every reception
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) | // DMA NBYTE count will update to declared value
                            _VAL2FLD(DMA_CCR_DIR, 0b1) |  // Memory to peripheral transfer CHANGE BACK TO 1
                            _VAL2FLD(DMA_CCR_MSIZE, 0b00) | // Set to byte length
                            _VAL2FLD(DMA_CCR_PSIZE, 0b00)   // Set to byte length
                            );
    
    // Set DMA source and destination addresses.
    // DEST: Address of the data from peripheral
    DMA1_Channel3->CPAR = _VAL2FLD(DMA_CPAR_PA, (uint32_t) &(SPI1->DR));

    // Source.: Address of the configuration address list
    DMA1_Channel3->CMAR = _VAL2FLD(DMA_CMAR_MA, (uint32_t) regcnfgr);

    // Set DMA data transfer length (# of samples)
    DMA1_Channel3->CNDTR |= _VAL2FLD(DMA_CNDTR_NDT, 40); // # pix per row * pixel width * # rows : 640 * 2 * 3
    
    // Select the 1st option for mux to channel 3
    DMA1_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 0b0001);

    // Enable interrupt bit for channel 3 -----------------Is an interrupt needed?
    DMA1_Channel3->CCR |=  _VAL2FLD(DMA_CCR_TCIE, 1);

    // Enable DMA1 channel3
    DMA1_Channel3->CCR  |= DMA_CCR_EN;

    // Enable the interrupt for DMA1 Channel3
    NVIC->ISER[1] |= (1<<DMA1_Channel3_IRQn);
*/