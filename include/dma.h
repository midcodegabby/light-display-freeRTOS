/*
Author: Gabriel Rodgers
Date: 1/9/2025
*/

#ifndef DMA_H
#define DMA_H

#define DMA1 0x40003800

#define DMA1_CR1 (*((volatile uint32_t *) (SPI2)))

void dma1_ch5_init();

#endif /* DMA_H */