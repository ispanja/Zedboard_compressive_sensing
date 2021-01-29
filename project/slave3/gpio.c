

#include <gpio.h>
void gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// LEDice
	RCC_AHB1PeriphClockCmd(LED3_ORANGE_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED3_ORANGE_PinNumber;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(LED3_ORANGE_GPIOx, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(LED4_GREEN_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED4_GREEN_PinNumber;
	GPIO_Init(LED4_GREEN_GPIOx, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(LED5_RED_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED5_RED_PinNumber;
	GPIO_Init(LED5_RED_GPIOx, &GPIO_InitStruct);
	
	RCC_AHB1PeriphClockCmd(LED6_BLUE_RCC_GPIOx, ENABLE);
	GPIO_InitStruct.GPIO_Pin = LED6_BLUE_PinNumber;
	GPIO_Init(LED6_BLUE_GPIOx, &GPIO_InitStruct);
	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; // SPI clock, miso mosi
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF; // alternate function mode
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3; 
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);	
}

void gpio_led_state(uint8_t LED_ID, uint8_t state)
{
	BitAction bitValue;
	bitValue = (state == 1) ? Bit_SET : Bit_RESET;
	switch(LED_ID)
	{
		case LED3_ORANGE_ID:
		GPIO_WriteBit(LED3_ORANGE_GPIOx, LED3_ORANGE_PinNumber, bitValue);
		break;
		case LED4_GREEN_ID:
		GPIO_WriteBit(LED4_GREEN_GPIOx, LED4_GREEN_PinNumber, bitValue);
		break;
		case LED5_RED_ID:
		GPIO_WriteBit(LED5_RED_GPIOx, LED5_RED_PinNumber, bitValue);
		break;
		case LED6_BLUE_ID:
		GPIO_WriteBit(LED6_BLUE_GPIOx, LED6_BLUE_PinNumber, bitValue);
		break;
	}
}

