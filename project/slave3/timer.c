
#include <timer.h> 
#include <usart.h>
#include <stdlib.h>

uint32_t TIMER_Ticks_Millisec; 
 
void TIMER_init(void) 
{ 
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; // init def struct for timer 
NVIC_InitTypeDef NVIC_InitStructure; // init def struct for NVIC 
uint16_t TimerPeriod; // 16-bit value because ARR register is 16-bit (although TIM2 is 32-bit!) 
RCC_ClocksTypeDef RCC_Clocks; // for reading current clock setting - useful 
uint32_t APB1_CLK; // APB1 clock - max. 42 MHz! 
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // enable TIM2 peripheral clock  
  TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE); // ensure that interrupt is disabled 
 	 
 // Compute the value to be set in ARR register to generate signal frequency at 1.000 kHz (timebase 1 ms) 
 // Warning - 168 MHz - this is default processor core frequency but it is NOT freq. at which peripherals  // are clocked!  
 // TimerPeriod = (uint16_t)((SystemCoreClock / 10000 ) - 1);   	 
 // for 1 kHz division should be 1000 and ARR == 168000, but this cannot be represented with uint16;   
 // therefore, it is necessery to divide by 10x more (10000) with prescaler x10; 
 // to derive correct frequency, first check APB1 clock: 
 RCC_GetClocksFreq(&RCC_Clocks); // fill query struct 
 APB1_CLK = RCC_Clocks.PCLK1_Frequency;  // WARNING: APB1 is "slow" peripheral interface that with
		
 // SYSCLK = 168 MHz cannot be faster than 42 MHz (APB1 domain!) (pp. 213)  
 // by default it is set to 13.44 MHz, and 13.44 MHz : 1000 = 13440.0, what would be acceptable for reload  // without prescaler 
 
 // Page 213: 
 //  	The timer clock frequencies are automatically set by hardware. There are two cases:  //  	1. If the APB prescaler is 1, the timer clock frequencies are set to the same frequency as  //  	that of the APB domain to which the timers are connected. 
 //  	2. Otherwise, they are set to twice (×2) the frequency of the APB domain to which the  //  	timers are connected. 	 
 // => therefore, we need to multiplay APB1_CLK with 2: 
 TimerPeriod = (uint16_t)(((APB1_CLK * 2)/ 1000 ) - 1);    	// 1 kHz    
 // Time Base configuration 
 TIM_TimeBaseStructure.TIM_Prescaler = 0;  
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // counts from 0 to autoreload, and then 
   //back to 0 
 TIM_TimeBaseStructure.TIM_Period = TimerPeriod; 
 TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
 TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; 
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
 
 // set TIM2 IRQ 
 TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); 	 
 	 
 // Enable the TIM2 global Interrupt  
 NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
 NVIC_Init(&NVIC_InitStructure);  	 	 
 	 
 // setup global timekeeping variable (incremented each 1 ms)  
  TIMER_Ticks_Millisec = 0; 
 	 
 // TIM2 counter enable
 TIM_Cmd(TIM2, ENABLE);   
} 

void TIM2_IRQHandler(void) 
{ 
 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
 	{ 
 	 	TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		TIMER_Ticks_Millisec++; 
 	} 
} 


uint32_t TIMER_get_millisec() 
{ 
uint32_t value; 
 	 
 	NVIC_DisableIRQ(TIM2_IRQn);
	value = TIMER_Ticks_Millisec;
	NVIC_EnableIRQ(TIM2_IRQn); 
 	return value;  
} 

void TIMER_wait_millisec(uint32_t ms) 
{ 
uint32_t t1, t2; 
 	 
 	t1 = TIMER_get_millisec();
	while(1) 
 	{ 
		
 	 	t2 = TIMER_get_millisec();
		if ((t2 - t1) >= ms) break; 
 	 	if (t2 < t1) break; // almost never occur, once in 49 days 
 	} 	 
} 

