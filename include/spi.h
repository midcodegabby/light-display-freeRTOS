/*
Author: Gabriel Rodgers
Date: 12/30/2024
*/

#ifndef SPI_H_
#define SPI_H_

#define SPI2 0x40003800

#define SPI2_CR1 (*((volatile uint32_t *) (SPI2)))
#define SPI2_CR2 (*((volatile uint32_t *) (SPI2 + 0x04)))
#define SPI2_SR (*((volatile uint32_t *) (SPI2 + 0x08)))
#define SPI2_DR (*((volatile uint32_t *) (SPI2 + 0x0C)))
#define SPI2_CR1 (*((volatile uint32_t *) (SPI2)))

void spi2_init(void);
void spi2_write(uint8_t data);

#endif /* SPI_H_ */