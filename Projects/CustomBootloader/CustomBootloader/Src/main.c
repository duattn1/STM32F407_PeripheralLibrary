/** @file main.c
 *  @brief Application main function
 *
 *  This is the main place to control the application.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */ 
 
#include "stm32f4xx.h"
#include <stdbool.h>
#include <stdio.h>
#include "hex_image.h"
#include "GPIO_driver.h"
#include "UART_driver.h"
#include "FLASH_driver.h"
#include "hex_file_processing.h"

#define APPLICATION_ADDRESS        0x08010000

#define ACK							0x79
#define NACK						0x1F
#define BINARY_REQUEST	0x01


uint8_t binaryImage[0x05D0]; //10 KB
uint8_t *binaryImagePtr = binaryImage;
uint32_t binaryImageLength = 0;
uint32_t binaryBaseAddress = 0;
bool endOfHexFile = false;

USART_TypeDef *UARTPortToRedirect = USART3;

typedef void (*pFunction)(void);

void initBootloader(void);
bool checkApplicationUpdate(void);
void jumpToApplication(void);
void receivedDataFromHex(void);

int main(void){	
	initBootloader();	
//	uint8_t selectedCmd;	
	
	if(checkApplicationUpdate() == false){
		jumpToApplication();
	} 
	/* Trigger the host to send the image data */
	printf("%c", BINARY_REQUEST);
	
	/* Do application update in the while loop*/
	while(1){
		/* Get the image first */
		if(endOfHexFile == false){
			receivedDataFromHex();
		} else {
			programBinaryImage(binaryBaseAddress, binaryImageLength, binaryImage);
			jumpToApplication();
		}		
	}
}

	
void initBootloader(void){
	/*------------------- USART initialization -------------------*/
	
	/* Configure PB10 as TX and PB11 as RX */
	GPIOInit(GPIOB, USART3PortB.txPin, GPIO_MODER_ALTERNATE_FUNCTION, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_VERY_HIGH_SPEED, GPIO_PUPDR_NOPULL, AF7);
	GPIOInit(GPIOB, USART3PortB.rxPin, GPIO_MODER_ALTERNATE_FUNCTION, GPIO_OTYPER_PUSHPULL, GPIO_OSPEEDR_VERY_HIGH_SPEED, GPIO_PUPDR_NOPULL, AF7);	
	USARTInit(USART3, USART_CR1_8_BITS_DATA, USART_CR2_1_STOP_BIT, USART_CR1_DISABLE_PARITY, USART_CR1_OVER_BY_16, 9600);		
		
	//NVICSetup(); 
	printf("Start bootloader\r\n");	
}


bool checkApplicationUpdate(void){
	/* Check if there is hex image is stored and the hex file is different with the current application */
	
	return true;
}


void jumpToApplication(void){
	pFunction appEntry;
	uint32_t appStack;

	printf("Exit bootloader\r\n");	
	
	/* Get the application stack pointer */
	appStack = (uint32_t) *((__IO uint32_t*)APPLICATION_ADDRESS);
	
	/* Get the application entry point */
	appEntry = (pFunction) *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
				
	/* Disable all interrupts */
	__disable_irq();
			
	/* Set the application stack pointer */
	__set_MSP(appStack);			
			
	/* Start the application */
	appEntry();		
}

void receivedDataFromHex(void){
	uint8_t verifyChecksum = 0;
	hexRecord hRecord;
	uint8_t offsetByte1, offsetByte2;
	uint8_t i;
	bool waitFor1stOffset = false;	
	
	/* Get length of hex record */
	hRecord.length = getChar(USART3);
	verifyChecksum += hRecord.length;
		
	/* Get address of hex record */
	offsetByte1 = getChar(USART3);
	offsetByte2 = getChar(USART3);
	verifyChecksum += offsetByte1;
	verifyChecksum += offsetByte2;
	hRecord.offset |= (offsetByte1 << 8) | (offsetByte2 << 0);

	/* Get record type of hex record */
	hRecord.type = getChar(USART3);
	verifyChecksum += hRecord.type;
	
	/* Get data of hex record */
	for(i = 0; i < hRecord.length; i++){
		hRecord.data[i] = getChar(USART3);
		verifyChecksum += hRecord.data[i];
	}
	
	/* Get checksum of hex record */
	hRecord.checksum = getChar(USART3);
	
	/* Compare received checksum and calculated checksum */
	verifyChecksum = (verifyChecksum ^ 0xFF) + 1;
	if(verifyChecksum == hRecord.checksum){
		//send ACK and host will transmit next record
		printf("%c", ACK);
		
		/**************** Things to do when received record is OK*********************/			
	
		/* set the base address of binary image 
		* Note that this is for some common cases, just lack of the others 
		*/
		if(waitFor1stOffset == true){		//Add 1st offset to the base address
			binaryBaseAddress |= hRecord.offset << 0;
			waitFor1stOffset = false;
		}			
		if(hRecord.type == 0x04){
			binaryBaseAddress |= hRecord.data[0] << 24;
			binaryBaseAddress |= hRecord.data[1] << 16;
			waitFor1stOffset = true;			
		} 
		
		/* Detect end of image file */
		if(hRecord.type == 0x01){
			endOfHexFile = true;
		}
		
		if(hRecord.type == 0x00){
			/* Store data from hex data record into the the binary array */		
			for(i = 0; i < hRecord.length; i++){
				*binaryImagePtr = hRecord.data[i];
				binaryImagePtr++;				
			}	
			binaryImageLength += hRecord.length;	// update length of binary
		}					
		
	/******************************************************************************/	
	} else {
		//send NACK and host will transmit this record again
		printf("%c", NACK);
	}
}
