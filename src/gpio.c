/*
Author: Gabriel Rodgers
Date: 10/13/2024
Purpose: configure GPIOs for I2C2, button input, and LED2
*/

//include files
#include <stdint.h>
#include "gpio.h"

//init SPI2: SPI2_NSS-PB9; SPI2_SCLK-PB10; SPI2_MOSI-PC3; SPI2_MISO-PC2
void gpio_spi2_init(void) {
	GPIOC_AFRL |= (0x5 << 8)|(0x5 << 12); //AF5 for PC2/PC3
	GPIOB_AFRH |= (0x5 << 4)|(0x5 << 8);  //AF5 for PB9/PB10

	GPIOC_OSPEEDR |= (0x3 << 4)|(0x3 << 6); //very high speed for PC2/PC3
	GPIOB_OSPEEDR |= (0x3 << 18)|(0x3 << 20); //very high speed for PB9/PB10

	GPIOB_PUPDR |= (1 << 18); //pull ups for PB9

	GPIOC_MODER &= ~((1 << 4)|(1 << 6)); //alt function for PC2/PC3
	GPIOB_MODER &= ~((1 << 18)|(1 << 20)); //alt function for PB9/PB10
}

//init timer2 channel1: TIM2_CH1-PA0
void gpio_timer2_ch1_init(void) {
	GPIOA_AFRL |= (1 << 0); //AF1 for PA0
	GPIOA_OSPEEDR |= (0x3 << 0); //very high speed for PA0
	GPIOA_MODER &= ~(1 << 0); //alt function for PA0
}

//init lcd reset and data/command pins
void gpio_lcd_init(void) {
	GPIOC_PUPDR |= (1 << 20)|(1 << 22); //pull ups for PC10/PC11
	GPIOC_MODER &= ~((1 << 21)|(1 << 23)); //general purpose output for PC10/PC11
}

//control lcd reset pin
void gpio_lcd_rst(uint8_t state) {
	if (state == 0){
		GPIOC_ODR &= ~(1 << 11);
	}
	else {
		GPIOC_ODR |= (1 << 11);
	}
}

//control lcd command/data pin - LOW=command, HIGH=data
void gpio_lcd_dc(uint8_t state) {
	if (state == 0){
		GPIOC_ODR &= ~(1 << 10);
	}
	else {
		GPIOC_ODR |= (1 << 10);
	}
}

//init I2C2: I2C2_SCL-PB13; I2C2_SDA-PB14
void gpio_i2c2_init(void) {
	GPIOB_AFRH |= (0x4 << 20); //AF4 for PB13
	GPIOB_AFRH |= (0x4 << 24); //AF4 for PB14

	GPIOB_OSPEEDR |= ((0x3 << 26) | (0x3 << 28)); //set very high speed for PB13/PB14
	GPIOB_PUPDR |= ((1 << 26) | (1 << 28)); //pull ups for PB13/PB14
	GPIOB_OTYPER |= (0x3 << 13); //open drain for PB13/PB14
	
	GPIOB_MODER &= ~(1 << 26); //alt function for PB13 
	GPIOB_MODER &= ~(1 << 28); //alt function for PB14
}

void gpio_button_init(void) {
	//set button pin (B1, connected to PC13) to be general purpose input mode
	GPIOC_MODER &= ~(0x3 << 26);

	//set input to pull up R
	GPIOC_PUPDR &= ~(0x3 << 26);
	GPIOC_PUPDR |= (1 << 26);
}

void gpio_led_init(void) {
	//set LED pin (GPIOA pin 5) to be general purpose output mode
	GPIOA_MODER |= (1 << 10);
	GPIOA_MODER &= ~(1 << 11);

	GPIOA_OTYPER &= ~(1 << 5); // set PA5 to be push-pull
}

void gpio_led_on(void) {
	GPIOA_ODR |= (1 << 5); 
}

void gpio_led_off(void) {
	GPIOA_ODR &= ~(1 << 5); 
}

void gpio_led_toggle(void) {
	GPIOA_ODR ^= (1 << 5); 
}

