/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include "main.h"
#include "main.h"
#include <stdio.h>

/*
uint8_t rxBuffer1[BUFFER_SIZE_R] __attribute__((section(".sram1_data")));
uint8_t rxBuffer2[BUFFER_SIZE_R] __attribute__((section(".sram1_data")));

uint8_t *currentBufferR = rxBuffer1;    // Active DMA buffer
uint8_t *processBufferR = NULL;         // Buffer whose data is ready to be processed
uint8_t *regcnfgr       = regBuf;       // Points at current register address in camera module to access pixel data                                
uint8_t *currentBufferT  = txBuffer1;    // ACtive DMA buffer receiving processed data
uint8_t *transmitBufferT = NULL;         // Buffer whose data is read to be sent over SPI
*/

uint16_t txBuffer1[BUFFER_SIZE_T] __attribute__((section(".sram1_data")));
uint16_t txBuffer2[BUFFER_SIZE_T] __attribute__((section(".sram1_data")));

uint16_t *currentBufferT = txBuffer1;
uint16_t *transmitBufferT = NULL;
//uint8_t bufferFullR = 0;                 // Flag to indicate that buffer is ready

int main(void) {
  // Buffers for PLL initialization
  configureFlash();
  configureClock();

  // Enable interrupts globally
  __enable_irq();
  // Enable interrupts globally
  __enable_irq();

  // Enable the GPIO ports
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOBEN, 1);
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOCEN, 1);
  RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_TIM15EN, 1);
  // Enable the GPIO ports
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOBEN, 1);
  RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOCEN, 1);
  RCC->APB2ENR |= _VAL2FLD(RCC_APB2ENR_TIM15EN, 1);

  initTIM(TIM15);

  ///////////////////////////////
  // SPI Configuration
  ///////////////////////////////
  ///////////////////////////////
  // SPI Configuration
  ///////////////////////////////

  // Designate the SPI pins for Camera Module and MCU communication
  pinMode(PA5, GPIO_ALT);       // SCK
  pinMode(PA7, GPIO_ALT);       // MOSI 
  pinMode(PA6, GPIO_ALT);       // MISO
  pinMode(PA8, GPIO_OUTPUT);    // CE

  // Give SPI pins proper ALT functions
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);    // SCK PA5
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL7, 5);    // MOSI PA7 
  GPIOA->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL6, 5);    // MISO PA6

  // Initialize SPI Peripherals (SPI1 and SPI3)
  initSPI(SPI1, 0, 0, 0);  // Setting necessary DMA bits for receiving and sending

  // Ensure that 
  digitalWrite(CE1, 1);

  /*
  spiTransaction(SPI1, CE1, 0x87, 0x80);
  spiTransaction(SPI1, CE1, 0x87, 0x00);

  while(1) {
    spiTransaction(SPI1, CE1, 0x3d, 0x00);
  }
*/
  ////////////////////////////////
  // DMA configuration
  ////////////////////////////////

  // Enable DMA Channels
  RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN);

  // Initialize DMA Channel
  initDMA1Ch3(); // M -> P

  // Keep CE low all the time
  digitalWrite(CE1, 0);

  // Prepare DMA1 channel(s) 2 and 3 to receive pixel data and send sclk packets/burst read fifo command
  //spi_transfer_dma(SPI1, currentBufferT, 1);

  ////////////////////////////////
  // Main Loop
  ////////////////////////////////

  while (1){
    //if (bufferFullR)
    // Grab the necessary nibbles and concatenate properly into a 12 bit packet. Then place nibble within 
    // Legit question, DMA is useless in this case now because of the camera's absence
    for (volatile int i =0; i < sizeof(grayBuf); i++){
      spiColSend(SPI1, CE1, grayBuf[i], grayBuf[i+3], grayBuf[i+6]);
    }
     }
  }








  /*
// Interrupt handler for DMA1Channel3 for Transmission
void DMA1_Channel3_IRQHandler(void) {
    DMA1->IFCR &= ~(DMA_IFCR_CHTIF3 | DMA_IFCR_CGIF3 | DMA_IFCR_CTEIF3);
  if (DMA1->ISR & DMA_ISR_TCIF3) {        // Transfer Complete Interrupt
    DMA1->IFCR |= DMA_IFCR_CTCIF3;        // Clear the interrupt flag
    
    if (currentBufferT == txBuffer1) {
      transmitBufferT      = txBuffer1;    // Assign the filled buffer for processing
      currentBufferT      = txBuffer2;    // Switch DMA to the second buffer
      DMA1_Channel3->CMAR = (uint32_t)&txBuffer2;
    } else {
      transmitBufferT      = txBuffer2;
      currentBufferT      = txBuffer1;
      DMA1_Channel3->CMAR = (uint32_t)&txBuffer1;
    }

    //bufferFullR = 1;    // Flag that allows core to process the buffer
  }
}
*/











/*

 // Clear the internal ArudCAM's internal Fifo
  spiTransaction(SPI1, CE1, 0x87, 0x80);    // Reset CPLD
  spiTransaction(SPI1, CE1, 0x87, 0x00);    // Reset CPLD
  */

