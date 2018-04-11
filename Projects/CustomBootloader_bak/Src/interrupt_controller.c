#include "interrupt_controller.h"
	
uint8_t allBinaryReceived;
uint8_t receivedString[5120];	//temporary place to store the hex image received from UART
uint32_t lengthOfReceivedString;
uint8_t *ptr = receivedString;

uint8_t hexEndString[] = ":00000001FF"; /* Use to detect end of hex file */
Bool hexEndStringDetected = FALSE; 
uint8_t previousCharacter = NULL;
uint8_t endStringIndex = 0;

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

	
	/* Here is a stuff to detect the end of a hex file. 
	/ Another simple way is add character 'z' (or what ever characters different with all hex characters)
	/ at the end of hex file. Then just detect the 'z' character.
	*/
	/* ----------------Begin the stuff----------------------- */
	if(receivedChar == ':'){
		hexEndStringDetected = TRUE;
		endStringIndex++;
		previousCharacter = receivedChar;
	}
	if(hexEndStringDetected == TRUE && receivedChar != ':'){
		uint8_t tmp = *(--ptr);
		ptr++; //turn the ptr value back after getting if previous value with above statement
		if(receivedChar == hexEndString[endStringIndex] && tmp == previousCharacter){
			/* Keep hexEndStringDetected = TRUE*/
			endStringIndex++;
			previousCharacter = receivedChar;
		} else {
			hexEndStringDetected = FALSE;
			endStringIndex = 0;
			previousCharacter = NULL;
		}
	}
	/* ----------------End the stuff----------------------- */
	
	/* Store the received character*/
	*ptr = receivedChar;
	ptr++;
	lengthOfReceivedString++;
	
	/* When hexEndString is matched, set the flag allBinaryReceived to 1 */
	if(endStringIndex >= sizeof(hexEndString) - 1){ //size of hexEndString - 1 (for '\0' character)
		allBinaryReceived = 1;
	}
					
	
}
