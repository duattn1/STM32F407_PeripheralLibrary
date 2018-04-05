#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

HANDLE hComm;                          /* Handle to the Serial port */
char  ComPortName[] = "\\\\.\\COM4";  /*Name of the Serial port(May Change) to be opened, */
BOOL  Status;                          /* Status of the various operations  */
DWORD dwEventMask;                    /* Event mask to trigger */
char  TempChar;                       /* Temperory Character */
char  SerialBuffer[256];               /* Buffer Containing Rxed Data */
DWORD NoBytesRead;                     /*Bytes read by ReadFile() */
int i = 0;

typedef enum {
	ACK = 0x79,
	NACK = 0x1F
} ack;

void printCMDMenu(void);


void serialPortInit(void);
char receiveByte(void);
char* receiveString(void);
void sendString(char lpBuffer[], int length);
void startBootloader(void);
void getBootloaderVersion(void);	
void readMemory(void);
	
void main(void){
	char bBuffer[] = {0x7F};
	serialPortInit();	

	Sleep(1000);
	startBootloader();
	
	while(1){
		int selectedCMD = 0;
		printCMDMenu();
		scanf("%d", &selectedCMD);
		switch(selectedCMD){
			case 1: getBootloaderVersion(); break;
			case 2: readMemory(); break;
			default: printf("Select again!\n");
		}
	}
	
	
	/*
	while(1){
		Status = WaitCommEvent(hComm, &dwEventMask, NULL); /*Wait for the character to be received 				
		if (Status == TRUE)
		{
			receiveString();
		}

	}	*/	
		
	CloseHandle(hComm); /*Closing the Serial Port*/
	_getch();
} 
	
void printCMDMenu(void){
	printf("Select command\n");
	printf("1. Get bootloader version\n");
	printf("2. Read memory\n");
}	

void startBootloader(void){
	char ack;
	char cmd[] = {0x7F};
	sendString(cmd, sizeof(cmd));
	
	ack = receiveByte();
	if(ack == ACK){
		printf("Bootloader is waiting for commands.\n");
	} else {
		printf("Bootloader CANNOT receive commands\n");
	}
	printf("--------------------\n");			
}

void getBootloaderVersion(void){
	char ack;
	uint16_t PID = 0;
	char cmd[] = {0x02, 0xFD};
	char* buffer; 
	
	sendString(cmd, sizeof(cmd));	
	buffer = receiveString();
	
	ack = buffer[0];
	if(ack != ACK){
		printf("[Error] Getting version failed.\n");
		return;
	}	
	PID |= buffer[2] << 8;
	PID |= buffer[3];
	
	printf("PID: %0.4x\n", PID);	
}

void readMemory(void){
	char ack;
	char cmd[] = {0x02, 0xFD};
	char* buffer; 
	
	sendString(cmd, sizeof(cmd));	
	buffer = receiveString();
	
	ack = buffer[0];
		
}
		
void serialPortInit(void){
	/*---------------------------------- Opening the Serial Port -------------------------------------------*/			
	hComm = CreateFile( ComPortName,                  /* Name of the Port to be Opened*/
		                GENERIC_READ | GENERIC_WRITE, /* Read/Write Access */
						0,                            /* No Sharing, ports cant be shared */
						NULL,                         /* No Security */
					    OPEN_EXISTING,                /* Open existing port only */
						0,                            /* Non Overlapped I/O */
						NULL);                        /* Null for Comm Devices */

	if (hComm == INVALID_HANDLE_VALUE){
		printf("[Error] Port %s can't be opened\n", ComPortName);
	}	

	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/			
	DCB dcbSerialParams = { 0 };                         /* Initializing DCB structure */
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);      /* retreives  the current settings */

	if (Status == FALSE){
		printf("[Error] in GetCommState()\n");
	}
		
	dcbSerialParams.BaudRate = CBR_115200;      /* Setting BaudRate = 115200 */
	dcbSerialParams.ByteSize = 8;             /* Setting ByteSize = 8 */
	dcbSerialParams.StopBits = ONESTOPBIT;    /* Setting StopBits = 1 */
	dcbSerialParams.Parity = EVENPARITY;        /* Setting Parity = EVENPARITY - even parity ;NOPARITY - None;   */

	Status = SetCommState(hComm, &dcbSerialParams);  /*Configuring the port according to settings in DCB  */

	if (Status == FALSE)
	{
		printf("[Error] in Setting DCB Structure\n");
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/
			
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
			
	if (SetCommTimeouts(hComm, &timeouts) == FALSE){
		printf("[Error] in Setting Time Outs\n");
	}				
			
	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/			
	Status = SetCommMask(hComm, EV_RXCHAR); /* Configure Windows to Monitor the serial device for Character Reception */
	
	if (Status == FALSE){
		printf("[Error] in Setting CommMask\n");
	}
}

char receiveByte(void){
	char receivedChar;
	Status = WaitCommEvent(hComm, &dwEventMask, NULL);  				
	if (Status == TRUE)
	{
		Status = ReadFile(hComm, &receivedChar, sizeof(TempChar), &NoBytesRead, NULL);
	}
	return receivedChar;
}
		
char* receiveString(void){
	do
	{
		Status = ReadFile(hComm, &TempChar, sizeof(TempChar), &NoBytesRead, NULL);
		SerialBuffer[i] = TempChar;
		i++;
	} while (NoBytesRead > 0);
	
	i = 0; /* reset buffer counter */	
	return SerialBuffer;
}	

void sendString(char lpBuffer[], int length){
	DWORD  dNoOFBytestoWrite;              /* No of bytes to write into the port */
	DWORD  dNoOfBytesWritten = 0;          /* No of bytes written to the port */

	Status = WriteFile(	hComm,               /* Handle to the Serialport */
						lpBuffer,            /* Data to be written to the port  */
						length,   /* No of bytes to write into the port */
						&dNoOfBytesWritten,  /* No of bytes written to the port */
						NULL);
		
	if (Status != TRUE){
		printf("[Error] in Writing to Serial Port",GetLastError());
	}			
}	
