#include "../inc/hex_parser.h"


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

void testbyteInterpret(void){
	uint8_t xxx;
	uint8_t m, n;
	uint8_t hexa[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	for(m = 0; m < sizeof(hexa); m++){
		for(n = 0; n < sizeof(hexa); n++){
			xxx = byteInterpret(hexa[m], hexa[n]);
			printf("%c%c = %0.2x\n", hexa[m], hexa[n], xxx);
		}
	}    
}

void readHexFile(void){
	uint32_t j = 0;
	uint32_t readIndex = 0;
	uint8_t buffer[256] = {'\0'};
	
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);    
    
    hFile = CreateFile("myfile.txt",    /* open MYFILE.TXT  */
                GENERIC_WRITE | GENERIC_READ,              
                FILE_SHARE_READ,               /* share for reading  */
                NULL,                          /* no security  */
                OPEN_EXISTING,                   /* existing file only  */
                FILE_ATTRIBUTE_NORMAL,         /* normal file  */
                NULL);                         /* no attr */    
	
	status = ReadFile(hFile,buffer,sizeof(buffer),&byteWritten,NULL);    
    if(status)
    {
     	/*WriteFile(hStdOut,buffer,sizeof(buffer),NULL,NULL);*/
     	while (*(buffer + j) != '\0'){
   			printf("%c", *(buffer + j));
   			j++;
   		}    	
		   /*	
   		while(readIndex < sizeof(buffer)){
			while(buffer[readIndex] != ':' && readIndex < sizeof(buffer)){
   				readIndex++;
  			}
  			if(buffer[readIndex] == ':'){
  				readHexRecord(buffer, readIndex);
			}	
			readIndex++;	
		} 	*/	  	
    } else {
    	printf("[Error] Reading file failed\n");
    	
	}

}

hexRecord readHexRecord(uint8_t hex[], uint32_t readIndex){
	uint8_t j;
	/* Set all struct variables to 0*/
	hexRecord record;
	record.length = 0;
	record.address = 0;
	record.type = 0;
	for(j = 0; j < 20; j++){
		record.data[j] = 0;
	}
	record.checksum = 0;	
	
	record.length = byteInterpret(hex[readIndex + 1], hex[readIndex + 2]);
	/* address is 2 bytes */
	record.address |= (byteInterpret(hex[readIndex + 3], hex[readIndex + 4])) << 8;
	record.address |= byteInterpret(hex[readIndex + 5], hex[readIndex + 6]) << 0;
	record.type = byteInterpret(hex[readIndex + 7], hex[readIndex + 8]);

	for(j = 0; j < record.length ;j++){
		record.data[j] = byteInterpret(hex[readIndex + 9 + 2*j], hex[readIndex + 10 + 2*j]);
	}
	
	record.checksum = byteInterpret(hex[readIndex + 9 + 2*record.length], hex[readIndex + 10 + 2*record.length]);   
    /*
	printf(":%0.2x", record.length);
	printf("%0.4x", record.address);
	printf("%0.2x", record.type);   
	for(j = 0; j < record.length ;j++){
		printf("%0.2x", record.data[j]);
	}   
	printf("%0.2x\n", record.checksum);
	*/
	
	return record;
}
