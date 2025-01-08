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

//define bitmap representation of characters (5x8 pixels each)
static const uint8_t ascii_bitmap[][BITMAP_WIDTH] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00 },  // 20  32
    { 0x00, 0x00, 0x9E, 0x00, 0x00 },  // 21  33  !
	{ 0x00, 0x0E, 0x00, 0x0E, 0x00 },  // 22  34  "
	{ 0x28, 0xFE, 0x28, 0xFE, 0x28 },  // 23  35  #
	{ 0x48, 0x54, 0xFE, 0x54, 0x24 },  // 24  36  $
	{ 0x46, 0x26, 0x10, 0xC8, 0xC4 },  // 25  37  %
	{ 0x6C, 0x92, 0xAA, 0x44, 0xA0 },  // 26  38  &
	{ 0x00, 0x0A, 0x06, 0x00, 0x00 },  // 27  39  '
	{ 0x00, 0x38, 0x44, 0x82, 0x00 },  // 28  40  (
	{ 0x00, 0x82, 0x44, 0x38, 0x00 },  // 29  41  )
	{ 0x28, 0x10, 0x7C, 0x10, 0x28 },  // 2A  42  *
	{ 0x10, 0x10, 0x7C, 0x10, 0x10 },  // 2B  43  +
	{ 0x00, 0xA0, 0x60, 0x00, 0x00 },  // 2C  44  ,
	{ 0x10, 0x10, 0x10, 0x10, 0x10 },  // 2D  45  -
	{ 0x00, 0xC0, 0xC0, 0x00, 0x00 },  // 2E  46  .
	{ 0x40, 0x20, 0x10, 0x08, 0x04 },  // 2F  47  /
	{ 0x7C, 0xA2, 0x92, 0x8A, 0x7C },  // 30  48  0
	{ 0x00, 0x84, 0xFE, 0x80, 0x00 },  // 31  49  1
	{ 0x84, 0xC2, 0xA2, 0x92, 0x8C },  // 32  50  2
	{ 0x42, 0x82, 0x8A, 0x96, 0x62 },  // 33  51  3
	{ 0x30, 0x28, 0x24, 0xFE, 0x20 },  // 34  52  4
	{ 0x4E, 0x8A, 0x8A, 0x8A, 0x72 },  // 35  53  5
	{ 0x78, 0x94, 0x92, 0x92, 0x60 },  // 36  54  6
	{ 0x06, 0x02, 0xE2, 0x12, 0x0E },  // 37  55  7
	{ 0x6C, 0x92, 0x92, 0x92, 0x6C },  // 38  56  8
	{ 0x0C, 0x92, 0x92, 0x52, 0x3C },  // 39  57  9
	{ 0x00, 0x6C, 0x6C, 0x00, 0x00 },  // 3A  58  :
	{ 0x00, 0xAC, 0x6C, 0x00, 0x00 },  // 3B  59  ;
	{ 0x10, 0x28, 0x44, 0x82, 0x00 },  // 3C  60  <
	{ 0x28, 0x28, 0x28, 0x28, 0x28 },  // 3D  61  =
	{ 0x00, 0x82, 0x44, 0x28, 0x10 },  // 3E  62  >
	{ 0x04, 0x02, 0xA2, 0x12, 0x0C },  // 3F  63  ?
	{ 0x64, 0x92, 0xF2, 0x82, 0x7C },  // 40  64  @
	{ 0xF8, 0x24, 0x22, 0x24, 0xF8 },  // 41  65  A
	{ 0xFE, 0x92, 0x92, 0x92, 0x6C },  // 42  66  B
	{ 0x7C, 0x82, 0x82, 0x82, 0x44 },  // 43  67  C
	{ 0xFE, 0x82, 0x82, 0x44, 0x38 },  // 44  68  D
	{ 0xFE, 0x92, 0x92, 0x92, 0x82 },  // 45  69  E
	{ 0xFE, 0x12, 0x12, 0x12, 0x02 },  // 46  70  F
	{ 0x7C, 0x82, 0x92, 0x92, 0xF4 },  // 47  71  G
	{ 0xFE, 0x10, 0x10, 0x10, 0xFE },  // 48  72  H
	{ 0x00, 0x82, 0xFE, 0x82, 0x00 },  // 49  73  I
	{ 0x40, 0x80, 0x82, 0x7E, 0x02 },  // 4A  74  J
	{ 0xFE, 0x10, 0x28, 0x44, 0x82 },  // 4B  75  K
	{ 0xFE, 0x80, 0x80, 0x80, 0x80 },  // 4C  76  L
	{ 0xFE, 0x04, 0x18, 0x04, 0xFE },  // 4D  77  M
	{ 0xFE, 0x08, 0x10, 0x20, 0xFE },  // 4E  78  N
	{ 0x7C, 0x82, 0x82, 0x82, 0x7C },  // 4F  79  O
	{ 0xFE, 0x12, 0x12, 0x12, 0x0C },  // 50  80  P
	{ 0x7C, 0x82, 0xA2, 0x42, 0xBC },  // 51  81  Q
	{ 0xFE, 0x12, 0x32, 0x52, 0x8C },  // 52  82  R
	{ 0x4C, 0x92, 0x92, 0x92, 0x64 },  // 53  83  S
	{ 0x02, 0x02, 0xFE, 0x02, 0x02 },  // 54  84  T
	{ 0x7E, 0x80, 0x80, 0x80, 0x7E },  // 55  85  U
	{ 0x3E, 0x40, 0x80, 0x40, 0x3E },  // 56  86  V
	{ 0x7E, 0x80, 0x70, 0x80, 0x7E },  // 57  87  W
	{ 0xC6, 0x28, 0x10, 0x28, 0xC6 },  // 58  88  X
	{ 0x0E, 0x10, 0xE0, 0x10, 0x0E },  // 59  89  Y
	{ 0xC2, 0xA2, 0x92, 0x8A, 0x86 },  // 5A  90  Z
	{ 0x00, 0xFE, 0x82, 0x82, 0x00 },  // 5B  91  [
	{ 0x04, 0x08, 0x10, 0x20, 0x40 },  // 5C  92  '\'
	{ 0x00, 0x82, 0x82, 0xFE, 0x00 },  // 5D  93  ]
	{ 0x08, 0x04, 0x02, 0x04, 0x08 },  // 5E  94  ^
	{ 0x80, 0x80, 0x80, 0x80, 0x80 },  // 5F  95  _
	{ 0x00, 0x02, 0x04, 0x08, 0x00 },  // 60  96  `
	{ 0x40, 0xA8, 0xA8, 0xA8, 0xF0 },  // 61  97  a
	{ 0xFE, 0x90, 0x88, 0x88, 0x70 },  // 62  98  b
	{ 0x70, 0x88, 0x88, 0x88, 0x40 },  // 63  99  c
	{ 0x70, 0x88, 0x88, 0x90, 0xFE },  // 64 100  d
	{ 0x70, 0xA8, 0xA8, 0xA8, 0x30 },  // 65 101  e
	{ 0x10, 0xFC, 0x12, 0x02, 0x04 },  // 66 102  f
	{ 0x10, 0xA8, 0xA8, 0xA8, 0x78 },  // 67 103  g
	{ 0xFE, 0x10, 0x08, 0x08, 0xF0 },  // 68 104  h
	{ 0x00, 0x90, 0xFA, 0x80, 0x00 },  // 69 105  i
	{ 0x40, 0x80, 0x88, 0x7A, 0x00 },  // 6A 106  j
	{ 0xFE, 0x20, 0x50, 0x88, 0x00 },  // 6B 107  k
	{ 0x00, 0x82, 0xFE, 0x80, 0x00 },  // 6C 108  l
	{ 0xF8, 0x08, 0xF0, 0x08, 0xF0 },  // 6D 109  m
	{ 0xF8, 0x10, 0x08, 0x08, 0xF0 },  // 6E 110  n
	{ 0x70, 0x88, 0x88, 0x88, 0x70 },  // 6F 111  o
	{ 0xF8, 0x28, 0x28, 0x28, 0x10 },  // 70 112  p
	{ 0x10, 0x28, 0x28, 0x30, 0xF8 },  // 71 113  q
	{ 0xF8, 0x10, 0x08, 0x08, 0x10 },  // 72 114  r
	{ 0x90, 0xA8, 0xA8, 0xA8, 0x40 },  // 73 115  s
	{ 0x08, 0x7E, 0x88, 0x80, 0x40 },  // 74 116  t
	{ 0x78, 0x80, 0x80, 0x40, 0xF8 },  // 75 117  u
	{ 0x38, 0x40, 0x80, 0x40, 0x38 },  // 76 118  v
	{ 0x78, 0x80, 0x60, 0x80, 0x78 },  // 77 119  w
	{ 0x88, 0x50, 0x20, 0x50, 0x88 },  // 78 120  x
	{ 0x18, 0xA0, 0xA0, 0xA0, 0x78 },  // 79 121  y
	{ 0x88, 0xC8, 0xA8, 0x98, 0x88 },  // 7A 122  z
	{ 0x00, 0x10, 0x6C, 0x82, 0x00 },  // 7B 123  {
	{ 0x00, 0x00, 0xFE, 0x00, 0x00 },  // 7C 124  |
	{ 0x00, 0x82, 0x6C, 0x10, 0x00 },  // 7D 125  }
	{ 0x20, 0x10, 0x10, 0x20, 0x10 }  // 7E 126  ~
};

