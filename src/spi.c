/*
Author: Gabriel Rodgers
Date: 12/30/2024
Purpose: Configure SPI communications as well as provide write functions.
*/

#include <stdint.h>

#include "spi.h"
#include "gpio.h"

//initialize SPI2
void spi2_init(void) {
    gpio_spi2_init();         //init the gpio for SPI2

    SPI2_CR1 |= (0x3 << 3);   //set Fsck to 3MHz (Fpclk/16)
    SPI2_CR1 &= ~(0x3);       //clear CPOL-0 when idle, CPHA-sample on first edge
    SPI2_CR1 |= (0x3 << 14);  //half-duplex, output enable on MOSI
    SPI2_CR1 |= (1 << 2);     //configure STM32 as master

    SPI2_CR2 |= (0x7 << 8);   //8-bit data size
    //SPI2_CR2 |= (1 << 2);     //set SS enable - pulls SS low while SPI2 is enabled (single master)

    SPI2_CR2 |= (1 << 3);     //set NSS pulse enable 

    SPI2_CR1 |= (1 << 6);     //enable SPI2
}

//write a single byte to MOSI using SPI2
void spi2_write(uint8_t data) {
    while (!(SPI2_SR & 0x2));     //wait for TXE flag to be set (transmit buffer empty)
    SPI2_DR = data;
}
