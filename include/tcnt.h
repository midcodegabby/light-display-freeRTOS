/*
Author: Gabriel Rodgers
Date: 12/27/2024
*/

#ifndef TCNT_H
#define TCNT_H

#define TIM2 0x40000000
#define TIM3 0x40000400

#define TIM2_CR1 (*((volatile uint32_t *) (TIM2)))
#define TIM2_DIER (*((volatile uint32_t *) (TIM2 + 0x0C)))
#define TIM2_SR (*((volatile uint32_t *) (TIM2 + 0x10)))
#define TIM2_EGR (*((volatile uint32_t *) (TIM2 + 0x14)))
#define TIM2_CCMR1 (*((volatile uint32_t *) (TIM2 + 0x18)))
#define TIM2_CCER (*((volatile uint32_t *) (TIM2 + 0x20)))
#define TIM2_CNT (*((volatile uint32_t *) (TIM2 + 0x24)))
#define TIM2_PSC (*((volatile uint32_t *) (TIM2 + 0x28)))
#define TIM2_ARR (*((volatile uint32_t *) (TIM2 + 0x2C)))
#define TIM2_CCR1 (*((volatile uint32_t *) (TIM2 + 0x34)))

#define TIM3_CR1 (*((volatile uint32_t *) (TIM3)))
#define TIM3_DIER (*((volatile uint32_t *) (TIM3 + 0x0C)))
#define TIM3_SR (*((volatile uint32_t *) (TIM3 + 0x10)))
#define TIM3_EGR (*((volatile uint32_t *) (TIM3 + 0x14)))
#define TIM3_CCMR1 (*((volatile uint32_t *) (TIM3 + 0x18)))
#define TIM3_CCER (*((volatile uint32_t *) (TIM3 + 0x20)))
#define TIM3_CNT (*((volatile uint32_t *) (TIM3 + 0x24)))
#define TIM3_PSC (*((volatile uint32_t *) (TIM3 + 0x28)))
#define TIM3_ARR (*((volatile uint32_t *) (TIM3 + 0x2C)))
#define TIM3_CCR1 (*((volatile uint32_t *) (TIM3 + 0x34)))

void timer3_basic_init(void);
void timer3_delay_us(uint16_t us);
void timer2_basic_init(void);
void timer2_pwm_init(void);
void timer2_pwm_set(uint8_t level);
void timer2_blocking_delay(uint32_t cycles);

#endif /* TCNT_H */