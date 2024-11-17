// STM32L432KC_DMA.h
// Header file for DMA peripheral

#ifndef STM32L4_SPI_H
#define STM32L4_SPI_H

#include <stdint.h>
#include <stm32l432xx.h>
#include <stdbool.h>

#define READ_ADDRESS 0x61

#define BUFFER_SIZE_R 1280
#define BUFFER_SIZE_T 640

void initDMA(DMA_Channel_TypeDef * DMAx, SPI_TypeDef * SPIx, bool receiveDMA, uint8_t buffer);

#endif


