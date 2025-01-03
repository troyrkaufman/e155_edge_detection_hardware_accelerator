// STM32L432KC_DMA.h
// Header file for DMA peripheral

#ifndef STM32L4_DMA_H
#define STM32L4_DMA_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stm32l432xx.h>

#define READ_ADDRESS 0x61

void initDMA1Ch2(void);

void initDMA1Ch3(void);

void initDMA2Ch2(void);

void spi_receive_dma(SPI_TypeDef * SPIx, uint8_t * src, uint32_t len);

void spi_transfer_dma(SPI_TypeDef * SPIx, uint16_t * dest, uint32_t len);

#endif
