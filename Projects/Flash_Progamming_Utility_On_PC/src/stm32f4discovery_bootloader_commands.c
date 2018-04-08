#include "stm32f4discovery_bootloader_commands.h"

uint8_t calculateChecksum(int count, ...){
   va_list list;
   uint8_t i = 0;
   uint8_t checksum = 0;

   va_start(list, count); 
   for(i = 0; i < count; i++){
     checksum ^= va_arg(list, int);
   }

   va_end(list);

   return checksum;
}

uint8_t startBootloader(void){
	uint8_t ack = 0;
	uint8_t cmd[] = {0x7F};
	
	
	sendString(cmd, sizeof(cmd));	
	ack = receiveByte();
		
	if(ack == ACK){
		printf("Bootloader is waiting for commands.\n");
		printf("--------------------\n");
		return 1; /* true */
	} else {
		printf("Bootloader CANNOT receive commands\n");
		printf("--------------------\n");
		return 0; /* false */
	}
				
}

void getBootloaderVersion(void){
	char ack;
	uint16_t PID = 0;
	char cmd[] = {0x02, 0xFD};
	char* buffer; 
	
	sendString(cmd, sizeof(cmd));	
	buffer = (char*)receiveString();
	
	ack = buffer[0];
	if(ack != ACK){
		printf("[NACK] Bootloader is not able to process Get PID command.\n");
		return;
	} else{
		PID |= buffer[2] << 8;
		PID |= buffer[3];
	
		printf("PID: %0.4x\n", PID);
	}			
}

void sendMemoryAddress(uint32_t address){
	uint8_t addressChecksum = 0;
	uint8_t addressByte1 = 0;
	uint8_t addressByte2 = 0;
	uint8_t addressByte3 = 0;
	uint8_t addressByte4 = 0;	
	
	addressByte1 |= (address & 0xFF000000) >> 24;
	addressByte2 |= (address & 0x00FF0000) >> 16;
	addressByte3 |= (address & 0x0000FF00) >> 8;
	addressByte4 |= (address & 0x000000FF) >> 0;
	addressChecksum = calculateChecksum(4, addressByte1, addressByte2, addressByte3, addressByte4);
		
	/* Send 4 bytes of start address, send MSB bits first*/
	sendByte(addressByte1);
	sendByte(addressByte2);
	sendByte(addressByte3);
	sendByte(addressByte4);
	/* Send the checksum of 4 bytes of startAddress */
	sendByte(addressChecksum);	
}

void readMemory(void){
	char ack;
	char cmd[] = {0x11, 0xEE};
	char* buffer; 	
		
	sendString(cmd, sizeof(cmd));	
	buffer = (char*)receiveString();
	
	ack = buffer[0];
	if(ack != ACK){
		printf("[NACK] Bootloader is not able to process Read Memory command.\n");
		return;
	} else {
		printf("[ACK] ready\n");
		uint32_t startAddress, endAddress;
		uint8_t readByteLength;
			
		startAddress 	= 0x08000000;
		endAddress 		= 0x080004FF;
		readByteLength = endAddress - startAddress;
		
		sendMemoryAddress(startAddress);
		buffer = (char*)receiveString();
	
		ack = buffer[0];
		if(ack != ACK){
			printf("[NACK] Something is wrong with the address sending.\n");
			return;
		} else{
			printf("[ACK] address ok\n");
			uint8_t lengthChecksum;
			lengthChecksum = readByteLength ^ 0xFF;
			/* Send the length of data to be read*/
			sendByte(readByteLength);
			/* Send the checksum of length*/
			sendByte(lengthChecksum);
			
			buffer = (char*)receiveString();
	
			ack = buffer[0];
			if(ack != ACK){
				printf("[NACK] Something is wrong with length sending.\n");
				return;
			} else {
				printf("[ACK] length ok\n");
				int i;
				for(i = 1; i < readByteLength + 2; i++){
					if(i % 8 == 1){
						printf("\n0x%8x:", startAddress + i - 1);
					}
					printf(" %02x", buffer[i]);					
				}				 
			}
		}
	}	
}

void eraseMemory(uint8_t numberOfPage){
	char ack;
	char cmd[] = {0x44, 0xBB};
	char* buffer; 	
		
	sendString(cmd, sizeof(cmd));	
	buffer = (char*)receiveString();
	
	ack = buffer[0];
	if(ack != ACK){
		printf("[NACK] Bootloader is not able to process Erase Memory command.\n");
		return;
	} else {
		printf("[ACK] ready\n");
		uint8_t checksum = 0;
		
		/* 0 <= numberOfPage <= 254
		/  numberOfPage = 255 => global erase mode
		/  1 page = 128 bytes
		*/
		
		/* erase 1 pages*/
		sendByte(0x00);
		checksum ^= 0x00;
		sendByte(0x00);
		checksum ^= 0x00;
		
		/* send n = 1 + 1 pages in 2 bytes*/
		sendByte(0x00);
		checksum ^= 0x00;
		sendByte(0x00);
		checksum ^= 0x00;
		
	
		
		sendByte(checksum); /* checksum */
		
		buffer = (char*)receiveString();
				
		ack = buffer[0];
		if(ack != ACK){
			printf("[NACK] Something is wrong with the number of page to be erased.\n");
			return;
		} else{
			printf("[ACK] Erase complete.\n");			
		}
	}	
}

void writeMemory(hexRecord hRecord){
	char ack;
	char cmd[] = {0x31, 0xCE};
	char* buffer; 	
		
	sendString(cmd, sizeof(cmd));	
	buffer = (char*)receiveString();
	
	ack = buffer[0];
	if(ack != ACK){
		printf("[NACK] Bootloader is not able to process Write Memory command.\n");
		return;
	} else {
		printf("[ACK] ready\n");
		uint32_t startAddress;
		uint8_t writeByteLength;
			
		startAddress 	= 0x08000000 + hRecord.address;
		writeByteLength = hRecord.length - 1;
			
		sendMemoryAddress(startAddress);		
		buffer = (char*)receiveString();
	
		ack = buffer[0];
		if(ack != ACK){
			printf("[NACK] Something is wrong with the address sending.\n");
			return;
		} else{
			printf("[ACK] address ok\n");			
			uint8_t i;	
			uint8_t dataChecksum = 0;
			dataChecksum ^= writeByteLength;		
			/* Send the length of data to be read*/
			sendByte(writeByteLength);
			
			/* Send data with length + 1*/
			for(i = 0; i < writeByteLength + 1; i++){
				sendByte(hRecord.data[i]);
				dataChecksum ^= hRecord.data[i];
				/*sendByte(0x00);
				dataChecksum ^= 0x00;*/
			}			
			
			/* Send the checksum of length*/
			sendByte(dataChecksum);
			
			buffer = (char*)receiveString();
	
			ack = buffer[0];
			if(ack != ACK){
				printf("[NACK] Something is wrong with length sending.\n");
				return;
			} else {
				printf("[ACK] Write commplete.\n");		 
			}
		}
	}	
}

