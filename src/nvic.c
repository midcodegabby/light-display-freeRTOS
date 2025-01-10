/*
Author: Gabriel Rodgers
Date: 10/13/2024
Purpose: To control interrupts/priorities through the NVIC
*/

#include <stdint.h>
#include "nvic.h"

//enable NVIC
void nvic_enable(void) {
        NVIC_ISER1 |= (1 << 8); //enable interrupt 40 (user button)
        NVIC_ISER1 |= (1 << 1); //enable interrupt 33 (I2C2_EV)
        //NVIC_ISER0 |= (1 << 28); //enable interrupt 28 (TIM2 global interrupt)
        NVIC_ISER0 |= (1 << 15); //enable interrupt 15 (DMA1 channel 5)
}

//disable NVIC
void nvic_disable(void) {
        NVIC_ICER1 |= (1 << 8); //disable interrupt 40 (user button)
        NVIC_ICER1 |= (1 << 1); //disable interrupt 33 (I2C2_EV)
        //NVIC_ICER0 |= (1 << 28); //disable interrupt 28 (TIM2 global interrupt)
        NVIC_ICER0 |= (1 << 15); //enable interrupt 15 (DMA1 channel 5)
}

//change priorities of nvic
void nvic_priority(void) {
	NVIC_IPR10 |= (0xF << 4); //change priority of button interrupt (40) to lowest
	NVIC_IPR8 |= (0xE << 12); //change priority of I2C2 receive interrupt (33) to second lowest
	//NVIC_IPR7 |= (0xF << 4); //change priority of TIM2 global interrupt (28) to lowest
        NVIC_IPR3 |= (0xD << 28);  //change priority of DMA1 CH5 interrupt (15) to 3rd lowest
}

