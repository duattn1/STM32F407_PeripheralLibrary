/** @file main.c
 *  @brief Application main function
 *
 *  This is the main place to control the application.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */ 
 
#include "stm32f4xx.h"
#include "GPIO_driver.h"
#include "UART_driver.h"
#include "FLASH_driver.h"
#include "interrupt_controller.h"
#include "hex_file_processing.h"

#define MAX_LENGTH 256
#define APPLICATION_ADDRESS        0x080E0000
#define FIRMWARE_START_ADDR                 (uint32_t)(&APPLICATION_ADDRESS)


/**************************/

//register uint32_t regMainStackPointer __ASM("msp");

/***************************/




extern uint8_t receivedString[5120];
extern uint32_t lengthOfReceivedString;
extern uint8_t isStringReceived;

USART_TypeDef *UARTPortToRedirect = USART3;

typedef void (*pFunction)(void);

void setupUART(){
	/* Configure PB10 as TX and PB11 as RX */
	GPIOInit(GPIOB, USART3PortB.txPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	GPIOInit(GPIOB, USART3PortB.rxPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);		
	
	UARTInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);		
	
	NVICSetup();
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

void Remap_Table(void)
{
    // Copy interrupt vector table to the RAM.
    volatile uint32_t *VectorTable = (volatile uint32_t *)0x20000000;
    uint32_t ui32_VectorIndex = 0;

		//Copy programmed app ISR table to RAM (0x20000000)
		//There are 98 items in ISR => check the DB
    for(ui32_VectorIndex = 0; ui32_VectorIndex < 98; ui32_VectorIndex++)
    {
        VectorTable[ui32_VectorIndex] = *(__IO uint32_t*)((uint32_t)APPLICATION_ADDRESS + (ui32_VectorIndex << 2));
    }

//    __HAL_RCC_AHB_FORCE_RESET();

//    //  Enable SYSCFG peripheral clock
//    __HAL_RCC_SYSCFG_CLK_ENABLE();

//    __HAL_RCC_AHB_RELEASE_RESET();

//    // Remap RAM into 0x0000 0000
//    __HAL_SYSCFG_REMAPMEMORY_SRAM();
}

int main(void){		
	setupUART();	

	
//	FLASHRead(FLASH_SECTOR_11, FLASH_SECTOR_11 + 80);

//	FLASHEraseSector();	
//	FLASHRead(FLASH_SECTOR_11, FLASH_SECTOR_11 + 80);
//	
//	FLASHWrite(FLASH_SECTOR_11, FLASH_SECTOR_11 + hexDataLength, hexData);	
//	FLASHRead(FLASH_SECTOR_11, FLASH_SECTOR_11 + 80);
	
	printf("Init device completed\r\n");
	
	
	
	
	while(1){
		if(isStringReceived){
			// Turn of this flag
			isStringReceived = 0;							
			
			printf("Start programing\r\n");
			programHexImage(receivedString, lengthOfReceivedString);
			
			
			/*******************************************************************************/			
			
			pFunction appEntry;
			uint32_t appStack, currentMSP;

			/* Get the application stack pointer (First entry in the application vector table) */
			appStack = (uint32_t) *((__IO uint32_t*)APPLICATION_ADDRESS);
	
			currentMSP = (uint32_t) *((__IO uint32_t*)0x08000000);

			/* Get the application entry point (Second entry in the application vector table) */
			appEntry = (pFunction) *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);

			// Reset RCC, disable all interrupts
			RCC_DeInit();
			SysTick->CTRL = 0; //reset SysTick timer
			SysTick->LOAD = 0;
			SysTick->VAL = 0;
			
			/* Reconfigure vector table offset register to match the application location */
			//Remap_Table();
			//NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xE0000);						
			SCB->VTOR = APPLICATION_ADDRESS ;			
			

			/* Set the application stack pointer */
			__set_MSP(appStack);
			
			
			/* Start the application */
			appEntry();
			
			/******************************************************************************/
			
		}
	}
}
