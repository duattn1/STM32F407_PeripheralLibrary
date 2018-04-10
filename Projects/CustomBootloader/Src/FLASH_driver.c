/** @file FLASH_driver.c
 *  @brief Function implementation for the FLASH driver.
 *
 *  This contains the function implementation for the FLASH driver.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */ 

#include "FLASH_driver.h"

void FLASHUnlock(void){
	if((FLASH->CR & FLASH_CR_LOCK) != 0){ // RESET
		FLASH->KEYR = FLASH_KEY1;
		FLASH->KEYR = FLASH_KEY2;
	}  
}

void FLASHRead(uint32_t startAddress, uint32_t endAddress)
{
	uint32_t currentAddress = startAddress;
	uint32_t readData;
	uint32_t numberDataInOneLine = 3;
	
	printf("Start reading\r\n");
	
	while(currentAddress < endAddress){
		readData = *(__IO uint32_t*)currentAddress;
		printf(" 0x%08X", readData);
		if(numberDataInOneLine == 0){			
			printf("\r\n");
			numberDataInOneLine = 4;
		}
		currentAddress = currentAddress + 4;
		numberDataInOneLine--;
	}
	printf("\r\n");
}


void FLASHEraseSector(void){
	FLASHUnlock();
	
	if(FLASH->SR & FLASH_FLAG_BSY){
		printf("Flash is busy\r\n");
	} else {
		printf("Start erasing\r\n");

		FLASH->CR |= 1 << 1; // Select setor erase mode
		FLASH->CR |= 11 << 3; // Choose sector 11 to delete
		
		
		FLASH->CR |= 1 << 16; // Set the STRT bit in the FLASH_CR register
		
		/* Wait for the BSY bit to be cleared */
		while(FLASH->SR & FLASH_FLAG_BSY);
		
		printf("Erasing complete\r\n");
	}
}

void FLASHWrite(uint32_t startAddress, uint32_t endAddress, uint8_t buffer[]){
	
	FLASHUnlock();
	
	if(FLASH->SR & FLASH_FLAG_BSY){		
		printf("Flash is busy\r\n");			
	} else {
		printf("Start writing\r\n");

		uint32_t currentAddress = startAddress;
		uint32_t byteIndex = 0;
		
		FLASH->CR |= 1 << 0; 	// Set the PG bit in the FLASH_CR register
		
		FLASH->CR |= 2 << 8; //word access
		
		/* Write the data */
		while (currentAddress < endAddress)
		{
			/* Note that the 1st byte will be written to LSB bit of Flash
			/ The 4th bytes will be written to MSB bit of Flash
			*/
			uint32_t dataToWrite = 0;
			dataToWrite |= buffer[byteIndex] << 0;
			dataToWrite |= buffer[byteIndex + 1] << 8;
			dataToWrite |= buffer[byteIndex + 2] << 16;
			dataToWrite |= buffer[byteIndex + 3] << 24;
			
			*(__IO uint32_t*)currentAddress = dataToWrite;
			
			/* Update currentAddress and byteIndex */
			currentAddress += 4;
			byteIndex += 4;
		}
		
		
		/* Wait for the BSY bit to be cleared */
		while(FLASH->SR & (1 << 16));
		
		/* Disable programming for safe */
		FLASH->CR &= ~(1 << 0);
		
		printf("Writing complete\r\n");			
	}
}


uint32_t charArrayToHexArray(uint8_t charArray[], uint8_t hexArray[], uint32_t charArrayLength){
	uint32_t i;
	
	for(i = 0; i < charArrayLength/2; i++){
		hexArray[i] = byteInterpret(charArray[2*i], charArray[2*i + 1]);
	}
	
	/* Return the length of converted hexArray */
	return charArrayLength/2;
}
