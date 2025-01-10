/*
Author: Gabriel Rodgers
Date: 12/27/2024
Purpose: To control timers/counters
*/

#include <stdint.h>

#include "tcnt.h"
#include "gpio.h"
#include "main.h"

//init timer3 with 1MHz clock, regular timer mode, and interrupts.
void timer3_basic_init(void) {
    TIM3_CR1 |= (1 << 7);   //enable buffering for auto-reload
    TIM3_PSC = 47;  //prescaler for 1MHz timer frequency
    TIM3_ARR = 0xFFFF;  //set as max value
    TIM3_EGR |= (1 << 0); //update registers
    TIM3_CR1 |= (1 << 0); //enable timer/counter
}

//delay function for timer3
void timer3_delay_us(uint16_t us) {
    TIM3_ARR = us;
    TIM3_EGR |= (1 << 0); //update registers
    TIM3_CNT = 0;

    while (TIM3_CNT < us); 
}

//init timer2 with 1MHz clock, regular timer mode, and interrupts.
void timer2_basic_init(void) {
    TIM2_CR1 |= (1 << 7);   //enae ble buffering for auto-reload
    TIM2_PSC = 47;  //prescaler for 1MHz timer frequency
    TIM2_ARR = duration_1s;  
    TIM2_EGR |= (1 << 0); //update registers
    TIM2_CR1 |= (1 << 0); //enable timer/counter
}

//init timer2 with internal clock, pwm mode 1 (output to Channel 1).
void timer2_pwm_init(void) {
    gpio_timer2_ch1_init(); //init the gpio for ch1 of timer2

    TIM2_CR1 |= (1 << 7);   //enable buffering for auto-reload
    TIM2_ARR = 0xFF;  //max value

    TIM2_CCMR1 |= (0x3 << 5); //PWM mode 1 for Channel 1
    TIM2_CCMR1 |= (1 << 3); //preload enabled for TIM2_CCR1
    TIM2_CCER |= (1 << 0); //enable TIM2 Channel 1 output
    TIM2_CCR1 = 0x0;    //pwm set to off.

    TIM2_EGR |= (1 << 0); //update registers
    TIM2_CR1 |= (1 << 0); //enable timer/counter
}

void timer2_pwm_set(uint8_t level) {
    TIM2_CCR1 = level;
}

//blocking counter
void timer2_blocking_delay(uint32_t cycles) {
    TIM2_ARR = cycles;
    TIM2_EGR |= (1 << 0); //update registers
    TIM2_CNT = 0;

    while (TIM2_CNT < cycles); 
}