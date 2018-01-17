#ifndef INTERRUPT_CONTROLLER_H_
#define INTERRUPT_CONTROLLER_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include "../Include/uart_driver.h"

#define EXTI_FALLING_TRIGGER_DISABLE 		0x00
#define EXTI_FALLING_TRIGGER_ENABLE 		0x01

#define USARTX_RXNE_INTERRUPT_ENABLE		(0x01 << 5)
#define USARTX_TC_INTERRUPT_ENABLE			(0x01 << 6)
#define USARTX_TXE_INTERRUPT_ENABLE			(0x01 << 7)

static volatile uint8_t receivedChar;

typedef enum{
	EXTI0		= 0,
	EXTI1		= 1,
	EXTI2		= 2,
	EXTI3		= 3,
	EXTI4		= 4,
	EXTI5		= 5,
	EXTI6		= 6,
	EXTI7		= 7,
	EXTI8		= 8,
	EXTI9		= 9,
	EXTI10	= 10,
	EXTI11	= 11,
	EXTI12	= 12,
	EXTI13	= 13,
	EXTI14	= 14,
	EXTI15	= 15,
	EXTI16	= 16,
	EXTI17	= 17,
	EXTI18	= 18,
	EXTI19	= 19,
	EXTI20	= 20,
	EXTI21	= 21,
	EXTI22	= 22,
	EXTI23	= 23
} extiLine;


typedef enum{
	EXTIx_PA			=	0x00,
	EXTIx_PB			=	0x01,
	EXTIx_PC			=	0x02,
	EXTIx_PD			=	0x03,
	EXTIx_PE			=	0x04,
	EXTIx_PF			=	0x05,
	EXTIx_PG			=	0x06,
	EXTIx_PH			=	0x07,
	EXTIx_PI			=	0x08
} exti0To15Source;

extern void gpioPinSet(GPIO_TypeDef *gpioX, uint8_t pinNumber);

void nvicSetup(void);

void gpioInterruptConfig(void);
void usartInterruptConfig(void);
void usartXConfig(USART_TypeDef *uartX);

void extiConfig(uint8_t extiLine, uint8_t fallingTriggerEnable);
void extiSourceSelect(uint8_t extiLine, uint8_t extiSource);

#endif /* INTERRUPT_CONTROLLER_H_ */
