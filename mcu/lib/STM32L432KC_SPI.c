// STM32L432KC_SPI.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/18/24
// Description: This C file creates the initSPI and spiSendReceive functions

#include "STM32L432KC_SPI.h"
#include "STM32L432KC_GPIO.h"

void initSPI(SPI_TypeDef * SPIx, int br, int cpol, int cpha){

// TODO: Add SPI initialization code
   // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_BR, br);
    
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
    
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_LSBFIRST, 0b0);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CRCEN, 0b0);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_SSM, 0b1);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_SSI, 0b1);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 0b1);

    SPIx->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b1111); // 16 bit data packet
    //SPIx->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111); 
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 0b1);
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_FRF, 0b0);
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0b1);
    
    SPIx->CR2 &= ~SPI_CR2_NSSP;

    //SPIx->CR2 |= SPI_CR2_TXDMAEN; // Enable the DMA for transmission

  SPIx->CR1 |= SPI_CR1_SPE;
}

char spiSendReceive(SPI_TypeDef *SPIx, char send) {
  // Wait until the TXFIFO level is less than half of its capacity to send data
  while (!(SPIx->SR & SPI_SR_TXE))
    ;

  // Send byte once, however, we run into a slight problem since we set our bit
  // width to 8 bits in SPI 1) First capture address of the 16 bit DR 2) Cast
  // this 16 bit value as a volatile 8 bit integer pointer 3) Then dereference
  // this value
  *((volatile uint8_t *)&SPIx->DR) = send;

  // Wait until there is data in the RXFIFO
  while ((!(SPIx->SR & SPI_SR_RXNE)))
    ;

  // Read byte once
  return SPIx->DR;
}

uint8_t spiTransaction(SPI_TypeDef * SPIx, int CE, char addr, char cmd){
    uint8_t readByte;
    digitalWrite(CE, 0);
    spiSendReceive(SPIx, addr);
    readByte = spiSendReceive(SPIx, cmd);
    digitalWrite(CE, 1);
    return readByte;
}

uint16_t spiColSend(SPI_TypeDef *SPIx, int CE, char byte1, char byte2, char byte3){
    uint8_t nib1 = byte1 & 0x0F; // Mask the lower nibble
    uint8_t nib2 = byte2 & 0x0F; // Mask the lower nibble
    uint8_t nib3 = byte3 & 0x0F; // Mask the lower nibble

    // Concatenate the nibbles into a 16-bit value
    uint16_t dataToSend = (nib1 << 8) | (nib2 << 4) | nib3;

    // Wait until the SPI is ready to transmit
    while (!(SPIx->SR & SPI_SR_TXE));

    // Send the 16-bit value
    SPIx->DR = dataToSend;

    // Wait until the transmission is complete
    while (SPIx->SR & SPI_SR_BSY);
    
    
    
    /*
    digitalWrite(CE, 0);
    
    while(!(SPIx->SR & SPI_SR_TXE)); 
    *((volatile uint16_t *) &SPIx->DR) = ((nib1<<8) | (nib2<<4) | nib3);
    while((!(SPIx->SR & SPI_SR_RXNE)));
    return SPIx->DR;
    digitalWrite(CE, 1);
    */
}



