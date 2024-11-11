// STM32L432KC_I2C.h
// Source code for I2C functions

#include "STM32L432KC.h"
#include "STM32L432KC_I2C.h"
#include "STM32L432KC_GPIO.h"
#include "STM32L432KC_RCC.h"

void init_I2C() {
    
    //1. Enable the I2C CLOCK and GPIO CLOCK
    RCC->APB1ENR1 |= _VAL2FLD(RCC_APB1ENR1_I2C1EN, 1);
    //2. Configure the I2C PINs for ALternate Functions
    //a) Select Alternate Function in MODER Register
    RCC->AHB2ENR |= _VAL2FLD(RCC_AHB2ENR_GPIOAEN, 1);
    //b) Select Open Drain Output 
    GPIOA->OTYPER |= _VAL2FLD(GPIO_OTYPER_OT9, 1);
    GPIOA->OTYPER |= _VAL2FLD(GPIO_OTYPER_OT10, 1);
    //c) Select High SPEED for the PINs-----Nope. We will be using the normal 100KHz clk mode
    
    //d) Select Pull-up for both the Pins-----USE EXTERNAL PULL UPS (4.7k ~ 10K)

    //e) Configure the Alternate Function in AFR Register
    gpioEnable(GPIO_PORT_A);
    pinMode(I2C_SCL, GPIO_ALT); // I2C1_SCL
    pinMode(I2C_SDA, GPIO_ALT);// I2C1_SDA

    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL10, 0b100);
    GPIOA->AFR[1] |= _VAL2FLD(GPIO_AFRH_AFSEL9, 0b100);

    //3. Reset the I2C 	
    I2C1->CR1 |= _VAL2FLD(I2C_CR1_SWRST, 1); // Reset System
    I2C1->CR1 |= _VAL2FLD(I2C_CR1_SWRST, 0); // Pull from reset

    //4. Program the peripheral input clock in I2C_CR2 Register in order to generate correct timings
    
    //5. Configure the clock control registers
    //6. Configure the rise time register
    //7. Program the I2C_CR1 register to enable the peripheral

}

