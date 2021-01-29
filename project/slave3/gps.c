
#include <gps.h>
//#include <usart.h>

char RX_BUFFER_6[BUFSIZE];
int  RX_BUFFER_HEAD_6, RX_BUFFER_TAIL_6;

uint8_t TxReady6;

void GPS_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// enable peripheral clocks (note: different bus interfaces for each peripheral!)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	// map port C pins for alternate function
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) will be used for USART6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF; // GPIO pins defined as alternate
	//function
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // I/O pins speed (signal rise time)
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; // push-pull output
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // activates pullup resistors
	GPIO_Init(GPIOC, &GPIO_InitStruct); // set chosen pins
	
	// set alternate function to USART1 (from multiple possible alternate function choices)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); // pins will automatically be assigned
	//to TX/RX - refer to datasheet to see AF mappings
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
	// use USART_InitStruct to config USART1 peripheral
	USART_InitStruct.USART_BaudRate = BAUDRATE_USART6; // set baudrate from define
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// 8 data bits
	USART_InitStruct.USART_StopBits = USART_StopBits_1; // 1 stop bit
	USART_InitStruct.USART_Parity = USART_Parity_No; // no parity check
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // no HW control flow
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // enable both character transmit and
	//receive
	USART_Init(USART6, &USART_InitStruct); // set USART6 peripheral
	
	// set interrupt triggers for USART1 ISR (but do not enable USART1 interrupts yet)
	USART_ITConfig(USART6, USART_IT_TXE, DISABLE);// should be disabled
	USART_ITConfig(USART6, USART_IT_TC, ENABLE); // transmission completed event (for reseting TxReady
	//flag)
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); // character received (to trigger buffering of new
	//character)
	
	TxReady6 = 1; // USART1 is ready to transmit
	RX_BUFFER_HEAD_6 = 0; RX_BUFFER_TAIL_6 = 0; // clear rx buffer
	
	// prepare NVIC to receive USART1 IRQs
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn; // configure USART6 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// max. priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // max. priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // enable USART6 interrupt in NVIC
	NVIC_Init(&NVIC_InitStructure); // set NVIC
	//for USART6 IRQ
	
	// enables USART1 interrupt generation
	USART_Cmd(USART6, ENABLE);
}

void USART6_IRQHandler(void)
{
	static char rx_char;
	static char rx_head;
	
	// RX event
	if (USART_GetITStatus(USART6, USART_IT_RXNE) == SET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_RXNE);
		rx_char = USART_ReceiveData(USART6);
		// check for buffer overrun:
		rx_head = RX_BUFFER_HEAD_6 + 1;
		if (rx_head == BUFSIZE) rx_head = 0;
		if (rx_head != RX_BUFFER_TAIL_6)
		{
			// adding new char will not cause buffer overrun:
			RX_BUFFER_6[RX_BUFFER_HEAD_6] = rx_char;
			RX_BUFFER_HEAD_6 = rx_head; // update head
		}
	}
	
	// TX event
	if (USART_GetITStatus(USART6, USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART6, USART_IT_TC);
		TxReady6 = 1;
	}
}

void GPS_SendChar(char c)
{
	while(!TxReady6);
	USART_SendData(USART6, c);
	TxReady6 = 0;
}

void GPS_SendStr(char *str) {
	while (*str) {
		while (!(USART6->SR & USART_SR_TXE)); // Wait while transmit data register not empty
		USART6->DR = *str++;
	}
}

int GPS_Dequeue(char* c)
{
	int ret;
	ret = 0;
	*c = 0;
	NVIC_DisableIRQ(USART6_IRQn);
	if (RX_BUFFER_HEAD_6 != RX_BUFFER_TAIL_6)
	{
		*c = RX_BUFFER_6[RX_BUFFER_TAIL_6];
		RX_BUFFER_TAIL_6++;
		if (RX_BUFFER_TAIL_6 == BUFSIZE) RX_BUFFER_TAIL_6 = 0;
		ret = 1;
	}
	NVIC_EnableIRQ(USART6_IRQn);
	return ret;
}



latlon parseGPSdata(char* message){
	latlon gpsData;
	char* delimeter = ",";
	char *p,*q;
	int i = 0;
	
	p = strstr(message, delimeter);

	while(p != NULL)
	{
		
		q = strstr(p+1, delimeter);
		if(q == p + 1){
			strcpy(gpsData.time,"00.00");
			strcpy(gpsData.latitudeValue,"00.00");
			strcpy(gpsData.latitudeDirection,"0");
			strcpy(gpsData.longitudeValue,"00.00");
			strcpy(gpsData.longitudeDirection,"0");
			break;
		}
		i++;
		// Pass validity character
		if(i == 2){
			p = q;
			continue;
		}
		// After latitude and longitude stop parsing
		if(i > 6)
			break;
		
		if(i == 1){
			strncpy(gpsData.time, p+1, q-p-1);
			gpsData.time[q-p-1] = 0;
		}
		
		if(i == 3){
			strncpy(gpsData.latitudeValue, p+1, q-p-1);
			gpsData.latitudeValue[q-p-1] = 0;
		}
		if(i == 4){
			strncpy(gpsData.latitudeDirection, p+1, q-p-1);
			gpsData.latitudeDirection[q-p-1] = 0;
		}
		if(i == 5){
			strncpy(gpsData.longitudeValue, p+1, q-p-1);
			gpsData.longitudeValue[q-p-1] = 0;
		}
		if(i == 6){
			strncpy(gpsData.longitudeDirection, p+1, q-p-1);
			gpsData.longitudeDirection[q-p-1] = 0;
		}
	
		p = q;
	}
	return gpsData;
}




