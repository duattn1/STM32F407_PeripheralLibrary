#include "stm32f4xx.h"
#include "gpio_driver.h"

#define NOAF 					0

#define GREENLED			12
#define ORANGELED			13
#define REDLED				14
#define BLUELED				15

void turnLedOn(GPIO_TypeDef *gpioX, uint8_t ledColor);

int main(void){
	gpioInit(GPIOD, BLUELED, OUTPUT, PUSHPULL, MEDIUMSPEED, NOPULL, NOAF);
	turnLedOn(GPIOD, BLUELED);
	return 0;
}

void turnLedOn(GPIO_TypeDef *gpioX, uint8_t ledColor){
	gpioWriteToPin(gpioX, ledColor, 1);
}
