/*
Author: Gabriel Rodgers
Date: 12/30/2024
Purpose: configure I2C2 and implement read and write functions
*/

//include files
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "i2c.h"
#include "gpio.h"
#include "nvic.h"
#include "tcnt.h"

//initialize I2C2 registers: 100KHz SCL Frequency, 7-bit addressing mode
void i2c2_init(void) {
	gpio_i2c2_init(); 	//set up gpio for i2c2

	I2C2_CR1 &= ~1; //disable I2C2 Peripheral
	I2C2_CR1 |= (1 << 4); //enable NACK interrupt
	
	I2C2_TIMINGR |= I2C2_TIMING_VALS;
	
	I2C2_CR2 |= (TSL2591_ADDRESS << 1); //set target address
	I2C2_CR1 |= 1; //enable I2C2 Peripheral
}

//write to the target; this function only allows up to 2 bytes of data transmission at once
void i2c2_write(uint8_t NBYTES, const uint16_t w_buffer) {
	I2C2_CR2 &= ~(1 << 10); //set to Write
	I2C2_CR2 &= ~(0xFF << 16); //clear NBYTES; if this is not done then sometimes no stop condition
	I2C2_CR2 |= (NBYTES << 16); //NBYTES = NBYTES parameter

	//loop until bus is idle
	volatile int count = 0;
	uint8_t timeout = 0xFF;
	while (timeout != 1) timeout = i2c2_check_bus(); 

	I2C2_CR2 |= (1 << 13); //send start condition

	//loop until all bytes are sent
	for (count = NBYTES-1; count > -1; count--) {
		while (!((I2C2_ISR >> 1) & 1)); //loop until TXIS flag is set
		I2C2_TXDR = (uint8_t)((w_buffer >> (8*count)) & 0xFF); 
	}

	//delay for target device to process 2nd data byte
	timer3_delay_us(500);

	I2C2_CR2 |= (1 << 14); //send STOP condition
	I2C2_ICR |= (1 << 5); //clear stop flag
}

//Write to a register in the target device then read and store 4 bytes to the r_buffer
void i2c2_write_read(uint8_t NBYTES, const uint16_t target_reg, uint32_t *r_buffer) {
	*r_buffer = 0; //clear r_buffer

	//complete write section first:
	I2C2_CR2 &= ~(1 << 10); //set to Write
	I2C2_CR2 &= ~(0xFF << 16); //clear NBYTES; if this is not done then sometimes no stop condition
	I2C2_CR2 |= (1 << 16); //NBYTES = 1
	
	//loop until bus is idle
	volatile int count = 0;
	uint8_t timeout = 0xFF;
	while (timeout != 1) timeout = i2c2_check_bus(); 

	I2C2_CR2 |= (1 << 13); //send start condition

	while (!((I2C2_ISR >> 1) & 1)); //loop until TXIS flag is set
	I2C2_TXDR = target_reg & 0xFF; //only use first byte 

	//repeated START to do read section:
	while ((I2C2_CR2 >> 13) & 1); //loop until START bit is not set

	I2C2_CR2 |= (1 << 10); //set to Read 
	I2C2_CR2 &= ~(0xFF << 16); //clear NBYTES; if this is not done then sometimes no stop condition
	I2C2_CR2 |= (NBYTES << 16); //NBYTES = 4 for the TSL2591 data registers

	//loop until bus is idle
	count = 0;
	timeout = 0xFF;
	while (timeout != 1) timeout = i2c2_check_bus(); 

	I2C2_CR2 |= (1 << 13); //send start condition

	//loop until all bytes are read 
	for (count = 0; count < NBYTES; count++) {
		while (!((I2C2_ISR >> 2) & 1)); //loop until RXNE flag is set
		*r_buffer |= (I2C2_RXDR & 0xFF) << (count*8); 
	}

	//NOTE: NACK is automatically sent after the last byte reception. 
	
	I2C2_CR2 |= (1 << 14); //send STOP condition
	I2C2_ICR |= (1 << 5); //clear stop flag
}	

//Use timers to check if the bus goes idle; returns 1 if idle and 0 if not idle
uint8_t i2c2_check_bus(void) {
	
	I2C2_TIMEOUTR |= (1 << 12); //set TIDLE - detect when SCL and SDA are high for a set period
	I2C2_TIMEOUTR |= (0xC7); //50us max tidle = 0xC7; for 10us do 0x28
	I2C2_TIMEOUTR |= (1 << 15); //enable timeout timer

	timer3_delay_us(50); 	//delay for 50us - or whatever setting you used for tidle
	 
	if ((I2C2_ISR >> 12) & 1) {	
		I2C2_TIMEOUTR &= ~(1 << 15); //disable timout timer
		return 1; //bus went idle
	}

	else {
		I2C2_TIMEOUTR &= ~(1 << 15); //disable timout timer
		return 0; //bus did not go idle
	}
}

//Function to resolve I2C deadlocks - not completed
void i2c2_resolve_deadlock(void) {
}

//IRQ handler for I2C2 event interrupts; for now for NACKs
void I2C2_EV_IRQHandler(void) {
	nvic_disable();
	//do something
	nvic_enable();
}

