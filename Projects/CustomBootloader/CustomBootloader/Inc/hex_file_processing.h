/** @file hex_file_processing.h
 *  @brief Function prototypes for the hex file processing.
 *
 *  This contains the function prototypes for the hex file processing
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */
 
#ifndef HEX_FILE_PROCESSING_H_
#define HEX_FILE_PROCESSING_H_

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/* structure of a hex record*/
typedef struct {
	uint8_t length;
	uint16_t offset;
	uint8_t type;
	uint8_t data[256];
	uint8_t checksum;
} hexRecord;

extern void FLASHEraseSector(uint8_t sectorNumber);
extern void FLASHWrite(uint32_t startAddress, uint32_t endAddress, uint8_t buffer[]);
extern void FLASHRead(uint32_t startAddress, uint32_t endAddress);

/** @brief Convert hex number in text format to hex number format
 *
 *  @param char1 The first character
 *  @param char2 The second character
 *
 *  @return uint8_t
 */
uint8_t byteInterpret(uint8_t char1, uint8_t char2);

/** @brief Read a tring and interpret into a Intel hex record
 *
 *  @param hex The array of characters to be interpret
 *  @param readIndex The index in the string which contains many hex records
 *
 *  @return hexRecord
 */
hexRecord readHexRecord(uint8_t hex[], uint32_t readIndex);

/** @brief Program a hex record into FLASH 
 *
 *  @param baseAddress Base address of binary file
 *  @param length The length of binary file
 *  @param binaryImage The content of binanary file
 *
 *  @return void
 */
void programBinaryImage(uint32_t baseAddress, uint32_t length, uint8_t binaryImage[]);

#endif /* GPIO_DRIVER_H_ */
