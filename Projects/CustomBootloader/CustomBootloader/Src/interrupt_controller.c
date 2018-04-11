#include "interrupt_controller.h"
	

uint8_t hexImage[5120];	
uint32_t lengthOfHexImage;
uint8_t *ptr = hexImage;


uint8_t hexRecord[128];	//some hex records may have more text in 1 record

uint8_t requestSendindHex = 0x7F;
bool writeOn;


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
	while(!(USART3->SR & USART_SR_FLAG_RXNE));
	receivedChar = (uint8_t)USART3->DR;

	if(writeOn == true){
		
	}
	
	if(receivedChar == WRITE_CMD){
		writeOn = true;
	}
	
	
	//printf("%c", receivedChar);	
}
