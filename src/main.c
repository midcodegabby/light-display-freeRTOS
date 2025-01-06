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

#include <FreeRTOS.h>
#include <task.h>

#define STACK_SIZE (128)
#define NVIC_PriorityGroup_4 (~(1 << 10))

extern volatile uint8_t global_button_flag;

//task prototypes
void task1_handler(void *args); //handles comms with lux sensor
void task2_handler(void *args); //handles comms with lcd screen
void task3_handler(void *args); //handles button pushes

//This function initializes all the hardware required for the rtos tasks
static void hardware_init(void) {
	sysclk_init();
	//hsi_init();
	peripheral_clk_init();
	nvic_enable();		
	nvic_priority();	//set interrupts to lowest priority.

	//Make sure NVIC uses no subpriorities
	AIRCR |= (VECTKEY);	//use the VECTKEY to gain write access to the AIRCR register
	AIRCR &= (NVIC_PriorityGroup_4); //clear bit 10 in AIRCR, resulting in no subpriorities

	exti_init();
	gpio_button_init();
	timer3_basic_init();
	gpio_led_init();
	lcd_init();
	
	char buf5[] = "hello micron";
	lcd_text_buffer_t lux_text_buffer = {buf5};

	lcd_output_text(lux_text_buffer);
}

int main(void) {

	BaseType_t status;

	hardware_init();

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
void task1_handler(void *args) {
	while(1) {
		gpio_led_on();
		
	}
}

void task2_handler (void *args) {
	while(1) {
		gpio_led_off();
		
	}
}

void task3_handler (void *args) {
	uint8_t lcd_backlight_brightness = 0xFF;

	while(1) {
		if (global_button_flag == 1) {
			switch(lcd_backlight_brightness) {
				case (0xFF):
					lcd_backlight_brightness = 0x00;
					lcd_backlight_set(lcd_backlight_brightness);
					break;
				default:
					lcd_backlight_brightness += 0xF;
					lcd_backlight_set(lcd_backlight_brightness);
			}
			global_button_flag = 0;
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
    for( ; ; ) {
		gpio_led_on();
		timer2_blocking_delay(duration_1ms*100);
	}
    
}
/*-----------------------------------------------------------*/

