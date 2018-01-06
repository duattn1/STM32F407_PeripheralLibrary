/** @file uart_driver.h
 *  @brief Function implementation for the uart driver.
 *
 *  This contains the function inplementation for the uart driver
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 * 	@date    	04-Jan-2018
 */
 
#include "../Include/uart_driver.h"

void uartInit(USART_TypeDef *uartX, uint8_t wordLength, uint8_t stopBitNumber, uint8_t parityEnable, uint8_t oversampling, uint32_t baudrate){
	/* Turn on the clock for the UART to be configured */
	if(uartX == USART1) _USART1_CLK_ENABLE();
	else if(uartX == USART2) _USART2_CLK_ENABLE();
	else if(uartX == USART3) _USART3_CLK_ENABLE();
	else if(uartX == UART4) _UART4_CLK_ENABLE();
	else if(uartX == UART5) _UART5_CLK_ENABLE();
	else if(uartX == USART6) _USART6_CLK_ENABLE();
	
	/* Configure length of data bits */
	uartX->CR1 |= wordLength << 12;
	
	/* Configure number of stop bit(s) */
	uartX->CR2 &= ~(0x03 << 12);
	uartX->CR2 |= stopBitNumber << 12;
	/* Configure parity check */
	uartX->CR1 |= parityEnable << 10;	
	/* Configure oversampling rate */
	uartX->CR1 |= oversampling << 15;

	/**
	 *	Configure baudrate
	 *	Bit [3:0] is for fraction setup
	 *	Bit [15:4] is for mantissa setup
	 */
	if(uartX == USART1 || uartX == USART6){ 
		uint16_t mantissa, fraction;
		fraction = baudrateFractionCal(APB2_CLK_SPEED, baudrate, oversampling);
		mantissa = baudrateMantissaCal(APB2_CLK_SPEED, baudrate, oversampling);		
		uartX->BRR |= fraction << 0;
		uartX->BRR |= mantissa << 4;
	} else { 
		uint16_t mantissa, fraction;
		fraction = baudrateFractionCal(APB1_CLK_SPEED, baudrate, oversampling);
		mantissa = baudrateMantissaCal(APB1_CLK_SPEED, baudrate, oversampling);		
		uartX->BRR |= fraction << 0;
		uartX->BRR |= mantissa << 4;
	}
	
	/* Enable transmission */
	uartX->CR1 |= 1 << 3;
	/* Enable reception */
	uartX->CR1 |= 1 << 2;
	/* Enable UART */
	uartX->CR1 |= 1 << 13;
}

void sendChar(USART_TypeDef *uartX, uint8_t character){
    while(!(uartX->SR & USART_FLAG_TXE)); //Bit USART_FLAG_TXE is set when data is transfer to shift register
    uartX->DR = character;
}

void sendString(USART_TypeDef *uartX, char text[], int length){
    uint8_t i = 0;
    while(length--){
        sendChar(uartX, text[i++]);
    }
}

uint8_t getChar(USART_TypeDef *uartX){
    while(!(uartX->SR & USART_FLAG_TXE));
    return (uint8_t)uartX->DR;
}

uint8_t* getString(USART_TypeDef *uartX, uint8_t buff[], uint8_t length){
    uint8_t i = 0;
    //char buff[128];
    uint8_t character = getChar(uartX);
    while(character != '\n'){
        buff[i++] = character;
    }
    return buff;
}

uint16_t baudrateMantissaCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling){
	uint16_t mantissa;
	mantissa = srcClk/(8*(2 - oversampling)*baudrate);
	return mantissa;
}

//duat: fraction must be more exact
uint16_t baudrateFractionCal(uint32_t srcClk, uint32_t baudrate, uint8_t oversampling){
	uint16_t remain;
	float fraction, roundedFraction;
	remain = srcClk%(8*(2 - oversampling)*baudrate);
	fraction = (float)remain/(8*(2 - oversampling)*baudrate);
	roundedFraction = fraction * 16;
	//to round the Fraction
	if((((uint16_t)(roundedFraction*10) - 10*(uint16_t)roundedFraction)) % 10 >= 5) roundedFraction = roundedFraction + 1;
	
	return (uint16_t)roundedFraction;
}
