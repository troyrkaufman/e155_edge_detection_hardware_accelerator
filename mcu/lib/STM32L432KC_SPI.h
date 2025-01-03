// STM32L432KC_SPI.h
// Author: Troy Kaufman
// Email: tkaufman@hmc.edu
// Date: 10/18/24
// Description: This header file initializes the initSPI and spiSendReceive
// prototypes

#ifndef STM32L4_SPI_H
#define STM32L4_SPI_H

#include <stdbool.h>
#include <stdint.h>
#include <stm32l432xx.h>

#define READ_ADDRESS 0x61

///////////////////////////////////////////////////////////////////////////////
// Function prototypes
///////////////////////////////////////////////////////////////////////////////

/* Enables the SPI peripheral and intializes its clock speed (baud rate),
 * polarity, and phase.
 *    -- br: (0b000 - 0b111). The SPI clk will be the master clock / 2^(BR+1).
 *    -- cpol: clock polarity (0: inactive state is logical 0, 1: inactive state
 * is logical 1).
 *    -- cpha: clock phase (0: data captured on leading edge of clk and changed
 * on next edge, 1: data changed on leading edge of clk and captured on next
 * edge) Refer to the datasheet for more low-level details. */

// Initialize SPI
void initSPI(SPI_TypeDef * SPIx,  int br, int cpol, int cpha);

/* Transmits a character (1 byte) over SPI and returns the received character.
 *    -- send: the character to send over SPI
 *    -- return: the character received over SPI */
char spiSendReceive(SPI_TypeDef *SPIx, char send);

/* Compacts SPI transaction into a function */
uint8_t spiTransaction(SPI_TypeDef * SPIx, int CE, char addr, char cmd);

// Sends a 12 bit signal 
uint16_t spiColSend(SPI_TypeDef *SPIx, char byte1, char byte2, char byte3);

#endif