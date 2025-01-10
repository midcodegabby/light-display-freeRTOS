/*
Author: Gabriel Rodgers
Date: 10/13/2024
*/

#ifndef GPIO_H
#define GPIO_H

#define GPIOA 0x48000000
#define GPIOB 0x48000400
#define GPIOC 0x48000800

#define GPIOA_MODER (*((volatile uint32_t *) GPIOA))
#define GPIOA_OTYPER (*((volatile uint32_t *) (GPIOA + 0x04)))
#define GPIOA_OSPEEDR (*((volatile uint32_t *) (GPIOA + 0x8)))
#define GPIOA_PUPDR (*((volatile uint32_t *) (GPIOA + 0xC)))
#define GPIOA_ODR (*((volatile uint32_t *) (GPIOA + 0x14)))
#define GPIOA_BSRR (*((volatile uint32_t *) (GPIOA + 0x18)))
#define GPIOA_AFRL (*((volatile uint32_t *) (GPIOA + 0x20)))
#define GPIOA_AFRH (*((volatile uint32_t *) (GPIOA + 0x24)))

#define GPIOB_MODER (*((volatile uint32_t *) GPIOB))
#define GPIOB_OTYPER (*((volatile uint32_t *) (GPIOB + 0x4)))
#define GPIOB_OSPEEDR (*((volatile uint32_t *) (GPIOB + 0x8)))
#define GPIOB_PUPDR (*((volatile uint32_t *) (GPIOB + 0xC)))
#define GPIOB_IDR (*((volatile uint32_t *) (GPIOB + 0x10)))
#define GPIOB_BSRR (*((volatile uint32_t *) (GPIOB + 0x18)))
#define GPIOB_AFRL (*((volatile uint32_t *) (GPIOB + 0x20)))
#define GPIOB_AFRH (*((volatile uint32_t *) (GPIOB + 0x24)))

#define GPIOC_MODER (*((volatile uint32_t *) GPIOC))
#define GPIOC_OTYPER (*((volatile uint32_t *) (GPIOC + 0x4)))
#define GPIOC_OSPEEDR (*((volatile uint32_t *) (GPIOC + 0x8)))
#define GPIOC_PUPDR (*((volatile uint32_t *) (GPIOC + 0x0C)))
#define GPIOC_IDR (*((volatile uint32_t *) (GPIOC + 0x10)))
#define GPIOC_ODR (*((volatile uint32_t *) (GPIOC + 0x14)))
#define GPIOC_BSRR (*((volatile uint32_t *) (GPIOC + 0x18)))
#define GPIOC_AFRL (*((volatile uint32_t *) (GPIOC + 0x20)))
#define GPIOC_AFRH (*((volatile uint32_t *) (GPIOC + 0x24)))

void gpio_spi2_init(void);
void gpio_timer2_ch1_init(void);
void gpio_lcd_init(void);
void gpio_lcd_rst(uint8_t state);
void gpio_lcd_dc(uint8_t state);
void gpio_i2c2_init(void);
void gpio_button_init(void);
void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_toggle(void);

#endif /* GPIO_H */
