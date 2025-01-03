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

static lcd_command_t _LCD_COMMAND;

void lcd_init(void){
    spi2_init();
    gpio_lcd_init();    //init RST and D/C pins for LCD

    gpio_timer2_ch1_init();     //init timer2 ch1 for LCD
    timer2_pwm_init();
    
    lcd_reset();
    lcd_on();
    //lcd_all_pixels();
    lcd_clear_pixels();
    lcd_backlight_on();

    //lcd_clear();
}

void lcd_on(void){
    lcd_command();

    _LCD_COMMAND = FUNCTION_ON_X_EXTENDED;
    spi2_write((uint8_t)_LCD_COMMAND);

    //set up optimal settings for LCD display
    _LCD_COMMAND = TEMPERATURE_COEFF_LOW;
    spi2_write((uint8_t)_LCD_COMMAND);
    spi2_write((uint8_t)BIAS(3));
    spi2_write((uint8_t)VLCD(100));
/*
    //set normal mode for LCD display
    _LCD_COMMAND = DISPLAY_INVERSE;
    spi2_write((uint8_t)_LCD_COMMAND);
*/
}

void lcd_off(void){
    lcd_command();

    //lcd_clear();       //fill RAM with 0s before powering off

    _LCD_COMMAND = FUNCTION_OFF;
    spi2_write((uint8_t)_LCD_COMMAND);
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
    timer3_delay_us(5);
    gpio_lcd_rst(1);    //set RST pin high
}

void lcd_all_pixels(void){
    lcd_command();

    _LCD_COMMAND = DISPLAY_ALL;
    spi2_write((uint8_t)_LCD_COMMAND);
}

void lcd_clear_pixels(void){
    lcd_command();

    _LCD_COMMAND = DISPLAY_BLANK;
    spi2_write((uint8_t)_LCD_COMMAND);
}

void lcd_x_scroll(void){

}

void lcd_y_scroll(void){

}

//clear the RAM of the LCD.
void lcd_clear(void){
    lcd_command();
    spi2_write((uint8_t)SET_Y_ADDRESS(0));
    spi2_write((uint8_t)SET_X_ADDRESS(0));

    lcd_data();

    //loop for all byte rows on the LCD 
    for (uint8_t y = 0; y < LCD_Y_COUNT; y++) {

        //loop for all columns on one row on the LCD
        for (uint8_t x = 0; x < LCD_X_COUNT; x++) {
            spi2_write(0x00);
        }
    }
}

void lcd_output_text(lcd_text_buffer_t const buf){

}

void lcd_output_pixels(lcd_pixel_buffer_t const buf){

}

