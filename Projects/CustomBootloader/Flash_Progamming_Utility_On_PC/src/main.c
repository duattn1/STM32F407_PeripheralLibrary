#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "../inc/serial_port_communication.h"
#include "../inc/hex_parser.h"

#define ACK							0x79
#define NACK						0x1F
#define BINARY_REQUEST				0x01


void sendImageData(void);
	
extern uint8_t hexBuffer[10*1024];	
	
void main(void){
	uint8_t receivedRequest = 0;
	uint32_t i = 0;
	bool isConnected;	
	
	
	isConnected = connectSerialPort();		
	if(isConnected == false){
		printf("Please check your connection and Restart the tool.\n");
		return;
	} else {
		serialPortInit();			
	}	

	Sleep(1000);
	
	while(1){
		receivedRequest = receiveByte();
		printf("%c", receivedRequest);
		if( receivedRequest == BINARY_REQUEST){
			sendImageData();
			break;
		}		
	}			
	
	CloseHandle(hComm); /*Closing the Serial Port*/

	_getch();
} 
	
void sendImageData(void){
	uint32_t readIndex = 0;    
    uint16_t baseAddress = 0;
    uint8_t i;
    uint32_t hexFileLength;
    char hexFileLocation[128];
    
    do{
    	printf("Enter hex file location: ");
    	scanf("%s", hexFileLocation);
    	
    	hexFileLength = readHexFile(hexFileLocation);
	} while (hexFileLength == 0);
    
   		
	
	
	while(readIndex < hexFileLength){
		while(hexBuffer[readIndex] != ':' && readIndex < hexFileLength){
   			readIndex++;
  		}
  		printf("----------\n");
  		if(hexBuffer[readIndex] == ':'){
  			hexRecord hRecord;  
			uint8_t ackValue = 0;  			
  			hRecord = readHexRecord(hexBuffer, readIndex);
  			int i;
  			do {
  				/* The order of sending each hex record infor to the STM32F4 board */				
  				sendByte(hRecord.length);
  				
  				uint8_t offset1stByte = (hRecord.offset & 0xFF00) >> 8;
  				uint8_t offset2ndByte = (hRecord.offset & 0x00FF) >> 0; 
  				
  				sendByte(offset1stByte);
  				sendByte(offset2ndByte);
  				sendByte(hRecord.type);
  				for(i = 0; i < hRecord.length; i++){
  					sendByte(hRecord.data[i]);  			
				}
				sendByte(hRecord.checksum);	
				
				/* Wait for ACK byte from STM32F4 after sending record data */
				ackValue = receiveByte();
				printf("- %02x,%02x, %02x  | ack = %x\n", offset1stByte, offset2ndByte, hRecord.checksum, ackValue); /* for debugging */
				Sleep(0);
			} while (ackValue != ACK);  			
			
		}			
		
		readIndex++;	
	}
	
}
		

