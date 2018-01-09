#include "../Include/gpio_driver.h"
#include "../Include/uart_driver.h"

#include "../UnitTesting/ut_main.h"
//#include "../UnitTesting/unity/unity.h"
//#include "../UnitTesting/ut_gpio_driver.h"

#define PORTB_USART3_TX			10
#define PORTB_USART3_RX			11


int main(void){
	/*//Init PB10: Tx and PB11: Rx
	gpioInit(GPIOB, PORTB_USART3_TX, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);
	gpioInit(GPIOB, PORTB_USART3_RX, ALTFUNCTION, PUSHPULL, VERYHIGHSPEED, NOPULL, AF7);	

	uartInit(USART3, IS_8_BITS_DATA, IS_1_STOP_BIT, DISABLE_PARITY, OVER_BY_16, 9600);	
	
	sendString(USART3, "con heo", 7);
	
	
	//gpioInit(GPIOD, REDLED, OUTPUT, PUSHPULL, LOWSPEED, NOPULL, NOAF);
	//turnLedOn(GPIOD, REDLED);*/
	
	
	runAllTest();
	return 0;
}