/*
// Designate the SPI pins for MCU and FPGA communication
  //pinMode(PB3, GPIO_ALT);       // SCK
  //pinMode(PB5, GPIO_ALT);       // MOSI
  //pinMode(PB4, GPIO_ALT);       // MISO
  //pinMode(PB1, GPIO_OUTPUT);    // CE

  //GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL3, 5);    // SCK PB3
  //GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL5, 5);    // MOSI PB5
  //GPIOB->AFR[0] |= _VAL2FLD(GPIO_AFRL_AFSEL4, 5);    // MISO PB4
  */

/*
////////////////////////////////
  // DMA configuration
  ////////////////////////////////

  // Enable DMA Channels
  RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN);
  //RCC->AHB1ENR |= (RCC_AHB1ENR_DMA2EN);

  // Initialize DMA Channels
  //initDMA1Ch2(); // P -> M
  initDMA1Ch3(); // M -> P
  //initDMA2Ch2(); // P -> M

  // Keep CE low all the time
  digitalWrite(CE1, 0);

  // Prepare DMA1 channel(s) 2 and 3 to receive pixel data and send sclk packets/burst read fifo command
  spi_receive_dma(SPI1, currentBufferR, 40);
  spi_transfer_dma(SPI1, regcnfgr, 40);

  // Prepare DMA2 channel 2 to transfer processed pixel data to the FPGA
  spi_transfer_dma(SPI3, txBuffer1, 1);

*/
















/*
uint8_t grayscaleConversion(uint16_t pixel) {
  // Extract RGB components from the 16-bit pixel (RGB565 format)
  uint8_t red   = (pixel & 0xF800) >> 11;    // Top 5 bits
  uint8_t green = (pixel & 0x07E0) >> 5;     // Middle 6 bits
  uint8_t blue  = pixel & 0x001F;            // Bottom 5 bits

  // Scale the values to 8-bit range
  red   = (red * 255) / 31;
  green = (green * 255) / 63;
  blue  = (blue * 255) / 31;

  // Compute the grayscale value using standard formula
  return (red * 30 + green * 59 + blue * 11) / 100;
}

void processData(void) { // User input would enter here...
  if (bufferFullR && processBufferR) {
    // Determine which output buffer to use
    uint8_t *outputBuffer = (currentBufferT == txBuffer1) ? txBuffer2 : txBuffer1;

    for (int i = 0; i < BUFFER_SIZE_R; i += PIXEL_BYTES) {
      uint16_t pixel = (processBufferR[i] << 8) | processBufferR[i + 1];    // Combine bytes to get RGB565 pixel
      outputBuffer[i / PIXEL_BYTES] = grayscaleConversion(pixel);           // Convert to grayscale and store
    }
    // Update flags and pointers
    transmitBufferT = outputBuffer;    // Assign output buffer for SPI transmission
    currentBufferT  = outputBuffer;    // Switch to the new output buffer
    bufferFullR     = 0;               // Clear flag after processing
    processBufferR  = NULL;            // Reset buffer pointer

    // toggle CE
    digitalWrite(CE1, 1);
    digitalWrite(CE1, 0);
  }
}
*/




/*

  ////////////////////////////////
  // I2C configuration
  ////////////////////////////////

  init_I2C();

  for (volatile int i = 0; i < LENGTH; i++) {
    write_I2C(ADDR, reg[i], data[i]);
  }

  */












/*
    uint8_t foo = 0;
    spiTransactionRead(SPI1, CE1, 0x81, 0x00);    // Set to Capture 1 Frame
    delay_millis(TIM15, 1);
    spiTransactionRead(SPI1, CE1, 0x85, 0x01);    // Test Mode Test Data
    delay_millis(TIM15, 1);
    spiTransactionRead(SPI1, CE1, 0x87, 0x80);    // Reset CPLD
    delay_millis(TIM15, 1);
    spiTransactionRead(SPI1, CE1, 0x87, 0x00);    // Lift Reset (CPLD)
    delay_millis(TIM15, 1);
    spiTransactionRead(SPI1, CE1, 0x84, 0x11);    // Reset Fifo Read Pointer and Clear Done Bit
    delay_millis(TIM15, 1);
    spiTransactionRead(SPI1, CE1, 0x84, 0x02);    // Start Capture

    // Poll for Frame Completion
    while (frame_done != 0x08) {
      frame_done = spiTransactionRead(SPI1, CE1, 0x41, 0x00);    // read the FIFO done flag
      spiTransactionRead(SPI1, CE1, 0x42, 0x00);    // Is there data written in the least sig. 8 bits in fifo
      spiTransaction(SPI1, CE1, 0x45, 0x00);        // Is Fifo full?
    }

    uint32_t fifoLength = 0;
    fifoLength          = readFifoLength();

    // Initate Read and Extract JPEG Data
    for (volatile int i = 0; i < fifoLength; i++) {
      spiTransaction(SPI1, PA8, 0x3d, 00);
    }
  }
  */

