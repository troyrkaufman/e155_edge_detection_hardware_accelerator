/**
    Main Header: Contains general defines and selected portions of CMSIS files
    @file main.h
    @author Troy Kaufman
    @version 1.0 11/23/2024
*/

#ifndef MAIN_H
#define MAIN_H

#include "../lib/STM32L432KC.h"

#define ADDR 0x30 // write ADDRess to camera module
#define LENGTH 43
//#define BUFFER_SIZE_R 3840
//#define BUFFER_SIZE_T 3
#define PIXEL_BYTES 2

#define I2C_SCL PA9
#define I2C_SDA PA10

// SPI1 Pins
#define SCLK1 PA5
#define MOSI1 PB5
#define MISO1 PA6
#define CE1   PA8

// SPI3 Pins
#define SCLK2 PB3
#define MOSI2 PA7
#define MISO2 PB4
#define CE2   PB1

const unsigned char reg[] = {
    0xff, 0x11, 0x12, 0xDA, 0xC2, 0x17, 0x18, 0x32, 0x19, 0x1a, 
    0x03, 0x37, 0x4f, 0x50, 0x5a, 0x6d, 0x3d, 0x39, 0x35, 0x22, 
    0x37, 0x34, 0x06, 0x0d, 0x0e, 0xff, 0xe0, 0xc0, 0xc1, 0x86, 
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x57, 0x5a, 0x5b, 0x5c, 
    0xd3, 0xe0, 0xff
};

const unsigned char data[] = {
    0x00, 0x01, 0x00, 0x10, 0x01, 0x11, 0x75, 0x36, 0x01, 0x97, 
    0x0f, 0x40, 0xbb, 0x9c, 0x57, 0x80, 0x34, 0x02, 0x88, 0x0a, 
    0x40, 0xa0, 0x02, 0xb7, 0x01, 0x00, 0x04, 0xc8, 0x96, 0x3d, 
    0x89, 0x90, 0x2c, 0x00, 0x00, 0x88, 0x00, 0xa0, 0x78, 0x00, 
    0x04, 0x00, 0xff
};

uint8_t regBuf[40] = { // 40 bytes  
    0x3d, 0x00, 0x3d, 0x00, 0x3d, 0x00, 0x3d, 0x00, 0x3d, 0x00, 
    0x3d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,};

#endif // MAIN_H