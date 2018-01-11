#include "../Include/gpio_driver.h"
#include "../Include/uart_driver.h"

#include "../UnitTesting/ut_main.h"

#define PORTB_USART3_TX			10
#define PORTB_USART3_RX			11


int main(void){
	gpioInit(GPIOD, 14, OUTPUT, PUSHPULL, VERYHIGHSPEED, NOPULL, NOAF);
	gpioWriteToPin(GPIOD, 14, 1);
	
	
	
	/*//Init PB10: Tx and PB11: Rx
	gpioInit(GPIOB, PORTB_USART3_TX, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	gpioInit(GPIOB, PORTB_USART3_RX, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);	

	uartInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);		
	sendString(USART3, "con heo", 7);
	*/
	
	/*spiInit(SPI2, SPI_MASTER, SPI_CLOCK_DIV_2, SPI_UNIDIRECTION, SPI_FIRST_CLOCK, SPI_CK_TO_0, SPI_8BITS_DATA, SPI_MSB_FIRST);
	
	gpioWriteToPin(GPIOB, SPI_NSS, 0);
	spiSend(SPI2, 'd');
	gpioWriteToPin(GPIOB, SPI_NSS, 1);
	*/
	
	runAllTest();
	return 0;
}

