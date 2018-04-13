/** @file serial_port_communication.h
 *  @brief Function prototypes for the Serial port communication on Windows
 *
 *  This contains the function prototypes for the Serial port communication on Windows
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */

#ifndef SERIAL_PORT_COMMUNICATION_H_
#define SERIAL_PORT_COMMUNICATION_H_

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

HANDLE hComm;                         /* Handle to the Serial port */
BOOL  Status;                         /* Status of the various operations  */
DWORD dwEventMask;                    /* Event mask to trigger */
char  TempChar;                       /* Temperory Character */
char  SerialBuffer[256];              /* Buffer Containing Rxed Data */
DWORD NoBytesRead;                    /*Bytes read by ReadFile() */
int bufferCounter;


/** @brief Attempt to connect the Serial port
 *
 *  @param void
 *
 *  @return bool
 */
bool connectSerialPort(void);

/** @brief Init Serial port communication after connect seccessfully
 *
 *  @param void
 *
 *  @return void
 */
void serialPortInit(void);

/** @brief Receive one byte from Serial port
 *
 *  @param void
 *
 *  @return char
 */
char receiveByte(void);

/** @brief Receive multiple bytes from Serial port
 *
 *  @param void
 *
 *  @return char*
 */
char* receiveString(void);

/** @brief Send one byte to Serial port
 *
 *  @param data The byte content want to send
 *
 *  @return void
 */
void sendByte(uint8_t data);

/** @brief Send multiple bytes to Serial port
 *
 *  @param lpBuffer The array of bytes want to send
 *  @param length The array length 
 *
 *  @return void
 */
void sendString(char lpBuffer[], int length);


#endif

