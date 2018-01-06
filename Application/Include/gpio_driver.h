#ifndef GPIO_DRIVER_H__
#define GPIO_DRIVER_H__

#include <stdint.h>
#include "stm32f4xx.h"

#define NOAF 					0

#define GREENLED			12
#define ORANGELED			13
#define REDLED				14
#define BLUELED				15

//GPIO output mode
#define INPUT 						0x00
#define OUTPUT 						0x01
#define ALTFUNCTION 			0x02
#define ANALOG 						0x03

//GPIO ouput type
#define PUSHPULL 					0x00
#define OPENDRAIN 				0x01

//GPIO ouput speed
#define LOWSPEED 					0x00
#define MEDIUMSPEED 			0x01
#define HIGHSPEED 				0x02
#define VERYHIGHSPEED 		0x03

//GPIO pull-up pull-down
#define NOPULL 						0x00
#define PULLUP 						0x01
#define PULLDOWN					0x02

//GPIO AF mode
#define AF0 							0x00
#define AF1							``0x01
#define AF2 							0x02
#define AF3							``0x03
#define AF4 							0x04
#define AF5 							0x05
#define AF6 							0x06
#define AF7 							0x07
#define AF8 							0x08
#define AF9 							0x09
#define AF10 							0x0A
#define AF11						``0x0B
#define AF12 							0x0C
#define AF13							0x0D
#define AF14 							0x0E
#define AF15 							0x0F

//GPIO CLOCK ENABLE
#define _GPIOA_CLK_ENABLE() 				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN)
#define _GPIOB_CLK_ENABLE() 				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN)
#define _GPIOC_CLK_ENABLE() 				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN)
#define _GPIOD_CLK_ENABLE() 				(RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN)


void gpioInit(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction);
uint8_t gpioReadFromPin(GPIO_TypeDef *gpioX, uint8_t pinNumber);
void gpioWriteToPin(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t value);
#endif
