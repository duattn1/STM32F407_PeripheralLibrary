#include "interrupt_controller.h"
	
uint8_t isStringReceived;
uint8_t receivedString[2048];	//temporary place to store the hex image received from UART
uint32_t lengthOfReceivedString;
uint8_t *ptr = receivedString;

void NVICSetup(void){	
	USARTInterruptConfig();
	
	/* Set priority for interrupts */	
	NVIC_SetPriority(USART3_IRQn, 0);
	
	/* Enable interrupts*/
	NVIC_EnableIRQ(USART3_IRQn);
}

void USARTInterruptConfig(void){
	USARTxConfig(USART3);
}	

void USARTxConfig(USART_TypeDef *UARTx){
	
	/* Enable TXNE interrupt */
	UARTx->CR1 |= USARTX_RXNE_INTERRUPT_ENABLE;
}


void USART3_IRQHandler(void){		
	while(!(USART3->SR & USART_FLAG_RXNE));
	receivedChar = (uint8_t)USART3->DR;

	/* Store the received character*/
	*ptr = receivedChar;
	ptr++;
	lengthOfReceivedString++;
	
	if(receivedChar == 'z'){
		// Turn this flag when 
		isStringReceived = 1;				
	}
}
