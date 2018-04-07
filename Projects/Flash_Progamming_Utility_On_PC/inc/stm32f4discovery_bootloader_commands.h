#ifndef STM32F4DISCOVERY_BOOTLOADER_COMMANDS_H_
#define STM32F4DISCOVERY_BOOTLOADER_COMMANDS_H_

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include "hex_parser.h"

typedef enum {
	ACK = 0x79,
	NACK = 0x1F
} ack;



uint8_t calculateChecksum(int count, ...);

uint8_t startBootloader(void);

void getBootloaderVersion(void);
	
void sendMemoryAddress(uint32_t address);
	
void readMemory(void);

void eraseMemory(uint8_t numberOfPage);

void writeMemory(hexRecord hRecord);

#endif

