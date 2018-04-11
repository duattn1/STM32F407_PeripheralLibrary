#ifndef SERIAL_PORT_COMMUNICATION_H_
#define SERIAL_PORT_COMMUNICATION_H_

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

HANDLE hComm;                          /* Handle to the Serial port */
BOOL  Status;                          /* Status of the various operations  */
DWORD dwEventMask;                    /* Event mask to trigger */
char  TempChar;                       /* Temperory Character */
char  SerialBuffer[256];               /* Buffer Containing Rxed Data */
DWORD NoBytesRead;                     /*Bytes read by ReadFile() */
int i;

void serialPortInit(void);
char receiveByte(void);
char* receiveString(void);

void sendByte(uint8_t data);
void sendString(char lpBuffer[], int length);


#endif