//function to return a pointer to a bitmap array
static const uint8_t* ascii_to_bitmap(char c) {
    if ((c < ' ') || (c > '~')){    //if char not supported return space
        return ascii_bitmap[0]; 
    }
    
    else {
        return ascii_bitmap[c - ' '];   //if char supported return it
    }
}

static void lcd_command(void){
    gpio_lcd_dc(0);
}

static void lcd_data(void){
    gpio_lcd_dc(1);
}

static void lcd_reset(void){
    gpio_lcd_rst(0);    //set RST pin low
    timer3_delay_us(10);
    gpio_lcd_rst(1);    //set RST pin high
}

void lcd_init(void){
    spi2_init();
    gpio_lcd_init();    //init RST and D/C pins for LCD

    gpio_timer2_ch1_init();     //init timer2 ch1 for LCD
    timer2_pwm_init();
    lcd_backlight_on();
    
    lcd_reset();
    lcd_on();

    //lcd_all_pixels();     //for testing
}

void lcd_on(void){
    lcd_command();

    spi2_write(LCD_FUNCTION_ON_X_EXTENDED); //select extended instruction set

    spi2_write(LCD_TEMPERATURE_COEFF_LOW);
    spi2_write(LCD_VOP(54));   
    spi2_write(LCD_BIAS(4));
    
    spi2_write(LCD_FUNCTION_ON_X_BASIC);    //select basic instruction set

    spi2_write(LCD_DISPLAY_NORMAL);

    lcd_clear();
}

