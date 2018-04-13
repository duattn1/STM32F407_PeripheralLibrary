/** @file hex_parser.h
 *  @brief Function prototypes for the hex parser utility
 *
 *  This contains the function prototypes for the hex parser utility
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */
 
#ifndef HEX_PARSER_H_
#define HEX_PARSER_H_

#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


DWORD byteWritten;
HANDLE hStdOut;
HANDLE hFile;
BOOL status;


/* Structure of a file record */
typedef struct {
	uint8_t length;
	uint16_t offset;
	uint8_t type;
	uint8_t data[256];
	uint8_t checksum;
} hexRecord;


/** @brief Intepret hex from character display to hex number
 *
 *  @param char1 The first character
 *  @param char2 The second character
 *
 *  @return uint8_t
 */
uint8_t byteInterpret(uint8_t char1, uint8_t char2);

/** @brief The test function for byteInterpret function
 *
 *  @param void
 *
 *  @return void
 */
void testbyteInterpret(void);

/** @brief Read a hex file stored in memory, then store in an array. Finally return the length of the read file
 *
 *  @param void
 *
 *  @return uint32_t
 */
uint32_t readHexFile(void);

/** @brief Read a array of hex file and interpret into 1 by 1 hex record
 *
 *  @param hex The array contains all content of hex file
 *  @param readIndex The index of hex file array 
 *
 *  @return hexRecord
 */
hexRecord readHexRecord(uint8_t hex[], uint32_t readIndex);


#endif

