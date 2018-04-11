/** @file hex_file_processing.c
 *  @brief Function implementation for the hex file processing.
 *
 *  This contains the function implementation for the hex file processing.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */ 

#include "hex_file_processing.h"

uint8_t byteInterpret(uint8_t char1, uint8_t char2){
	uint8_t result = 0;
	/* Get char1 integer value*/
	if(char1 < 'A'){
		result |= (char1 - 48) << 4;	
	} else {
		result |= (char1 - 55) << 4;
	}
	
	/* Get char2 integer value*/
	if(char2 < 'A'){
		result |= char2 - 48;	
	} else {
		result |= char2 - 55;
	}
	
	return result;
}

hexRecord readHexRecord(uint8_t hex[], uint32_t readIndex){
	uint8_t j;
	/* Set all struct variables to 0*/
	hexRecord record;
	record.length = 0;
	record.offset = 0;
	record.type = 0;
	for(j = 0; j < 20; j++){
		record.data[j] = 0;
	}
	record.checksum = 0;	
	
	record.length = byteInterpret(hex[readIndex + 1], hex[readIndex + 2]);
	/* offset is 2 bytes */
	record.offset |= (byteInterpret(hex[readIndex + 3], hex[readIndex + 4])) << 8;
	record.offset |= byteInterpret(hex[readIndex + 5], hex[readIndex + 6]) << 0;
	record.type = byteInterpret(hex[readIndex + 7], hex[readIndex + 8]);

	for(j = 0; j < record.length ;j++){
		record.data[j] = byteInterpret(hex[readIndex + 9 + 2*j], hex[readIndex + 10 + 2*j]);
	}
	
	record.checksum = byteInterpret(hex[readIndex + 9 + 2*record.length], hex[readIndex + 10 + 2*record.length]);   
 	
	return record;
}

void programHexImage(uint8_t hexImageContent[],  uint32_t lengthOfReceivedString){
	int readIndex = 0;    
	uint32_t baseAddress = 0;
    
	// Erase sector first
	FLASHEraseSector();
			
	while(readIndex < lengthOfReceivedString){
		while(hexImageContent[readIndex] != ':' && readIndex < lengthOfReceivedString){
   			readIndex++;
  	}
//  	printf("----------\n");
  	if(hexImageContent[readIndex] == ':'){
  			hexRecord xx;  			
  			xx = readHexRecord(hexImageContent, readIndex);
  			if(xx.type == 4){
					baseAddress |= xx.data[0] << 24;	
					baseAddress |= xx.data[1] << 16;	
						//baseAddress = 0x0800C000;
				}	else if (xx.type == 0){	
				printf("Write at address: %08x\r\n", baseAddress + xx.offset);
				FLASHWrite(baseAddress + xx.offset, baseAddress + xx.offset + xx.length, xx.data);	
				} /* Not process the other type of record yet*/  			
			
		}		
		readIndex++;	
	}
}
