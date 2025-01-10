/*
Author: Gabriel Rodgers
Date: 8/1/2024
Purpose: To get the LD2 on the Nucleo-L476RG to turn on. 
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "tcnt.h"
#include "nvic.h"
#include "lcd.h"
#include "exti.h"
#include "i2c.h"
#include "tsl2591_functions.h"

#define STACK_SIZE (512)
#define NVIC_PriorityGroup_4 (~(1 << 10))

//extern volatile uint8_t g_dma_flag; //REPLACE WITH SEMAPHORE

//binary semaphores
SemaphoreHandle_t p_button_binary_semaphore = NULL;

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
	uint8_t ret = i2c2_write(2, TSL2591_INIT_MESSAGE);
	tsl2591_write_settings(again_low, atime_100ms);
}

int main(void) {

	BaseType_t status;

	hardware_init();

	p_button_binary_semaphore = xSemaphoreCreateBinary();		//create binary semaphore for button
	configASSERT(p_button_binary_semaphore != NULL);

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
	uint8_t i2c2_stage = I2C2_START_POLL;
	uint32_t processed_data = 0;

	//WIP-----------------------------------
	uint32_t raw_data = 0;	//THIS SHOULD POINT TO DMA MEMORY PART
	//START QUEUE
	//USE port_YIELD_FROM_ISR() in dma part!!!
	while(1) {
		gpio_led_off();
		/*
		if (i2c2_stage == I2C2_START_POLL) { //write data
			i2c2_stage = i2c2_write(1, TSL2591_DATA_REGISTER);
		}
		if (i2c2_stage == I2C2_POST_WRITE) { //read data
			i2c2_stage = i2c2_read(4);
		}

		//this part is a WIP-need to do some sort of DMA flag for this - perhaps a mutex.
		if (g_dma_flag == 1) { //process data
			processed_data = rawdata_to_lux(raw_data, again_low, atime_100ms);
			i2c2_stage = I2C2_POST_RECEIVE;

		}
		if (i2c2_stage == I2C2_POST_RECEIVE) {
			//send data via queue to task 2
			g_dma_flag = 0; //RELEASE MUTEX, but only do next step if it suceeds!!!
			i2c2_stage = I2C2_START_POLL;
		}
		*/
	}
}

void task2_handler (void *args) {
	char lux_buf[15];
	lcd_text_buffer_t lux_text_buffer = {lux_buf};
	uint32_t value = 0;

	while(1) {	
		gpio_led_on();
		
		//wait for queue to have data in it
		if (1) {
			value++;
			snprintf(lux_buf, 15, "%lu LUX", value);
			lcd_output_text(lux_text_buffer);
		}
	}
}

//this task handles misc operations like button presses
void task3_handler (void *args) {
	uint8_t lcd_backlight_brightness = 0xFF;

	while(1) {
		if (xSemaphoreTake(p_button_binary_semaphore, portMAX_DELAY) == pdPASS) {	//take the semaphore and block if unable tos
			switch(lcd_backlight_brightness) {
				case (0xFF):
					lcd_backlight_brightness = 0x00;
					lcd_backlight_set(lcd_backlight_brightness);
					break;
				default:
					lcd_backlight_brightness += 0xF;
					lcd_backlight_set(lcd_backlight_brightness);
			}
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
	gpio_led_on();
    while(1);
}
/*-----------------------------------------------------------*/
void vApplicationMallocFailedHook(void) {
    // Handle heap allocation failure

	gpio_led_on();
    while (1); // Halt or reset
}
/*-----------------------------------------------------------*/
