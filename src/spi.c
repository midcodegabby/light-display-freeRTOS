/*
Author: Gabriel Rodgers
Date: 12/30/2024
Purpose: Configure SPI communications as well as provide write functions.
*/

#include <stdint.h>

#include "spi.h"
#include "gpio.h"
#include "tcnt.h"

//initialize SPI2
void spi2_init(void) {
    gpio_spi2_init();         //init the gpio for SPI2

    SPI2_CR1 |= (0x3 << 3);   //set Fsck to 3MHz (Fpclk/16)
    SPI2_CR1 &= ~(0x3);       //clear CPOL-0 when idle, CPHA-sample on first edge
    SPI2_CR1 |= (0x3 << 14);  //half-duplex, output enable on MOSI
    SPI2_CR1 |= (1 << 2);     //configure STM32 as master

    SPI2_CR2 |= (0x7 << 8);   //8-bit data size
    SPI2_CR2 |= (1 << 3);     //set NSS pulse enable - pulses NSS every time SCLK starts

    SPI2_CR1 |= (1 << 6);     //enable SPI2
}

//write a single byte to MOSI using SPI2
void spi2_write(uint8_t data) {
    SPI2_DR = data;
    while (!(SPI2_SR & 0x2));     //wait for TXE flag to be set (transmit buffer empty)
    timer3_delay_us(2);          //delay slightly so that when the SPI frame is received the D/C pin is still set correctly!
}
