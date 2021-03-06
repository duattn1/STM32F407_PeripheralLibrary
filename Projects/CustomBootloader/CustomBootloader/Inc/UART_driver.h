/** @file UART_driver.h
 *  @brief Function prototypes for the UART driver.
 *
 *  This contains the function prototypes for the UART driver
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include <stdint.h>
#include <stdio.h>
#include "stm32f407xx.h"

#define APB1_CLK_SPEED 						16000000
#define APB2_CLK_SPEED 						32000000

#define USART_SR_FLAG_PARITY_ERROR 			(1 << 0)
#define USART_SR_FLAG_FRAMING_ERROR 			(1 << 1)
#define USART_SR_FLAG_NOISE_DETECTED			(1 << 2)
#define USART_SR_FLAG_IDLE_LINE_DETECTED	(1 << 2)
#define USART_SR_FLAG_OVERRUN_ERROR			(1 << 3)
#define USART_SR_FLAG_RXNE 							(1 << 5)
#define USART_SR_FLAG_TC 								(1 << 6)
#define USART_SR_FLAG_TXE 								(1 << 7)


/* UART word length */
#define USART_CR1_8_BITS_DATA						0x00
#define USART_CR1_9_BITS_DATA						0x01

/* UART number of stop bits */
#define USART_CR2_1_STOP_BIT							0x00
#define USART_CR2_0DOT5_STOP_BIT					0x01
#define USART_CR2_2_STOP_BIT							0x02
#define USART_CR2_1DOT5_STOP_BIT					0x03

/* UART parity check enable */
#define USART_CR1_DISABLE_PARITY						0x00
#define USART_CR1_ENABLE_PARITY							0x01

/* UART oversampling rate */
#define USART_CR1_OVER_BY_16								0x00
#define USART_CR1_OVER_BY_8									0x01

/* UART clock enable Macro functions */
#define _USART1_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART1EN)
#define _USART2_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART2EN)
#define _USART3_CLK_ENABLE()				(RCC->APB1ENR |= RCC_APB1ENR_USART3EN)
#define _UART4_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART4EN)
#define _UART5_CLK_ENABLE()					(RCC->APB1ENR |= RCC_APB1ENR_UART5EN)
#define _USART6_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_USART6EN)

typedef struct {
	USART_TypeDef *USARTx;
	int8_t txPin;
	int8_t rxPin;
	int8_t clkPin;
} USARTPinPack;

static USARTPinPack USART1PortA = {USART1, 9, 10, 8};
static USARTPinPack USART1PortB = {USART1, 6, 7, -1};
static USARTPinPack USART2PortA = {USART2, 2, 3, 4};
static USARTPinPack USART2PortD = {USART2, 5, 6, 7};
static USARTPinPack USART3PortB = {USART3, 10, 11, 12};
static USARTPinPack USART3PortC = {USART3, 10, 11, 12};
static USARTPinPack USART3PortD = {USART3, 8, 9, 10};
static USARTPinPack UART4PortA = {UART4, 0, 1, -1};
static USARTPinPack UART4PortC = {UART4, 10, 11, -1};
static USARTPinPack USART6PortC = {USART6, 6, 7, 8};

static USARTPinPack UART5PortC12AndD2 = {UART5, 12, 2, -1};


/** @brief Configure a UART.
 *
 *  @param USARTx The address of UART to be configured.
 *  @param wordLength The length of data bits.
 *  @param stopBitNumber The number of stop bit(s).
 *  @param parityEnable Selection to enable/disable the parity check.
 *  @param oversampling Selection of oversampling rate by 8 or 16.
 *  @param baudrate The baudrate to be configured.
 *
 *  @return void.
 */
void USARTInit(USART_TypeDef *USARTx, uint8_t wordLength, uint8_t stopBitNumber, uint8_t parityEnable, uint8_t oversampling, uint32_t baudrate);

/** @brief Send a character over UART.
 *
 *  @param USARTx The address of UART which has a character to be sent.
 *  @param character The character to be sent.
 *
 *  @return void.
 */
void sendChar(USART_TypeDef *USARTx, uint8_t character);

/** @brief Send a string over UART.
 *
 *  @param USARTx The address of UART which has a string to be sent.
 *  @param text The string to be sent.
 * 	@param length The length of the string to be sent.
 *
 *  @return void.
 */
void sendString(USART_TypeDef *USARTx, char text[], int length);

/** @brief Receive a character from UART.
 *
 *  @param USARTx The address of UART which has a string to be received.
 *
 *  @return uint8_t.
 */
uint8_t getChar(USART_TypeDef *USARTx);

/** @brief Receive a string from UART.
 *
 *  @param USARTx The address of UART which has a string to be received.
 * 	@param length The length of the string to be received.
 *
 *  @return uint8_t pointer.
 */
uint8_t* getString(USART_TypeDef *USARTx);

/** @brief Calculate the mantissa of the baudrate to be write into UART baudrate register.
 *
 *  @param srcClk The clock value of the UART bus.
 *  @param baudrate The baudrate value to be configured.
 * 	@param oversampling The oversampling rate mode, which is from the function UARTInit().
 *
 *  @return uint16_t.
 */
uint16_t baudrateMantissaCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);

/** @brief Calculate the fraction of the baudrate to be write into UART baudrate register.
 *
 *  @param srcClk The clock value of the UART bus.
 *  @param baudrate The baudrate value to be configured.
 * 	@param oversampling The oversampling rate mode, which is from the function UARTInit().
 *
 *  @return uint16_t.
 */
uint16_t baudrateFractionCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling);

#endif /* UART_DRIVER_H_ */
