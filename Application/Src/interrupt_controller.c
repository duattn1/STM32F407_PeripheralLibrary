#include "../Include/interrupt_controller.h"


void nvicSetup(void){	
	//gpioInterruptConfig();
	//usartInterruptConfig();
	spiInterruptConfig();
	
	/* Set priority for interrupts */	
	//NVIC_SetPriority(EXTI0_IRQn, 0);
	//NVIC_SetPriority(USART3_IRQn, 0);
	NVIC_SetPriority(SPI2_IRQn, 0);
	
	/* Enable interrupts*/
	NVIC_EnableIRQ(SPI2_IRQn);
}

void gpioInterruptConfig(void){
	extiConfig(EXTI0, EXTI_FALLING_TRIGGER_ENABLE);	
	extiSourceSelect(EXTI0, EXTIx_PA);
}

void usartInterruptConfig(void){
	usartXConfig(USART3);
}	

void spiInterruptConfig(void){
	spiXConfig(SPI2);
}

void usartXConfig(USART_TypeDef *uartX){
	
	/* Enable TXNE interrupt */
	uartX->CR1 |= USARTX_RXNE_INTERRUPT_ENABLE;
}

void spiXConfig(SPI_TypeDef *spiX){
	
	/*Enable RXNE interrupt*/
	spiX->CR2 |= SPIX_RXNE_INTERRUPT_ENABLE;
}

void EXTI0_IRQHandler(void){
	/* Clear EXTI0 pending bit by writting 1 */
	EXTI->PR |= (1 << 0);
	
	/* Things to do in EXTI0 IRQ */
	//GPIOD->ODR ^= 1 << 14;
}

void USART3_IRQHandler(void){			
	while(!(USART3->SR & USART_FLAG_RXNE));
	receivedChar = (uint8_t)USART3->DR;
	
	if(receivedChar){
		//sendString(USART3, "received: ", 10);
		sendChar(USART3, receivedChar);
	}

		ITM_SendChar(receivedChar);
	
}

void SPI2_IRQHandler(void){
	while(!(SPI2->SR & SPI_I2S_FLAG_RXNE));
	receivedChar = (uint8_t)SPI2->DR;
	ITM_SendChar(receivedChar);
}

void extiConfig(uint8_t extiLine, uint8_t fallingTriggerEnable){
	EXTI->IMR |= 1 << extiLine;
	if(fallingTriggerEnable){
		EXTI->RTSR |= 1 << extiLine;
	} else {
		EXTI->RTSR &= ~(1 << extiLine);
	}			
}


void extiSourceSelect(uint8_t extiLine, uint8_t extiSource){
	if(extiLine <= 3){
		SYSCFG->EXTICR[0] |= extiSource << 4*extiLine;
	} else if(extiLine <= 7){
		SYSCFG->EXTICR[1] |= extiSource << 4*(extiLine - 4);
	} else if(extiLine <= 11){
		SYSCFG->EXTICR[2] |= extiSource << 4*(extiLine - 8);
	} else {
		SYSCFG->EXTICR[3] |= extiSource << 4*(extiLine - 12);
	}
}
