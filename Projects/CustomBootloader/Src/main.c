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

#define APPLICATION_ADDRESS        0x08010000

extern uint8_t receivedString[5120];
extern uint32_t lengthOfReceivedString;
extern uint8_t allBinaryReceived;

USART_TypeDef *UARTPortToRedirect = USART3;

typedef void (*pFunction)(void);

void setupUART(){
	/* Configure PB10 as TX and PB11 as RX */
	GPIOInit(GPIOB, USART3PortB.txPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	GPIOInit(GPIOB, USART3PortB.rxPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);		
	
	UARTInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);		
	
	NVICSetup();
}

int main(void){		
	setupUART();	
	

//	FLASHEraseSector();	
//	FLASHWrite(FLASH_SECTOR_11, FLASH_SECTOR_11 + hexDataLength, hexData);	
//	FLASHRead(FLASH_SECTOR_11, FLASH_SECTOR_11 + 80);
	
	printf("Init device completed\r\n");
	
	
		
	
	while(1){
		if(allBinaryReceived){
			// Turn of this flag
			allBinaryReceived = 0;							
			
			printf("Start programing\r\n");

			programHexImage(receivedString, lengthOfReceivedString);			
			
			/*******************************************************************************/						
			pFunction appEntry;
			uint32_t appStack;

			/* Get the application stack pointer (First entry in the application vector table) */
			appStack = (uint32_t) *((__IO uint32_t*)APPLICATION_ADDRESS);
	
			/* Get the application entry point (Second entry in the application vector table) */
			appEntry = (pFunction) *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);

			//*(uint32_t *)(APPLICATION_ADDRESS + 0x0000003C) = (uint32_t)SysTick_Handler_RAM;
			
			/* Disable all interrupts */
			__disable_irq();
			
			
			/* Relocate vector table offset register to match the application location */								
			SCB->VTOR = APPLICATION_ADDRESS ;			
			

			/* Set the application stack pointer */
			__set_MSP(appStack);
			
			
			/* Start the application */
			appEntry();			
			/******************************************************************************/

		}
	}
}
