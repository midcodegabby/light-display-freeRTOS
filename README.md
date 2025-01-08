# Light Display FreeRTOS  
### Overview  
A collection of programs, runnable on STM32L476 devices, to measure and display light measurements.    

### Features  
- Built around FreeRTOS, guaranteeing no light measurements are lost or are not displayed.  
- LCD driver uses SPI @ 3MHz, guaranteeing quick display of light measurements to the LCD screen.  
- SPI, I2C, etc. drivers are custom written from scratch in C.  

### Materials   
- Nucleo-L476RG reference board   
- Nokia 5110 LCD screen   
- TSL2591 light sensor   

### Build Guide
```
make all
make install
```

### Status 
- Incomplete  

### Testing
[youtube link](youtube.com)   

### Lessons Learned
- How to create applications that meet timing requirements using FreeRTOS.  
- How to port FreeRTOS to a given MCU without using HAL/CMSIS.  
- How to initialize and control SPI, I2C, and Timer (in counting and PWM modes) peripherals in ARM Cortex-M4 MCUs.
- How to debug hard faults, bus faults, etc. using ARM Core registers and System Control Block registers.
- How to write an LCD driver.  

```
lcd_data();
spi2_write(0xFF);
spi2_write(0x11);
spi2_write(0xFF);

lcd_command();
spi2_write(LCD_SET_Y_ADDRESS(5));
spi2_write(LCD_SET_X_ADDRESS(20));

lcd_data();
spi2_write(0xFF);
spi2_write(0x11);
spi2_write(0xFF);
```
This is the code that I was doing to show oscilloscope d/c commands

lcd_reset();
    lcd_on();

    lcd_all_pixels();
    then

    void lcd_all_pixels(void){
    lcd_command();
    spi2_write(LCD_DISPLAY_ALL);
} 

then 
#define LCD_DISPLAY_ALL (0x09)
