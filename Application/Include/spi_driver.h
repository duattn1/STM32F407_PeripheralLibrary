#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include <stdint.h>
#include "stm32f4xx.h"

#define _SPI1_CLK_ENABLE() 				(RCC->APB2ENR |= RCC_APB2ENR_SPI1EN)
#define _SPI2_CLK_ENABLE() 				(RCC->APB1ENR |= RCC_APB1ENR_SPI2EN)
#define _SPI3_CLK_ENABLE() 				(RCC->APB1ENR |= RCC_APB1ENR_SPI3EN)

void spiInit();
/** @brief Configure a UART.
 *
 *  @param uartX The address of UART to be configured.
 *  @param wordLength The length of data bits.
 *  @param stopBitNumber The number of stop bit(s).
 *  @param parityEnable Selection to enable/disable the parity check.
 *  @param oversampling Selection of oversampling rate by 8 or 16.
 *  @param baudrate The baudrate to be configured.
 *
 *  @return void.
 */

#endif /*  SPI_DRIVER_H_ */