void lcd_off(void){
    lcd_command();
    lcd_clear();       //fill RAM with 0s before powering off
    spi2_write(LCD_FUNCTION_OFF);
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

void lcd_all_pixels(void){
    lcd_command();
    spi2_write(LCD_DISPLAY_ALL);
}

void lcd_clear_pixels(void){
    lcd_command();
    spi2_write(LCD_DISPLAY_BLANK);
}

void lcd_x_scroll(void){

}

void lcd_y_scroll(void){

}

//clear the RAM of the LCD.
void lcd_clear(void){
    lcd_data();
    for (uint16_t y = 0; y < LCD_Y_COUNT*LCD_X_COUNT; y++) {
        spi2_write(0x00);
    }
}

void lcd_output_text(lcd_text_buffer_t const buf){
    char c;
    for (uint8_t y = 0; y < LCD_Y_COUNT; y++) {     //loop for every byte row
        lcd_command();
        spi2_write(LCD_SET_Y_ADDRESS(y));   //set y address to equal the index of the array of the current string

        for (uint8_t x = 0; buf[y][x] != '\0'; x++){  //loop for every bit column until null terminator is hit
            
            c = buf[y][x];
            const uint8_t* ascii_array = ascii_to_bitmap(c);    //convert each char to an array of bytes representing pixels

            for (uint8_t column = 0; column < 6; column++){
                if (column < BITMAP_WIDTH) {
                    lcd_data();
                    spi2_write(ascii_array[column]);
                }
                else {  //this section adds a space between each character
                    spi2_write(0x00);
                }
            }
        }
    }
}

/*
void lcd_output_pixels(lcd_pixel_buffer_t const buf){
}
*/




