/*
Author: Gabriel Rodgers
Date: 8/1/2024
Purpose: To get the LD2 on the Nucleo-L476RG to turn on. 
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "tcnt.h"
#include "nvic.h"
#include "lcd.h"
#include "exti.h"
#include "i2c.h"
#include "tsl2591_functions.h"

#include <FreeRTOS.h>
#include <task.h>

#define STACK_SIZE (512)
#define NVIC_PriorityGroup_4 (~(1 << 10))

extern volatile uint8_t g_button_flag;
extern volatile uint8_t g_i2c2_stage;

volatile uint32_t g_lux_data;
volatile uint32_t g_read_buffer = 0;

//task prototypes
void task1_handler(void *args); //handles comms with lux sensor
void task2_handler(void *args); //handles comms with lcd screen
void task3_handler(void *args); //handles button pushes

//This function initializes all the hardware required for the rtos tasks
static void hardware_init(void) {
	sysclk_init();
	hsi_init();
	peripheral_clk_init();
	nvic_enable();		
	nvic_priority();	//set interrupts to lowest priority.

	//Make sure NVIC uses no subpriorities
	AIRCR |= (VECTKEY);	//use the VECTKEY to gain write access to the AIRCR register
	AIRCR &= (NVIC_PriorityGroup_4); //clear bit 10 in AIRCR, resulting in no subpriorities

	//init peripherals
	exti_init();
	gpio_button_init();
	timer3_basic_init();
	gpio_led_init();
	lcd_init();
	i2c2_init();

	//init TSL2591 via I2C
	i2c2_write(2, TSL2591_INIT_MESSAGE);
	tsl2591_write_settings(again_low, atime_100ms);
/*
	i2c2_write(1, TSL2591_DATA_REGISTER);
	i2c2_read(4);

	while (g_i2c2_stage != I2C2_POST_RECEIVE);

	g_lux_data = rawdata_to_lux(g_read_buffer, again_low, atime_100ms);

	g_i2c2_stage = I2C2_POST_DISPLAY;

	char lux_buf[] = "              ";	//init an empty buffer for lux measurements. -> has size [15] (added null term)
	snprintf(lux_buf, 15, "%lu            ", g_lux_data);

	lux_buf[11] = 'L';
	lux_buf[12] = 'U';
	lux_buf[13] = 'X';

	lcd_text_buffer_t lux_text_buffer = {lux_buf};

	lcd_output_text(lux_text_buffer);
*/
}

int main(void) {

	BaseType_t status;

	hardware_init();

	status = xTaskCreate( (TaskFunction_t) task1_handler, "lux_task", STACK_SIZE, NULL, 2, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate( (TaskFunction_t) task2_handler, "lcd_task", STACK_SIZE, NULL, 2, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate( (TaskFunction_t) task3_handler, "lcd_task", STACK_SIZE, NULL, 2, NULL);
	configASSERT(status == pdPASS);

	vTaskStartScheduler();

	//if the program gets to here, then that means that the heap was overrun - which is not good.
	while (1) {
	}

	return 0;
}

/*---------------------------TASKS---------------------------*/
/*-----------------------------------------------------------*/
void task1_handler(void *args) {
	while(1) {
		gpio_led_off();
		if (g_i2c2_stage == I2C2_POST_DISPLAY) { //write data
			i2c2_write(1, TSL2591_DATA_REGISTER);
		}

		if (g_i2c2_stage == I2C2_POST_WRITE) { //read data
			i2c2_read(4);
		}

		if (g_i2c2_stage == I2C2_POST_RECEIVE){ //process data
			g_lux_data = rawdata_to_lux(g_read_buffer, again_low, atime_100ms);
			g_i2c2_stage = I2C2_POST_DISPLAY;
		}
	}
}

void task2_handler (void *args) {
	//char lux_buf[] = "              ";	//init an empty buffer for lux measurements. -> has size [15] (14 + \0)
	char lux_buf[15];
	lcd_text_buffer_t lux_text_buffer = {lux_buf};

	while(1) {	
		gpio_led_on();
		///*
		g_lux_data = 10000;
		snprintf(lux_buf, 15, "%lu", g_lux_data);

		//lux_buf[11] = 'L';
		//lux_buf[12] = 'U';
		//lux_buf[13] = 'X';
		//lux_buf[2] = '\0';

		lcd_output_text(lux_text_buffer);
		//*/
	}
}

//this task handles misc operations like button presses
void task3_handler (void *args) {
	uint8_t lcd_backlight_brightness = 0xFF;

	while(1) {
		if (g_button_flag == 1) {
			switch(lcd_backlight_brightness) {
				case (0xFF):
					lcd_backlight_brightness = 0x00;
					lcd_backlight_set(lcd_backlight_brightness);
					break;
				default:
					lcd_backlight_brightness += 0xF;
					lcd_backlight_set(lcd_backlight_brightness);
			}
			g_button_flag = 0;
		}
	}
}
/*-----------------------------------------------------------*/

/*----------------------------HOOKS--------------------------*/
/*-----------------------------------------------------------*/
//allows for debugging in the case of stack overflow.
void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

	//do stuff in here to debug
    while(1) {
		gpio_led_on();
	}
    
}
/*-----------------------------------------------------------*/

