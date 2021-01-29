/* gpio.h */
#ifndef SPI_H
#define SPI_H

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>

//Tx SPI param: adress, data
void SPI_Tx(uint8_t adress, uint8_t data);

//Rx SPI param: adress, return data
uint8_t SPI_Rx(uint8_t adress);
void spi_init(void);

#endif
