#ifndef HEX_PARSER_H_
#define HEX_PARSER_H_

#include <windows.h>
#include <stdint.h>
#include <stdio.h>


DWORD byteWritten;
HANDLE hStdOut;
HANDLE hFile;
BOOL status;

typedef struct {
	uint8_t length;
	uint16_t address;
	uint8_t type;
	uint8_t data[256]; /* STM32F$ Discovery support maximum 256 bytes for each write command in bootloader */
	uint8_t checksum;
} hexRecord;

uint8_t byteInterpret(uint8_t char1, uint8_t char2);

void testbyteInterpret(void);

void readHexFile(void);

hexRecord readHexRecord(uint8_t hex[], uint32_t readIndex);


#endif

