/** @file uart_driver.h
 *  @brief Function prototypes for the uart driver.
 *
 *  This contains the function prototypes for the uart driver
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 * 	@date    	04-Jan-2018
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define APB1_CLK_SPEED 						16000000  //duat: recheck the speed
#define APB2_CLK_SPEED 						32000000	//duat: recheck the speed

#define USART_FLAG_TXE 0x80

/* UART word length */
#define IS_8_BITS_DATA						0x00
#define IS_9_BITS_DATA						0x01

/* UART number of stop bits */
#define IS_1_STOP_BIT							0x00
#define IS_0DOT5_STOP_BIT					0x01
#define IS_2_STOP_BIT							0x02
#define IS_1DOT5_STOP_BIT					0x03

/* UART parity check enable */
#define DISABLE_PARITY						0x00
#define ENABLE_PARITY							0x01

/* UART oversampling rate */
#define OVER_BY_16								0x00
#define OVER_BY_8									0x01

/* UART clock enable Macro functions */
#define _USART1_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#define _USART2_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART2EN)
#define _USART3_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART3EN)
#define _UART4_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART4EN)
#define _UART5_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART5EN)
#define _USART6_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART6EN)

/** @brief Configure a UART.
 *
 *  @param uartX The address of UART to be configured.
 *  @param wordLength The length of data bits.
 *  @param stopBitNumber The number of stop bit(s).
 *  @param parityEnable Selection to enable/disable the parity check.
 *  @param oversampling Selection of oversampling rate by 8 or 16.
 *  @param baudrate The baudrate to be configured.
 *
 *  @return void.
 */
void uartInit(USART_TypeDef *uartX, uint8_t wordLength, uint8_t stopBitNumber, uint8_t parityEnable, uint8_t oversampling, uint32_t baudrate);

/** @brief Send a character over UART.
 *
 *  @param uartX The address of UART which has a character to be sent.
 *  @param character The character to be sent.
 *
 *  @return void.
 */
void sendChar(USART_TypeDef *uartX, uint8_t character);

/** @brief Send a string over UART.
 *
 *  @param uartX The address of UART which has a string to be sent.
 *  @param text The string to be sent.
 * 	@param length The length of the string to be sent.
 *
 *  @return void.
 */
void sendString(USART_TypeDef *uartX, char text[], int length);

/** @brief Receive a character from UART.
 *
 *  @param uartX The address of UART which has a string to be received.
 *
 *  @return uint8_t.
 */
uint8_t getChar(USART_TypeDef *uartX);

/** @brief Receive a string from UART.
 *
 *  @param uartX The address of UART which has a string to be received.
 *  @param buff The buffer to temporarily stored the received string.
 * 	@param length The length of the string to be received.
 *
 *  @return uint8_t pointer.
 */
uint8_t* getString(USART_TypeDef *uartX, uint8_t buff[], uint8_t length);

/** @brief Calculate the mantissa of the baudrate to be write into UART baudrate register.
 *
 *  @param srcClk The clock value of the UART bus.
 *  @param baudrate The baudrate value to be configured.
 * 	@param oversampling The oversampling rate mode, which is from the function uartInit().
 *
 *  @return uint16_t.
 */
uint16_t baudrateMantissaCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);

/** @brief Calculate the fraction of the baudrate to be write into UART baudrate register.
 *
 *  @param srcClk The clock value of the UART bus.
 *  @param baudrate The baudrate value to be configured.
 * 	@param oversampling The oversampling rate mode, which is from the function uartInit().
 *
 *  @return uint16_t.
 */
uint16_t baudrateFractionCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);

#endif /* UART_DRIVER_H_ */
