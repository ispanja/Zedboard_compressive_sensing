#ifndef GPS_H
#define GPS_H

#include <stm32f4xx.h>								// common stuff
#include <stm32f4xx_gpio.h>						// gpio control
#include <stm32f4xx_rcc.h>						// reset and clocking
#include <stm32f4xx_usart.h>					// USART
#include <string.h>
#include <usart.h>
#include <stdlib.h>

#define BAUDRATE_USART6		9600

typedef struct latlon {
	char time[15];
	char latitudeValue[15];
	char latitudeDirection[5];
	char longitudeValue[15];
	char longitudeDirection[5];
} latlon;


void GPS_Init(void);										// init USART1 peripheral
void GPS_SendChar(char c);							// blocking send character
void GPS_SendStr(char *c);							// send string to USART6
int GPS_Dequeue(char* c);								// pop character from receive FIFO
latlon parseGPSdata(char* message);			// parse incoming GPS data


#endif
