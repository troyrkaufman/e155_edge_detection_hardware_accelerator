// STM32L432KC.h
// Header to include all other STM32L432KC libraries.

#ifndef STM32L4_H
#define STM32L4_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stm32l432xx.h>  // CMSIS device library include
#include <stdbool.h>

// Include other peripheral libraries

#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"
#include "STM32L432KC_TIM.h"
#include "STM32L432KC_FLASH.h"
#include "STM32L432KC_USART.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_I2C.h"
#include "STM32L432KC_DMA.h"
//#include "ov2640_updated.h"
//#include "ArduCAM.h"
//#include "sccb_bus.h"
//#include "spi.h"

/*
../STM32L152/HARDWARE/ArduCAM/ArduCAM.c
../STM32L152/HARDWARE/ArduCAM/ArduCAM.h
../STM32L152/HARDWARE/ArduCAM/ov2640_updated.h
../STM32L152/HARDWARE/I2C/sccb_bus.c
../STM32L152/HARDWARE/I2C/sccb_bus.h
../STM32L152/HARDWARE/SPI/spi.c
../STM32L152/HARDWARE/SPI/spi.h
*/

// Global defines

#define HSI_FREQ 16000000 // HSI clock is 16 MHz
#define MSI_FREQ 4000000  // HSI clock is 4 MHz


#endif