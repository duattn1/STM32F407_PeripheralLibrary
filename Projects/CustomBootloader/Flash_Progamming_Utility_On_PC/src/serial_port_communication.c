#include "../inc/serial_port_communication.h"

char  ComPortName[] = "\\\\.\\COM4";  /*Name of the Serial port(May Change) to be opened, */


void serialPortInit(void){
	/*---------------------------------- Opening the Serial Port -------------------------------------------*/		
	printf("Connecting with serial port....\n\n");	
	hComm = CreateFile( ComPortName,                  /* Name of the Port to be Opened*/
		                GENERIC_READ | GENERIC_WRITE, /* Read/Write Access */
						0,                            /* No Sharing, ports cant be shared */
						NULL,                         /* No Security */
					    OPEN_EXISTING,                /* Open existing port only */
						0,                            /* Non Overlapped I/O */
						NULL);                        /* Null for Comm Devices */

	if (hComm == INVALID_HANDLE_VALUE){
		printf("[Error] Port %s can't be opened\n", ComPortName);
		return;
	}	else {
		printf("Connect succesfully.\n");	
	}

	/*------------------------------- Setting the Parameters for the SerialPort ------------------------------*/			
	DCB dcbSerialParams = { 0 };                         /* Initializing DCB structure */
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);      /* retreives  the current settings */

	if (Status == FALSE){
		printf("[Error] GetCommState() failed.\n");
	}
		
	dcbSerialParams.BaudRate = CBR_9600;      /* Setting BaudRate = 115200 */
	dcbSerialParams.ByteSize = 8;             /* Setting ByteSize = 8 */
	dcbSerialParams.StopBits = ONESTOPBIT;    /* Setting StopBits = 1 */
	dcbSerialParams.Parity = NOPARITY;        /* Setting Parity = EVENPARITY - even parity ;NOPARITY - None;   */

	Status = SetCommState(hComm, &dcbSerialParams);  /*Configuring the port according to settings in DCB  */

	if (Status == FALSE)
	{
		printf("[Error] Setting DCB structure failed\n");
	}

	/*------------------------------------ Setting Timeouts --------------------------------------------------*/
			
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;
	timeouts.ReadTotalTimeoutConstant    = 50;
	timeouts.ReadTotalTimeoutMultiplier  = 10;
	timeouts.WriteTotalTimeoutConstant   = 50;
	timeouts.WriteTotalTimeoutMultiplier = 10;
			
	if (SetCommTimeouts(hComm, &timeouts) == FALSE){
		printf("[Error] Setting timeout failed.\n");
	}				
			
	/*------------------------------------ Setting Receive Mask ----------------------------------------------*/			
	Status = SetCommMask(hComm, EV_RXCHAR); /* Configure Windows to Monitor the serial device for Character Reception */
	
	if (Status == FALSE){
		printf("[Error] Setting CommMask failed\n");
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

void sendByte(uint8_t data){
	DWORD  dNoOfBytesWritten = 0; 			/* No of bytes written to the port */
	Status = WriteFile(	hComm,               /* Handle to the Serialport */
						&data,            /* Data to be written to the port  */
						1,   /* No of bytes to write into the port */
						&dNoOfBytesWritten,  /* No of bytes written to the port */
						NULL);
		
	if (Status != TRUE){
		printf("[Error] Sending byte failed.\n",GetLastError());
	}			
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
		printf("[Error] Sending string failed.\n",GetLastError());
	}			
}	
