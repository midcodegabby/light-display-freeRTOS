/*
Author: Gabriel Rodgers
Date: 12/27/2024
*/

#ifndef LCD_H_
#define LCD_H_

#define control 0x49

#define LCD_Y_COUNT 6     //# of vertical pixels, grouped in bytes
#define LCD_X_COUNT 84    //# of horizontal pixels

typedef uint8_t lcd_pixel_buffer_t[LCD_Y_COUNT][LCD_X_COUNT];
typedef char* lcd_text_buffer_t[LCD_Y_COUNT];

void lcd_init(void);
void lcd_on(void);
void lcd_off(void);
void lcd_command(void);
void lcd_data(void);
void lcd_backlight_on(void);
void lcd_backlight_off(void);
void lcd_backlight_set(uint8_t brightness);
void lcd_reset(void);
void lcd_x_scroll(void);
void lcd_y_scroll(void);
void lcd_clear(void);
void lcd_fill(uint8_t c);
void lcd_output_text(lcd_text_buffer_t const buf);
void lcd_output_pixels(lcd_pixel_buffer_t const buf);

#endif /* LCD_H_ */