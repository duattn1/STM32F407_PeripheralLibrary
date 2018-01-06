#ifndef UART_DRIVER_H__
#define UART_DRIVER_H__

#include <stdint.h>
#include "stm32f4xx.h"

#define APB1_CLK_SPEED 						16000000  //duat: recheck the speed
#define APB2_CLK_SPEED 						32000000	//duat: recheck the speed

#define USART_FLAG_TXE 0x80

//UART word length
#define IS_8_BITS_DATA						0x00
#define IS_9_BITS_DATA						0x01

//UART number of stop bits
#define IS_1_STOP_BIT							0x00
#define IS_0DOT5_STOP_BIT					0x01
#define IS_2_STOP_BIT							0x02
#define IS_1DOT5_STOP_BIT					0x03

//UART parity check enable
#define DISABLE_PARITY						0x00
#define ENABLE_PARITY							0x01

//UART oversampling rate
#define OVER_BY_16								0x00
#define OVER_BY_8									0x01

#define _USART1_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#define _USART2_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART2EN)
#define _USART3_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART3EN)
#define _UART4_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART4EN)
#define _UART5_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART5EN)
#define _USART6_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART6EN)

void uartInit(USART_TypeDef *uartX, uint8_t wordLength, uint8_t stopBitNumber, uint8_t parityEnable, uint8_t oversampling, uint32_t baudrate);

void sendChar(USART_TypeDef *uartX, uint8_t character);
void sendString(USART_TypeDef *uartX, char text[], int length);
uint8_t getChar(USART_TypeDef *uartX);
uint8_t* getString(USART_TypeDef *uartX, uint8_t buff[], uint8_t length);

uint16_t baudrateMantissaCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);
uint16_t baudrateFractionCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);

#endif
