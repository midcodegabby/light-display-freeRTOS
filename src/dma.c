/*
Author: Gabriel Rodgers
Date: 1/9/2025
Purpose: Configure DMA for I2C2 Reception.
*/

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "dma.h"
#include "i2c.h"
#include "gpio.h"

//semaphore to synchronize dma to task actions.
extern SemaphoreHandle_t p_dma_binary_semaphore;

void dma1_ch5_init(void) {
    //channel select
    DMA1_CSELR |= (0x3 << 16); //map CH5 to I2C2_RX

    //channel init
    DMA1_CPAR5 = (I2C2 + 0x24); //set peripheral address
    DMA1_CMAR5 = 0x10000000; //set memory address to top of SRAM2 (which is unused)
    DMA1_CNDTR5 = 4; //set number of data transfers to 4 
    DMA1_CCR5 |= (1 << 7)|(1 << 5)|(1 << 1)|(1); //8-bit data transfers, minc, circular mode, read from p, transfer complete interrupt enable, ch enable
}

//occurs after 4 data transfers
void DMA1_Stream5_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;  //init a var to hold whether a higher priority task is being blocked by the button binary semaphore

    if (((DMA1_ISR >> 17) & 1) == 1) {    //if data done transferring, then give semaphore
        xSemaphoreGiveFromISR(p_dma_binary_semaphore, &xHigherPriorityTaskWoken); //give dma_binary_semaphore away
        DMA1_IFCR |= (1 << 17); //clear transfer complete flag
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   //force context switch to higher priority blocked task (if there is one)
}
