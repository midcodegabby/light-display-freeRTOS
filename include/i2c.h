/*
Author: Gabriel Rodgers
Date: 12/30/2024
*/

#ifndef I2C_H
#define I2C_H

//definitions and register mapping
#define I2C2 0x40005800

//initialization/control registers
#define I2C2_CR1 (*((volatile uint32_t *) (I2C2)))
#define I2C2_CR2 (*((volatile uint32_t *) (I2C2 + 0x04)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMINGR (*((volatile uint32_t *) (I2C2 + 0x10)))
#define I2C2_TIMEOUTR (*((volatile uint32_t *) (I2C2 + 0x14)))
#define I2C2_ISR (*((volatile uint32_t *) (I2C2 + 0x18)))
#define I2C2_ICR (*((volatile uint32_t *) (I2C2 + 0x1C)))

//data registers
#define I2C2_RXDR (*((volatile uint32_t *) (I2C2 + 0x24)))
#define I2C2_TXDR (*((volatile uint32_t *) (I2C2 + 0x28)))

#define TSL2591_ADDRESS (0x29U)
#define TSL2591_DATA_REGISTER (0xB4U) 
#define TSL2591_INIT_MESSAGE ((0xA0 << 8)|(0x03))

#define I2C2_NBYTES (4U)

//timing for 16MHz I2CCLK and 400KHz transmission frequency
#define I2C2_TIMING_VALS ((0x1<<28)|(0x3<<20)|(0x2<<16)|(0x3<<8)|(0x9))

typedef enum{
    I2C2_START_POLL,
    I2C2_POST_WRITE,
    I2C2_POST_READ,
} i2c2_stages_t;

void i2c2_init(void);
uint8_t i2c2_write(const uint8_t NBYTES, const uint16_t w_buffer);
uint8_t i2c2_read(const uint8_t NBYTES);
uint8_t i2c2_check_bus(void);

#endif /* I2C_H */
