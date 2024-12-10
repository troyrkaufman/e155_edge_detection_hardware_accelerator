// Troy Kaufman
// Source code for parsing a provided bin file portraying an image

#include "PROCESS_IMAGE.h"
#include "STM32L432KC_SPI.h"
#include "STM32L432KC_TIM.h"

#define LENGTH 320
#define WIDTH 240

void processFile(void) {
    char nibble1, nibble2, nibble3;
    
    for (volatile int i = 0; i < LENGTH*WIDTH/2; i++){
        // Extract first nibble
        if (i % 2 == 0){
            char nibble1 = pix_data[i] & 0xf0;
        } else {
            char nibble1 = pix_data[i] & 0x0f;
        }

        // Extract second nibble
        if (i % 2 == 0){
            char nibble2 = pix_data[i * WIDTH] & 0xf0;
        } else {
            char nibble2 = pix_data[i * WIDTH] & 0x0f;
        }

        // Extract thrid nibble
        if (i % 2 == 0){
            char nibble3 = pix_data[i * 2 * WIDTH] & 0xf0;
        } else {
            char nibble3 = pix_data[i * 2 * WIDTH] & 0x0f;
        }
  
        spiColSend(SPI1, nibble1, nibble2, nibble3);

        delay_millis(TIM15, 1);
    }
}