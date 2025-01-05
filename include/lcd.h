/*
Author: Gabriel Rodgers
Date: 12/27/2024
*/

#ifndef LCD_H_
#define LCD_H_

#define BITMAP_WIDTH 5

#define LCD_Y_COUNT 6     //# of vertical pixels, grouped in bytes
#define LCD_X_COUNT 84    //# of horizontal pixels

/*---------------------------LCD COMMANDS-------------------------*/ 
#define LCD_SET_Y_ADDRESS(y) ((0x40)|(y))   // 0 <= y <= 5
#define LCD_SET_X_ADDRESS(x) ((0x80)|(x))   // 0 <= x <= 83

#define LCD_FUNCTION_ON_X_BASIC (0x20)
#define LCD_FUNCTION_ON_X_EXTENDED (0x21)
#define LCD_FUNCTION_ON_Y_BASIC (0x22)
#define LCD_FUNCTION_ON_Y_EXTENDED (0x23)
#define LCD_FUNCTION_OFF (0x24)

#define LCD_DISPLAY_BLANK (0x08)
#define LCD_DISPLAY_NORMAL (0x0C)
#define LCD_DISPLAY_ALL (0x09)
#define LCD_DISPLAY_INVERSE (0x0D)

//next 6 commands require EXTENDED instruction set
#define LCD_BIAS(b) ((0x10)|(b))            // 0 <= b <= 7  
#define LCD_VOP(v) ((0x80)|(v))        // 0 <= v <= 127
#define LCD_TEMPERATURE_COEFF_LOW (0x04)
#define LCD_TEMPERATURE_COEFF_MED_LOW (0x05)
#define LCD_TEMPERATURE_COEFF_MED_HIGH (0x06)
#define LCD_TEMPERATURE_COEFF_HIGH (0x07)
/*---------------------------------------------------------------*/ 

typedef uint8_t lcd_pixel_buffer_t[LCD_Y_COUNT][LCD_X_COUNT];
typedef char* lcd_text_buffer_t[LCD_Y_COUNT];

void lcd_init(void);
void lcd_on(void);
void lcd_off(void);
void lcd_backlight_on(void);
void lcd_backlight_off(void);
void lcd_backlight_set(uint8_t brightness);
void lcd_all_pixels(void);
void lcd_x_scroll(void);
void lcd_y_scroll(void);
void lcd_clear(void);
void lcd_output_text(lcd_text_buffer_t const buf);
void lcd_output_pixels(lcd_pixel_buffer_t const buf);

#endif /* LCD_H_ */