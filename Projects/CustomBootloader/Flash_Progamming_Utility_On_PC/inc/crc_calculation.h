/** @file crc_calculation.h
 *  @brief Function prototypes for the CRC calculation
 *
 *  This contains the function prototypes for the CRC calculation
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */
 
#ifndef CRC_CALCULATION_H_
#define CRC_CALCULATION_H_

#include <stdint.h>

/* The CRC table used for calculate CRC */
const uint32_t crc_table[256];

/** @brief Calculate CRC for an stream of data
 *
 *  @param initial The initial CRC value
 *  @param data	The stream of data to calculate CRC
 *  @param count Length of the data stream
 *
 *  @return uint32_t
 */
uint32_t crc32_update(uint32_t initial, uint32_t *data, uint32_t count);

#endif

