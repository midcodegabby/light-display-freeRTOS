/*
Author: Gabriel Rodgers
Date: 1/9/2025
*/

#ifndef DMA_H
#define DMA_H

#define DMA1 0x40020000

#define DMA1_ISR (*((volatile uint32_t *) (DMA1)))
#define DMA1_IFCR (*((volatile uint32_t *) (DMA1 + 0x04)))
#define DMA1_CCR5 (*((volatile uint32_t *) (DMA1 + 0x58)))
#define DMA1_CNDTR5 (*((volatile uint32_t *) (DMA1 + 0x5C)))
#define DMA1_CPAR5 (*((volatile uint32_t *) (DMA1 + 0x60)))
#define DMA1_CMAR5 (*((volatile uint32_t *) (DMA1 + 0x64)))
#define DMA1_CSELR (*((volatile uint32_t *) (DMA1 + 0xA8)))

void dma1_ch5_init();

#endif /* DMA_H */