/*
File: main.c
Author: Troy Kaufman
Email: tkaufman@hmc.edu
Date: 11/12/24
*/

#include "main.h"
#include <stdio.h>


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

  // Initialize Timer
  initTIM(TIM15);

  // Initialize SPI Peripheral 
  initSPI(SPI1, 1, 0, 0);  

  // Ensure that 
  digitalWrite(CE1, 1);
 
  // Keep CE low all the time
  //digitalWrite(CE1, 0);

  ////////////////////////////////
  // Main Loop
  ////////////////////////////////

  while (1){
    processFile();
    }
  }







