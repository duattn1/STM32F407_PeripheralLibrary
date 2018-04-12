#include "interrupt_controller.h"

bool isButtonPressed;

void NVICSetup(void){	
	GPIOInterruptConfig();
	
	/* Set priority for interrupts */	
	NVIC_SetPriority(EXTI0_IRQn, 0);
	
	/* Enable interrupts*/
	NVIC_EnableIRQ(EXTI0_IRQn);
}

void GPIOInterruptConfig(void){
	extiConfig(EXTI0, EXTI_RISING_TRIGGER_ENABLE);	
	extiSourceSelect(EXTI0, EXTIx_PA);
}

void EXTI0_IRQHandler(void){
	/* Clear EXTI0 pending bit by writting 1 */
	EXTI->PR |= (1 << 0);
	
	/* Things to do in EXTI0 IRQ */
	isButtonPressed = true;
}

void extiConfig(uint8_t extiLine, uint8_t risingTriggerEnable){
	EXTI->IMR |= 1 << extiLine;
	if(risingTriggerEnable){
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
