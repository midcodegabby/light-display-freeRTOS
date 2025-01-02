/*
Author: Gabriel Rodgers
Date: 12/30/2024
Purpose: Configure and control NOKIA 5110 LCD screen.
*/

#include <stdint.h>

#include "lcd.h"
#include "spi.h"
#include "tcnt.h"
#include "gpio.h"

void lcd_init(void){
    spi2_init();

    gpio_timer2_ch1_init();     //init timer2 ch1 for LCD
    timer2_pwm_init();
	timer2_enable();
	
    gpio_lcd_init();    //init RST and D/C pins for LCD

    lcd_reset();



}

void lcd_on(void){
    lcd_command();
}

void lcd_off(void){
    lcd_command();
}

void lcd_command(void){
    gpio_lcd_dc(0);
}

void lcd_data(void){
    gpio_lcd_dc(1);
}

void lcd_backlight_on(void){
    timer2_pwm_set(0xFF);
}

void lcd_backlight_off(void){
    timer2_pwm_set(0x00);
}

void lcd_backlight_set(uint8_t brightness){
    timer2_pwm_set(brightness);
}

void lcd_reset(void){
    gpio_lcd_rst(0);    //set RST pin low (for at least 0.1us)
    for (volatile uint8_t i = 0; i < 8; i++);
    gpio_lcd_rst(1);    //set RST pin high
}



void lcd_x_scroll(void);
void lcd_y_scroll(void);
void lcd_clear(void);
void lcd_fill(uint8_t c);
void lcd_output_text(lcd_text_buffer_t const buf);
void lcd_output_pixels(lcd_pixel_buffer_t const buf);


