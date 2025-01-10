/*
Author: Gabriel Rodgers
Date: 1/9/2025
Purpose: To create an application for a real-time light display system using FreeRTOS.
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"
#include "tcnt.h"
#include "nvic.h"
#include "lcd.h"
#include "exti.h"
#include "i2c.h"
#include "tsl2591_functions.h"
#include "dma.h"

#define DMA1_WRITE_ADDRESS (*((volatile uint32_t *) (0x10000000)))

#define STACK_SIZE (512)
#define NVIC_PriorityGroup_4 (~(1 << 10))

//binary semaphores
SemaphoreHandle_t p_button_binary_semaphore = NULL;
SemaphoreHandle_t p_dma_binary_semaphore = NULL;

//queues
static QueueHandle_t lux_data_queue = NULL;

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
	dma1_ch5_init();

	//init TSL2591 via I2C
	uint8_t ret = i2c2_write(2, TSL2591_INIT_MESSAGE);
	tsl2591_write_settings(again_low, atime_100ms);
}

int main(void) {

	BaseType_t status;

	hardware_init();

	p_button_binary_semaphore = xSemaphoreCreateBinary();		//create binary semaphore for button
	configASSERT(p_button_binary_semaphore != NULL);

	p_dma_binary_semaphore = xSemaphoreCreateBinary();		//create binary semaphore for dma writes
	configASSERT(p_dma_binary_semaphore != NULL);

	lux_data_queue = xQueueCreate(8, (size_t) 4); 		//create queue with 8 indeces, with each index having a max size of 4 bytes
	configASSERT(lux_data_queue != NULL);

	status = xTaskCreate( (TaskFunction_t) task1_handler, "lux_task", STACK_SIZE, NULL, 1, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate( (TaskFunction_t) task2_handler, "lcd_task", STACK_SIZE, NULL, 1, NULL);
	configASSERT(status == pdPASS);

	status = xTaskCreate( (TaskFunction_t) task3_handler, "lcd_task", STACK_SIZE, NULL, 1, NULL);
	configASSERT(status == pdPASS);

	vTaskStartScheduler();

	//if the program gets to here, then that means that the heap was overrun - which is not good.
	while (1) {
	}

	return 0;
}

/*---------------------------TASKS---------------------------*/
/*-----------------------------------------------------------*/
//this task handles I2C comms with light sensor and sending processed data to task2.
void task1_handler(void *args) {
	uint8_t i2c2_stage = I2C2_START_POLL;
	uint32_t lux_data = 0;
	
	const TickType_t ticks_to_wait = pdMS_TO_TICKS(10);	//time to try to grab p_dma_binary_semaphore before quitting

	while(1) {	
		if (i2c2_stage == I2C2_START_POLL) { //write data
			i2c2_stage = i2c2_write(1, TSL2591_DATA_REGISTER);
		}
		else if (i2c2_stage == I2C2_POST_WRITE) { //read data
			i2c2_stage = i2c2_read(4);			
		}
		else if (xSemaphoreTake(p_dma_binary_semaphore, ticks_to_wait) == pdPASS) { //process data but only if the dma interrupt gives the semaphore
			lux_data = rawdata_to_lux(DMA1_WRITE_ADDRESS, again_low, atime_100ms);	//process the data
			xQueueSend(lux_data_queue, &lux_data, portMAX_DELAY);	//send the address of lux_data through the queue, but only if queue is not full
			i2c2_stage = I2C2_START_POLL;
		}
	}
}

//this task handles displaying light measurements to the LCD screen
void task2_handler (void *args) {
	char lux_buf[15];
	lcd_text_buffer_t lux_text_buffer = {lux_buf};
	uint32_t lux_data = 0;
	uint32_t len_prev_buf = 0;

	const TickType_t ticks_to_wait = pdMS_TO_TICKS(10);	//time to try and get data from empty queue before quitting

	while(1) {	
		if (xQueueReceive(lux_data_queue, &lux_data, ticks_to_wait) == pdPASS) { //wait for queue to have data in it
			snprintf(lux_buf, 15, "%lu LUX", lux_data);
			
			if (strlen(lux_buf) < len_prev_buf) { //clear the RAM of the previous write, but only if the number of chars written was greater than current
				lcd_clear_n_chars(len_prev_buf);	
			}
			
			lcd_output_text(lux_text_buffer);
			len_prev_buf = strlen(lux_buf);
		}
	}
}

//this task handles misc operations like button presses
void task3_handler (void *args) {
	uint8_t lcd_backlight_brightness = 0xFF;

	const TickType_t ticks_to_wait = pdMS_TO_TICKS(10);

	while(1) {
		if (xSemaphoreTake(p_button_binary_semaphore, ticks_to_wait) == pdPASS) {	//take the semaphore and block if unable to
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

	//do stuff in here to debug
	gpio_led_on();
    while (1);
}
/*-----------------------------------------------------------*/
