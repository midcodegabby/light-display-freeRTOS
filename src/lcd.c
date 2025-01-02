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

lcd_command_t _LCD_COMMAND;

void lcd_init(void){
    spi2_init();
    gpio_lcd_init();    //init RST and D/C pins for LCD

    gpio_timer2_ch1_init();     //init timer2 ch1 for LCD
    timer2_pwm_init();
    
    lcd_reset();
    lcd_on();
    lcd_all_pixels();
    lcd_backlight_on();
}

void lcd_on(void){
    lcd_command();

    _LCD_COMMAND = FUNCTION_ON_X_EXTENDED;
    spi2_write((uint8_t) _LCD_COMMAND);

    //set up optimal settings for LCD display
    _LCD_COMMAND = TEMPERATURE_COEFF_LOW;
    spi2_write((uint8_t) _LCD_COMMAND);
    spi2_write((uint8_t) BIAS(3));
    spi2_write((uint8_t) VLCD(100));
}

void lcd_off(void){
    lcd_command();

    //lcd_fill(0x00);       //fill RAM with 0s before powering off

    _LCD_COMMAND = FUNCTION_OFF;
    spi2_write((uint8_t) _LCD_COMMAND);
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
    gpio_lcd_rst(0);    //set RST pin low
    timer3_delay_us(1);
    gpio_lcd_rst(1);    //set RST pin high
}

void lcd_all_pixels(void){
    lcd_command();

    _LCD_COMMAND = DISPLAY_ALL;
    spi2_write((uint8_t) _LCD_COMMAND);
}

void lcd_x_scroll(void){

}

void lcd_y_scroll(void){

}

void lcd_clear(void){

}

void lcd_output_text(lcd_text_buffer_t const buf){

}

void lcd_output_pixels(lcd_pixel_buffer_t const buf){

}


