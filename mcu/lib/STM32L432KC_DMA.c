// STM32L432KC_DMA.c
// Source code for DMA peripheral

#include "STM32L432KC_DMA.h"

/*
void initDMA1Ch2(){
// Reset DMA1 Channel 2
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

*/





/*
void initDMA(DMA_Channel_TypeDef * DMAx, SPI_TypeDef * SPIx, bool receiveDMA, uint8_t buffer){
    // TODO: Reset DMA channel configuration

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

    // Source: Address of the peripheral
    DMAx->CPAR |= _VAL2FLD(DMA_CPAR_PA, (uint32_t) &SPIx->DR);

    
    if (receiveDMA == true){
        // Set the direction to be from peripheral to memory 
        DMAx->CCR |= _VAL2FLD(DMA_CCR_DIR, 0);

        // Set the priority level
        DMAx->CCR |= _VAL2FLD(DMA_CCR_PL, 1);

        // TODO: Dest.: Memory address
        DMAx->CMAR |= _VAL2FLD(DMA_CMAR_MA, (uint32_t) &buffer);

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
        DMAx->CMAR |= _VAL2FLD(DMA_CMAR_MA, (uint32_t) &buffer);

        // TODO: Set DMA data transfer length (# of samples).
        DMAx->CNDTR |= BUFFER_SIZE_T;

         // TODO: Select correct selection for DMA channel
        DMA2_CSELR->CSELR |= _VAL2FLD(DMA_CSELR_C2S, 3);
    }
    
    // TODO: Enable DMA channel.
    DMAx->CCR  |= DMA_CCR_EN;
} */