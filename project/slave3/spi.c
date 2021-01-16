/**
  ******************************************************************************
  * 
  * @file    spi.c
  * @author  K. Abbassa, T. Kovacevic, D. Perkovic, A. Sedmak
  * @version V1.0.1
  * @date    January 2018
  * @brief   SPI initialisation and Rx/Tx functions
	
  ******************************************************************************
  */

#include "spi.h"
#include <stm32f4xx_spi.h>
#include "stm32f4xx.h" 

void spi_init() {
	SPI_InitTypeDef SPI_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	SPI_InitStruct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2; // lowest value = max spi speed
	SPI_InitStruct.SPI_Direction=SPI_Direction_2Lines_FullDuplex; // 
	SPI_InitStruct.SPI_Mode=SPI_Mode_Master; // jer je stm master
	SPI_InitStruct.SPI_DataSize=SPI_DataSize_8b; // ocekujemo uint_8
	SPI_InitStruct.SPI_FirstBit=SPI_FirstBit_MSB; 
	SPI_InitStruct.SPI_CPOL=SPI_CPOL_High; // Clock POlarity -
	SPI_InitStruct.SPI_CPHA=SPI_CPHA_2Edge; // Clock PHase
	SPI_InitStruct.SPI_NSS=SPI_NSS_Soft|SPI_NSSInternalSoft_Set; // Slave select
	SPI_Init(SPI1,&SPI_InitStruct);
}

//Tx SPI param: adress, data
void SPI_Tx(uint8_t adress, uint8_t data)
{
	// posalji adresu
  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)); // dok je nesto drugo u tijeku, cekaj
  SPI_I2S_SendData(SPI1,adress);
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));// sve dok receive buffer nije prazan - jos uvijek se prima
  SPI_I2S_ReceiveData(SPI1); 
	
	// posalji data
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1,data);
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
	// chip select akcelerometar
  GPIO_SetBits(GPIOE,GPIO_Pin_3);
  
}

//Rx SPI param: adress, return data
 uint8_t SPI_Rx(uint8_t adress)
{
  GPIO_ResetBits(GPIOE,GPIO_Pin_3);
	
	// the first bit must be 0 for transmission, 1 for reception
  adress=0x80|adress;
	
	// set address
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1,adress);
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
  SPI_I2S_ReceiveData(SPI1);
	
	// set data - dummy data
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
  SPI_I2S_SendData(SPI1,0x00); // dummy data
  while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE));
  GPIO_SetBits(GPIOE,GPIO_Pin_3); // chip select akcelerometer
  return SPI_I2S_ReceiveData(SPI1); //kreni
}
