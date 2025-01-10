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
#include "tcnt.h"

//semaphore to synchronize dma to task actions.
extern SemaphoreHandle_t p_dma_binary_semaphore;

void dma1_ch5_init(void) {
    




}

void DMA1_Stream5_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;  //init a var to hold whether a higher priority task is being blocked by the button binary semaphore

    if (1) {    //if data done receiving, then give semaphore!
        xSemaphoreGiveFromISR(p_dma_binary_semaphore, &xHigherPriorityTaskWoken); //give button_binary_semaphore away

        //STORE DATA HERE?
    }

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);   //force context switch to higher priority blocked task (if there is one)
}
