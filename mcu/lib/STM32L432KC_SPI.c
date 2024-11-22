// STM32L432KC_SPI.c
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/18/24
// Description: This C file creates the initSPI and spiSendReceive functions

#include "STM32L432KC_SPI.h"

void initSPI(SPI_TypeDef * SPIx, int br, int cpol, int cpha, bool receive){

// TODO: Add SPI initialization code
   // Turn on SPI clock domain
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
  
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_BR, 0b0111);
    
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CPOL, cpol);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CPHA, cpha);
    
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_LSBFIRST, 0b0);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_CRCEN, 0b0);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_SSM, 0b1);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_SSI, 0b1);
    SPIx->CR1 |= _VAL2FLD(SPI_CR1_MSTR, 0b1);

    SPIx->CR2 |= _VAL2FLD(SPI_CR2_DS, 0b0111);
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_SSOE, 0b1);
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_FRF, 0b0);
    SPIx->CR2 |= _VAL2FLD(SPI_CR2_FRXTH, 0b1);
    
    SPIx->CR2 &= ~SPI_CR2_NSSP;

    // DMA request logic
    if (receive == true){
        SPIx->CR2 |= _VAL2FLD(SPI_CR2_RXDMAEN, 1);
    } else {
        SPIx->CR2 |= _VAL2FLD(SPI_CR2_TXDMAEN, 1);
    }

    SPIx->CR1 |= SPI_CR1_SPE;
}

char spiSendReceive(SPI_TypeDef * SPIx, char send){
    // Wait until the TXFIFO level is less than half of its capacity to send data
    while(!(SPIx->SR & SPI_SR_TXE)); 
    
    // Send byte once, however, we run into a slight problem since we set our bit width to 8 bits in SPI
    // 1) First capture address of the 16 bit DR
    // 2) Cast this 16 bit value as a volatile 8 bit integer pointer
    // 3) Then dereference this value
    *((volatile uint8_t *) &SPIx->DR) = send;             

    // Wait until there is data in the RXFIFO
    while((!(SPIx->SR & SPI_SR_RXNE)));
    
    // Read byte once
    return SPIx->DR;
}
