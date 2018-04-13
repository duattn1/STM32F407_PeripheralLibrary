#include "stm32f4xx.h"
#include "GPIO_driver.h"

void RCC_DeInit(void);
void enterSTM32F4Bootloader(void);

int main(void){		
	enterSTM32F4Bootloader();
	while(1){
		
	}
}

void RCC_DeInit(void){
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;

  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;

  /* Reset HSEON, CSSON, PLLON and PLLI2S bits */
  RCC->CR &= (uint32_t)0xFAF6FFFF;

  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x24003010;

  /* Reset PLLI2SCFGR register */
  RCC->PLLI2SCFGR = 0x20003000;

  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
}

void enterSTM32F4Bootloader(void){
	void (* const SysMemBootJump)(void) = (void (*)(void))(*((uint32_t*)0x1FFF0004)); //First 4 bytes is the MSP
	
	//1. Disable all peripheral clocks	
	RCC_DeInit();
	
	//2. Disable interrupts
	__disable_irq();	
	
	SysTick->CTRL = 0; //reset SysTick timer
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	//3. Set MSP to its default value
	__set_MSP(0x20001000); // set MSP to its default value
	
	SysMemBootJump();
	while(1);
}

