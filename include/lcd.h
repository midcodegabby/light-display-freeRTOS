/*
Author: Gabriel Rodgers
Date: 12/27/2024
*/

#ifndef LCD_H_
#define LCD_H_

#define LCD_Y_COUNT 6     //# of vertical pixels, grouped in bytes
#define LCD_X_COUNT 84    //# of horizontal pixels

/*---------------------------LCD COMMANDS-------------------------*/ 
#define SET_Y_ADDRESS(y) ((0x40)|(y))   // 0 <= y <= 5
#define SET_X_ADDRESS(x) ((0x80)|(x))   // 0 <= x <= 83

//next 2 commands require EXTENDED instruction set
#define BIAS(b) ((0x10)|(b))            // 0 <= b <= 7  
#define VLCD(vop) ((0x80)|(vop))        // 0 <= vop <= 127

//typedef enum for all 'magic number' LCD commands (aside from x/y addressing and biasing commands)
typedef enum {
    FUNCTION_ON_X_BASIC = 0x20,
    FUNCTION_ON_X_EXTENDED = 0x21,
    FUNCTION_ON_Y_BASIC = 0x22,
    FUNCTION_ON_Y_EXTENDED = 0x23,
    FUNCTION_OFF = 0x24,

    DISPLAY_BLANK = 0x08,
    DISPLAY_NORMAL = 0x0C,
    DISPLAY_ALL = 0x09,
    DISPLAY_INVERSE = 0x0D,

    //next 4 commands require EXTENDED instruction set 
    TEMPERATURE_COEFF_LOW = 0x04,
    TEMPERATURE_COEFF_MED_LOW = 0x05,
    TEMPERATURE_COEFF_MED_HIGH = 0x06,
    TEMPERATURE_COEFF_HIGH = 0x07,
}lcd_command_t;
/*---------------------------------------------------------------*/ 

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
void lcd_all_pixels(void);
void lcd_x_scroll(void);
void lcd_y_scroll(void);
void lcd_clear(void);
void lcd_fill(uint8_t c);
void lcd_output_text(lcd_text_buffer_t const buf);
void lcd_output_pixels(lcd_pixel_buffer_t const buf);

#endif /* LCD_H_ */