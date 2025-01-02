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
  
