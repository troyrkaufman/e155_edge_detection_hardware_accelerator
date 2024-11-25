// STM32L432KC_DMA.h
// Header file for DMA peripheral

#ifndef STM32L4_DMA_H
#define STM32L4_DMA_H

#include <stdint.h>
#include <stm32l432xx.h>
#include <stdbool.h>
#include <stdlib.h>

#define READ_ADDRESS 0x61

void initDMA1Ch2(void);

void initDMA1Ch3(void);

void spi_receive_dma(SPI_TypeDef * SPIx, uint8_t * src, uint32_t len);

void spi_transfer_dma(SPI_TypeDef * SPIx, uint8_t * dest, uint32_t len);

#endif


