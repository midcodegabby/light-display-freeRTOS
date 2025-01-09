/*
Author: Gabriel Rodgers
Date: 10/13/2024
Purpose: to enable interrupts and handle some IRQs
*/

#include <stdint.h>

#include "exti.h"
#include "clock.h"
#include "nvic.h"
#include "gpio.h"
#include "lcd.h"

//flag for button press
volatile uint8_t g_button_flag;

//enable interrupts
void exti_init(void) {
        EXTI_FTSR1 |= (1 << 13);        //enable falling edge trigger for line 13
        EXTI_RTSR1 &= ~(1 << 13);       //disable rising edge trigger for line 13        
        SYSCFG_EXTICR4 |= (1 << 5);     //choose PC13 to be the event trigger for EXTI13
        EXTI_IMR1 |= (1 << 13);         //enable interrupts on line 13
}

void exti_disable(void) {
        EXTI_IMR1 &= ~(1 << 13);        //disable interrupts for line 13, corresponding to the user button
}

//IRQ handler for button push interrupt
void EXTI15_10_IRQHandler(void) {
        g_button_flag = 1;
        EXTI_PR1 |= (1 << 13);  //clear pending interrupts on this line
}

