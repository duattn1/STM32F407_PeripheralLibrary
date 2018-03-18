#include "../Include/gpio_driver.h"
#include "../Include/uart_driver.h"
#include "../Include/spi_driver.h"

#include "../Include/interrupt_controller.h"

#include "../UnitTesting/ut_main.h"

#define PORTB_USART3_TX			10
#define PORTB_USART3_RX			11

extern int runAllTest(void);

int main(void){
	//gpioInit(GPIOA, 0, INPUT, PUSHPULL, VERYHIGHSPEED, NOPULL, NOAF);
	/*gpioInit(GPIOD, 14, OUTPUT, PUSHPULL, VERYHIGHSPEED, NOPULL, NOAF);
	
	while(1){
		gpioPinSet(GPIOD, 14);
		gpioPinReset(GPIOD, 14);
				
	}	*/
	//baudrate: 8MHz
	//spiInit(SPI2, SPI_MASTER, SPI_CLOCK_DIV_2, SPI_1_LINE_BIDIRECTION, SPI_FIRST_CLOCK, SPI_CK_TO_0, SPI_8BITS_DATA, SPI_MSB_FIRST);
	
	//spiSend(SPI2, 'a');
	//gpioInit(GPIOD, 14, OUTPUT, PUSHPULL, VERYHIGHSPEED, NOPULL, NOAF);
	//gpioPinConfigLock(GPIOD, 14);
	//gpioPinSet(GPIOD, 14);
	
	/*
	//Init PB10: Tx and PB11: Rx
	gpioInit(GPIOB, usart3PortB.txPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	gpioInit(GPIOB, usart3PortB.rxPin, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);		
	
	uartInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);		
	nvicSetup();
	
	sendString(USART3, "con heo", 7);
	
	
	
	while(1){
		
	}
	*
	/*
	spiInit(SPI2, SPI_SLAVE, SPI_CLOCK_DIV_2, SPI_2_LINE_UNIDIRECTION, SPI_FIRST_CLOCK, SPI_CK_TO_1, SPI_8BITS_DATA, SPI_MSB_FIRST);
	nvicSetup();
	//gpioPinReset(GPIOB, SPI_NSS);
	//spiSend(SPI2, 'd');
	//gpioPinSet(GPIOB, SPI_NSS);
	while(1){
		
	}
	*/
	runAllTest();
	return 0;
}

