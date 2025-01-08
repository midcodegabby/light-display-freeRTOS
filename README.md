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
- How to debug digital signals using oscilloscopes - see images below, along with explanations!

### Oscilloscope Testing
A preface: I decided to observe the signals on the Data/Control pin and the SPI bus (MOSI, SS, and SCK) to better understand how   
my system was working and to be able to debug signal integrity issues in the future. To do this, I had to learn how to use the trigger   
and bus features on Tektronix oscilloscopes. Below are some images and code snippets that explain the debugging process.   

I used this code snippet, placed in lcd_on(), to look at the Data/Control pin's signal.   
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
From this arrangement, I was able to capture the following two events:
###### The rising edge of the Data/Control signal, occurring right after the second `lcd_data()` call:
![oscope-dc-rising-edge](https://github.com/user-attachments/assets/0834f20c-197d-4579-a832-97c71ce9dbd9)




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
