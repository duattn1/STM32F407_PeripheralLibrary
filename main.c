#include "stm32f4xx.h"
#include "gpio_driver.h"

#define GREENLED			12
#define ORANGELED			13
#define REDLED				14
#define BLUELED				15

void ledInit(GPIO_TypeDef *gpioX, uint8_t ledColor);
void turnLedOn(GPIO_TypeDef *gpioX, uint8_t ledColor);

int main(void){
	ledInit(GPIOD, BLUELED);
	turnLedOn(GPIOD, BLUELED);
	return 0;
}

void ledInit(GPIO_TypeDef *gpioX, uint8_t ledColor){
	GPIOD_CLK_ENABLE()
	
	gpioPinConfig pinConfig;
	pinConfig.pin = ledColor;
	pinConfig.mode = OUTPUT;
	pinConfig.outputType = PUSHPULL;
	pinConfig.outputSpeed = MEDIUMSPEED;
	pinConfig.pupd = NOPULL;
	gpioInit(gpioX, &pinConfig);
}

void turnLedOn(GPIO_TypeDef *gpioX, uint8_t ledColor){
	gpioWriteToPin(gpioX, ledColor, 1);
}
