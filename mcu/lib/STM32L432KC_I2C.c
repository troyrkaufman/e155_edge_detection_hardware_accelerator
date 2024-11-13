// STM32L432KC_I2C.h
// Source code for I2C functions

#include "STM32L432KC.h"
#include "STM32L432KC_I2C.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"

void init_I2C() {
    
    // Enable the I2C CLOCK
    RCC->APB1ENR1 |= _VAL2FLD(RCC_APB1ENR1_I2C1EN, 1);

    // Set pclk to 8Mhz by settting the prescalar to 8 (sysclk is 64 Mhz)
    RCC->CFGR |= _VAL2FLD(RCC_CFGR_PPRE1, 0b110);

    // Enable the analog filter
    I2C1->CR1 &= ~I2C_CR1_ANFOFF;

    // Disable digital filter
    I2C1->CR1 &= ~I2C_CR1_DNF;

    // Disable clock stretching
    I2C1->CR1 &= ~I2C_CR1_NOSTRETCH;

    // Set up autoend
    I2C1->CR2 |= I2C_CR2_AUTOEND;

    // Reset I2C Peripheral
    I2C1->CR1 &= ~I2C_CR1_PE; // Disable I2C1
    I2C1->CR1 |= I2C_CR1_SWRST; // Reset I2C
    I2C1->CR1 &= ~I2C_CR1_SWRST; // Clear reset

    // Select Open Drain Output ... USE EXTERNAL PULL UPS (4.7k ~ 10K)

    // Configure the GPIO pins in ALT mode
    gpioEnable(GPIO_PORT_A);
    pinMode(I2C_SCL, GPIO_ALT); 
    pinMode(I2C_SDA, GPIO_ALT); 

    // Set up I2C ALT pin mode 
    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL10, 4);
    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL9, 4);

    //3. Reset the I2C Peripheral
    I2C1->CR1 |= _VAL2FLD(I2C_CR1_SWRST, 1); // Reset System
    I2C1->CR1 |= _VAL2FLD(I2C_CR1_SWRST, 0); // Pull from reset

    //4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
    I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_PRESC, 1);
    I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLDEL, 0x4);
    I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SDADEL, 0x2);
    I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLH, 0xF);
    I2C1->TIMINGR |= _VAL2FLD(I2C_TIMINGR_SCLL, 0x13);

    // Enable the I2C peripheral
    I2C1->CR1 |= I2C_CR1_PE;
}

void write_I2C(char addr, const uint8_t * data, int length){

    // Clear CR2 settings from previous transactions
    I2C1->CR2 &= ~(I2C_CR2_SADD | I2C_CR2_NBYTES | I2C_CR2_START | I2C_CR2_RD_WRN);

    // Master operates in 7-bit addressing mode
    I2C1->CR2 &= ~I2C_CR2_ADD10;

    // Slave address for writing
    I2C1->CR2 |= _VAL2FLD(I2C_CR2_SADD, (addr << 1));

    // Master requests a write transfer
    I2C1->CR2 &= ~I2C_CR2_RD_WRN;

    // Number of bytes that will be sent
    I2C1->CR2 |= _VAL2FLD(I2C_CR2_NBYTES, length);

    // Start I2C Master Write
    I2C1->CR2 |= I2C_CR2_START;

    // Poll for TXIS bit to be set
    for (volatile int i = 0; i < length; i++){
        while (!(I2C1->ISR & I2C_ISR_TXE));
            *(volatile char *) (&I2C1->TXDR) = data[i];
    }

    //autoend will stop the process

}

