/*
Author: Gabriel Rodgers
Date: 12/30/2024
*/

#ifndef I2C_H_
#define I2C_H_

void i2c2_init(void);
void i2c2_write(uint8_t NBYTES, uint32_t *w_buffer);
void i2c2_write_read(uint8_t NBYTES, uint32_t *target_reg, uint32_t *r_buffer);
uint8_t i2c2_check_bus(volatile int count);
void i2c2_resolve_deadlock(void);
void I2C2_EV_IRQHandler(void);

#endif /* I2C_H_ */
