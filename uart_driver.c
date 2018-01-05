#include "uart_driver.h"

void uartInit(USART_TypeDef *uartX, uint8_t wordLength, uint8_t stopBitNumber, uint8_t parityEnable, uint8_t oversampling, uint32_t baudrate){
	if(uartX == USART1) _USART1_CLK_ENABLE();
	else if(uartX == USART2) _USART2_CLK_ENABLE();
	else if(uartX == USART3) _USART3_CLK_ENABLE();
	else if(uartX == UART4) _UART4_CLK_ENABLE();
	else if(uartX == UART5) _UART5_CLK_ENABLE();
	else if(uartX == USART6) _USART6_CLK_ENABLE();
	
	//config word length
	uartX->CR1 |= wordLength << 12;
	//config stop bits
	uartX->CR2 &= ~(0x03 << 12);
	uartX->CR2 |= stopBitNumber << 12;
	//config parity check
	uartX->CR1 |= parityEnable << 10;	
	//config oversampling rate
	uartX->CR1 |= oversampling << 15;
	//config baudrate
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
	
	//enable transmission
	uartX->CR1 |= 1 << 3;
	//enable reception
	uartX->CR1 |= 1 << 2;
	//enable UART
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
	float fraction;
	remain = srcClk%(8*(2 - oversampling)*baudrate);
	fraction = (float)remain/(8*(2 - oversampling)*baudrate);
	return (uint16_t)(fraction * 16);
}
