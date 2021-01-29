/* gpio.h */
#ifndef GPIO_H
#define GPIO_H

#define LED3_ORANGE_ID 1
#define LED4_GREEN_ID 2
#define LED5_RED_ID 3
#define LED6_BLUE_ID 4

#include <stm32f4xx.h> // common stuff
#include <stm32f4xx_gpio.h> // gpio control
#include <stm32f4xx_rcc.h> // reset anc clocking
#define LED3_ORANGE_RCC_GPIOx RCC_AHB1Periph_GPIOD
#define LED4_GREEN_RCC_GPIOx RCC_AHB1Periph_GPIOD
#define LED5_RED_RCC_GPIOx RCC_AHB1Periph_GPIOD
#define LED6_BLUE_RCC_GPIOx RCC_AHB1Periph_GPIOD
#define LED3_ORANGE_GPIOx GPIOD
#define LED4_GREEN_GPIOx GPIOD
#define LED5_RED_GPIOx GPIOD
#define LED6_BLUE_GPIOx GPIOD
#define LED3_ORANGE_PinNumber GPIO_Pin_13
#define LED4_GREEN_PinNumber GPIO_Pin_12
#define LED5_RED_PinNumber GPIO_Pin_14
#define LED6_BLUE_PinNumber GPIO_Pin_15

void gpio_init(void);
void gpio_led_state(uint8_t LED_ID, uint8_t state);

#endif
