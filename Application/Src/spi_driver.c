#include "../Include/spi_driver.h"
#include "../Include/gpio_driver.h"


void spiInit(SPI_TypeDef *spiX, uint32_t mode, uint32_t baudrate, uint32_t direction, uint32_t cpha, uint32_t cpol, uint32_t size, uint32_t firstBit){
	gpioInit(GPIOB, SPI_NSS, OUTPUT, PUSHPULL, HIGHSPEED, NOPULL, NOAF);
	
	gpioInit(GPIOB, SPI_CLK, ALTFUNCTION, PUSHPULL, HIGHSPEED, NOPULL, AF5);
	gpioInit(GPIOB, SPI_MISO, ALTFUNCTION, PUSHPULL, HIGHSPEED, NOPULL, AF5);
	gpioInit(GPIOB, SPI_MOSI, ALTFUNCTION, PUSHPULL, HIGHSPEED, NOPULL, AF5);

	
	/* Turn on the clock for the GPIO whose pin to be configured */
	if(spiX == SPI1)	_SPI1_CLK_ENABLE();
	else if(spiX == SPI2)	_SPI2_CLK_ENABLE();
	else if(spiX == SPI3)	_SPI3_CLK_ENABLE();
	
	//Slave deselect
	gpioPinSet(GPIOB, SPI_NSS);
	
	/* Configure mode: master/slave */
	spiX->CR1 |= mode;	
	/* Configure baudrate prescaler */
	spiX->CR1 |= baudrate;
	/* Configure direction */
	spiX->CR1 |= direction;
	/* Configure clock phase */
	spiX->CR1 |= cpha;
	/* Configure clock polarity */
	spiX->CR1 |= cpol;
	/* Configure data size */
	spiX->CR1 |= size;
	/* Configure first bit mode */
	spiX->CR1 |= firstBit;
	
	/* Enable SPI */
	spiX->CR1 |= 1 << 6;
}


uint8_t spiSend(SPI_TypeDef *spiX, uint8_t data){
	spiX->DR = data;
	//Wait until the data has been transmitted.
	while (!(SPI1->SR & SPI_I2S_FLAG_TXE));
	// Wait for any data on MISO pin to be received.
	while (!(SPI1->SR & SPI_I2S_FLAG_RXNE));
	//All data transmitted/received but SPI may be busy so wait until done.
	while (SPI1->SR & SPI_I2S_FLAG_BSY);
	
	return(SPI1->DR);
}
