#include "stm32f4xx.h"
#include "GPIO_driver.h"
#include "UART_driver.h"

void RCC_DeInit(void);
void bootloaderInit(void);

int main(void){	
	/* Enter bootloader without patterns. Before starting, we must */
	//1. Disable all peripheral clocks	
	//2. Disable used PLL
	//3. Disable interrupts
	//4. Clear pending interrupts	
	
	
	/* Jump to memory in System Memory */
	//1. Shut down any tasks running
	//2. Switch to HSI clock source - No PLL
	//3. Disable interrupts
	//4. Set MSP to its default value
	//5. Load program counter with SystemMemory reset vector
	bootloaderInit();
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

void bootloaderInit(void){
	void (* const SysMemBootJump)(void) = (void (*)(void))(*((uint32_t*)0x1FFF0004)); //First 4 bytes is the MSP
	
	RCC_DeInit();
	SysTick->CTRL = 0; //reset SysTick timer
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	
	__set_PRIMASK(1); // disable interrupts
	__set_MSP(0x20001000); // set MSP to its default value
	SysMemBootJump();
	while(1);
}