//define bitmap representation of characters
static const uint8_t ascii_bitmap[][BITMAP_WIDTH] = {
{0x00, 0x00, 0x00, 0x00, 0x00}, // '!'
{0x00, 0x00, 0xfa, 0x00, 0x00}, // '"'
{0x00, 0xc0, 0x00, 0xc0, 0x00}, // '#'
{0x28, 0xfe, 0x28, 0xfe, 0x28}, // '$'
{0x24, 0x54, 0xfe, 0x54, 0x48}, // '%'
{0xc4, 0xc8, 0x10, 0x26, 0x46}, // '&'
{0x6c, 0x92, 0x6a, 0x04, 0x0a}, // '''
{0x00, 0xa0, 0xc0, 0x00, 0x00}, // '('
{0x00, 0x38, 0x44, 0x82, 0x00}, // ')'
{0x00, 0x82, 0x44, 0x38, 0x00}, // '*'
{0x28, 0x10, 0x7c, 0x10, 0x28}, // '+'
{0x10, 0x10, 0x7c, 0x10, 0x10}, // ','
{0x00, 0x0a, 0x0c, 0x00, 0x00}, // '-'
{0x10, 0x10, 0x10, 0x10, 0x10}, // '.'
{0x00, 0x06, 0x06, 0x00, 0x00}, // '/'
{0x04, 0x08, 0x10, 0x20, 0x40}, // '0'
{0x7c, 0x8a, 0x92, 0xa2, 0x7c}, // '1'
{0x00, 0x42, 0xfe, 0x02, 0x00}, // '2'
{0x42, 0x86, 0x8a, 0x92, 0x62}, // '3'
{0x84, 0x82, 0xa2, 0xd2, 0x8c}, // '4'
{0x18, 0x28, 0x48, 0xfe, 0x08}, // '5'
{0xe4, 0xa2, 0xa2, 0xa2, 0x9c}, // '6'
{0x3c, 0x52, 0x92, 0x92, 0x0c}, // '7'
{0x80, 0x8e, 0x90, 0xa0, 0xc0}, // '8'
{0x6c, 0x92, 0x92, 0x92, 0x6c}, // '9'
{0x60, 0x92, 0x92, 0x94, 0x78}, // ':'
{0x00, 0x6c, 0x6c, 0x00, 0x00}, // ';'
{0x00, 0x6a, 0x6c, 0x00, 0x00}, // '<'
{0x10, 0x28, 0x44, 0x82, 0x00}, // '='
{0x28, 0x28, 0x28, 0x28, 0x28}, // '>'
{0x00, 0x82, 0x44, 0x28, 0x10}, // '?'
{0x40, 0x80, 0x8a, 0x90, 0x60}, // '@'
{0x4c, 0x92, 0x9e, 0x82, 0x7c}, // 'A'
{0x7e, 0x88, 0x88, 0x88, 0x7e}, // 'B'
{0xfe, 0x92, 0x92, 0x92, 0x6c}, // 'C'
{0x7c, 0x82, 0x82, 0x82, 0x44}, // 'D'
{0xfe, 0x82, 0x82, 0x82, 0x7c}, // 'E'
{0xfe, 0x92, 0x92, 0x92, 0x82}, // 'F'
{0xfe, 0x90, 0x90, 0x90, 0x80}, // 'G'
{0x7c, 0x82, 0x92, 0x92, 0x5e}, // 'H'
{0xfe, 0x10, 0x10, 0x10, 0xfe}, // 'I'
{0x00, 0x82, 0xfe, 0x82, 0x00}, // 'J'
{0x04, 0x02, 0x82, 0xfc, 0x80}, // 'K'
{0xfe, 0x10, 0x28, 0x44, 0x82}, // 'L'
{0xfe, 0x02, 0x02, 0x02, 0x02}, // 'M'
{0xfe, 0x40, 0x20, 0x40, 0xfe}, // 'N'
{0xfe, 0x40, 0x20, 0x10, 0xfe}, // 'O'
{0x7c, 0x82, 0x82, 0x82, 0x7c}, // 'P'
{0xfe, 0x90, 0x90, 0x90, 0x60}, // 'Q'
{0x7c, 0x82, 0x8a, 0x84, 0x7a}, // 'R'
{0xfe, 0x90, 0x98, 0x94, 0x62}, // 'S'
{0x62, 0x92, 0x92, 0x92, 0x8c}, // 'T'
{0x80, 0x80, 0xfe, 0x80, 0x80}, // 'U'
{0xfc, 0x02, 0x02, 0x02, 0xfc}, // 'V'
{0xf8, 0x04, 0x02, 0x04, 0xf8}, // 'W'
{0xfc, 0x02, 0x1c, 0x02, 0xfc}, // 'X'
{0xc6, 0x28, 0x10, 0x28, 0xc6}, // 'Y'
{0xe0, 0x10, 0x0e, 0x10, 0xe0}, // 'Z'
{0x86, 0x8a, 0x92, 0xa2, 0xc2}, // '['
{0x00, 0xfe, 0x82, 0x82, 0x00}, // '\'
{0x40, 0x20, 0x10, 0x08, 0x04}, // ']'
{0x00, 0x82, 0x82, 0xfe, 0x00}, // '^'
{0x20, 0x40, 0x80, 0x40, 0x20}, // '_'
{0x02, 0x02, 0x02, 0x02, 0x02}, // '`'
{0x00, 0xc0, 0xe0, 0x00, 0x00}, // 'a'
{0x04, 0x2a, 0x2a, 0x2a, 0x1e}, // 'b'
{0xfe, 0x22, 0x22, 0x22, 0x1c}, // 'c'
{0x1c, 0x22, 0x22, 0x22, 0x04}, // 'd'
{0x1c, 0x22, 0x22, 0x22, 0xfe}, // 'e'
{0x1c, 0x2a, 0x2a, 0x2a, 0x18}, // 'f'
{0x10, 0x7e, 0x90, 0x80, 0x40}, // 'g'
{0x10, 0x2a, 0x2a, 0x2a, 0x3c}, // 'h'
{0xfe, 0x20, 0x20, 0x20, 0x1e}, // 'i'
{0x00, 0x22, 0xbe, 0x02, 0x00}, // 'j'
{0x04, 0x02, 0x22, 0xbc, 0x00}, // 'k'
{0xfe, 0x08, 0x14, 0x22, 0x00}, // 'l'
{0x00, 0x82, 0xfe, 0x02, 0x00}, // 'm'
{0x3e, 0x20, 0x18, 0x20, 0x3e}, // 'n'
{0x3e, 0x10, 0x20, 0x20, 0x1e}, // 'o'
{0x1c, 0x22, 0x22, 0x22, 0x1c}, // 'p'
{0x3e, 0x28, 0x28, 0x28, 0x10}, // 'q'
{0x10, 0x28, 0x28, 0x28, 0x3e}, // 'r'
{0x3e, 0x10, 0x20, 0x20, 0x10}, // 's'
{0x12, 0x2a, 0x2a, 0x2a, 0x04}, // 't'
{0x20, 0xfc, 0x22, 0x02, 0x04}, // 'u'
{0x3c, 0x02, 0x02, 0x04, 0x3e}, // 'v'
{0x38, 0x04, 0x02, 0x04, 0x38}, // 'w'
{0x3c, 0x02, 0x0c, 0x02, 0x3c}, // 'x'
{0x22, 0x14, 0x08, 0x14, 0x22}, // 'y'
{0x30, 0x0a, 0x0a, 0x0a, 0x3c}, // 'z'
{0x22, 0x26, 0x2a, 0x32, 0x22}, // '{'
{0x00, 0x10, 0x6c, 0x82, 0x00}, // '|'
{0x00, 0x00, 0xfe, 0x00, 0x00}, // '}'
{0x00, 0x82, 0x6c, 0x10, 0x00}, // '~'
{0x40, 0x80, 0x40, 0x20, 0x40}  // 
};

//function to return a pointer to a bitmap array
static const uint8_t *ascii_to_bitmap(char c) {
    if (c < ' ' || c > '~'){
        return ascii_bitmap[0];
    }
    
    else {
        return ascii_bitmap[c - ' '];
    }
}




