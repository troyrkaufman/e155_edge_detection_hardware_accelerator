// STM32L432KC_DMA.c
// Source code for DMA peripheral

#include "STM32L432KC_DMA.h"

uint8_t rxbuffer1[BUFFER_SIZE_R];
uint8_t rxbuffer2[BUFFER_SIZE_R];

uint8_t txbuffer1[BUFFER_SIZE_T];
uint8_t txbuffer2[BUFFER_SIZE_T];

void initDMA(DMA_Channel_TypeDef * DMAx, SPI_TypeDef * SPIx, bool receiveDMA){
    // TODO: Reset DMA channel configuration

    // Turn on DMA controller in RCC
    RCC->AHB1ENR |= (RCC_AHB1ENR_DMA1EN);

    // Reset DMA controller
    DMAx->CCR  &= ~(0xFFFFFFFF);
    DMAx->CCR  |= (_VAL2FLD(DMA_CCR_PL,0b10) |
                            _VAL2FLD(DMA_CCR_MINC, 0b1) |
                            _VAL2FLD(DMA_CCR_CIRC, 0b1) |
                            _VAL2FLD(DMA_CCR_DIR, 0b1)
                            );
    // TODO: Set DMA Channel configuration

    // Turn on memory address incrementing
    DMAx->CCR |= _VAL2FLD(DMA_CCR_MINC, 1);

    // Turn on circular addressing
    DMAx->CCR |= _VAL2FLD(DMA_CCR_CIRC, 1);

    // Source: Address of the peripheraly.
    DMAx->CPAR |= _VAL2FLD(DMA_CPAR_PA, (uint32_t) &SPIx->DR);

    
    if (receiveDMA == true){
        // Set the direction to be from peripheral to memory 
        DMAx->CCR |= _VAL2FLD(DMA_CCR_DIR, 0);

        // Set the priority level
        DMAx->CCR |= _VAL2FLD(DMA_CCR_PL, 1);

        // TODO: Dest.: Memory address
        DMAx->CMAR |= _VAL2FLD(DMA_CMAR_MA, (uint32_t) &rxbuffer1);

        // TODO: Set DMA data transfer length (# of samples).
        DMAx->CNDTR |= BUFFER_SIZE_R;

         // TODO: Select correct selection for DMA channel
        DMA1_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 1);
    } else {
        // Set the direction to be  memory to peripheral
        DMAx->CCR |= _VAL2FLD(DMA_CCR_DIR, 1); 

        // Set the priority level
        DMAx->CCR |= _VAL2FLD(DMA_CCR_PL, 2);

        // TODO: Dest.: Memory address
        DMAx->CMAR |= _VAL2FLD(DMA_CMAR_MA, (uint32_t) &txbuffer1);

        // TODO: Set DMA data transfer length (# of samples).
        DMAx->CNDTR |= BUFFER_SIZE_T;

         // TODO: Select correct selection for DMA channel
        DMA2_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 3);
    }
    
    // TODO: Enable DMA channel.
    DMAx->CCR  |= DMA_CCR_EN;
}