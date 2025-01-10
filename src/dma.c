/*
Author: Gabriel Rodgers
Date: 1/9/2025
Purpose: Configure DMA for I2C2 Reception.
*/

#include <stdint.h>

#include "dma.h"
#include "i2c.h"
#include "tcnt.h"

void dma1_ch5_init(void);
void DMA1_Stream5_IRQHandler(void);
