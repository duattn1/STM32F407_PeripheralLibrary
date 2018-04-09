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

extern uint8_t receivedString[2048];
extern uint32_t lengthOfReceivedString;
extern uint8_t isStringReceived;

USART_TypeDef *UARTPortToRedirect = USART3;

void setupUART(){
	/* Configure PB10 as TX and PB11 as RX */
	GPIOInit(GPIOB, USART3PortB.txPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	GPIOInit(GPIOB, USART3PortB.rxPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);		
	
	UARTInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);		
	
	NVICSetup();
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
		}
	}
}
