#include "gpio_driver.h"

void gpioPinModeConfig(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode){
	gpioX->MODER &= ~(0x03 << (2*pinNumber));
	gpioX->MODER |= mode << (2*pinNumber);
}

void gpioPinOuputTypeConfig(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t type){
	gpioX->OTYPER |= type << pinNumber;
}

void gpioPinOuputSpeedConfig(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t speedType){
	gpioX->OSPEEDR &= ~(0x03 << (2*pinNumber));
	gpioX->OSPEEDR |= speedType << (2*pinNumber);
}

void gpioPinPUPDConfig(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t pullType){
	gpioX->PUPDR &= ~(0x03 << (2*pinNumber));
	gpioX->PUPDR |= pullType << (2*pinNumber);
	
}

void gpioPinAltFunctionConfig(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t altFunction){
	if(pinNumber < 8){
		gpioX->AFR[0] &= ~(0x0f << (4 * pinNumber));
		gpioX->AFR[0] |= altFunction << (4 * pinNumber);
	} else {
		gpioX->AFR[1] &= ~(0x0f << (4 * pinNumber));
		gpioX->AFR[1] |= altFunction << (4 * pinNumber);
	}
}

void gpioInit(GPIO_TypeDef *gpioX, gpioPinConfig *pinConfig){
	gpioPinModeConfig(gpioX, pinConfig->pin, pinConfig->mode);
	gpioPinOuputTypeConfig(gpioX, pinConfig->pin, pinConfig->outputType);
	gpioPinOuputSpeedConfig(gpioX, pinConfig->pin, pinConfig->outputSpeed);
	gpioPinPUPDConfig(gpioX, pinConfig->pin, pinConfig->pupd);
	gpioPinAltFunctionConfig(gpioX, pinConfig->pin, pinConfig->altFunction);
}

uint8_t gpioReadFromPin(GPIO_TypeDef *gpioX, uint8_t pinNumber){
	uint8_t value;
	value = (gpioX->IDR >> pinNumber) & 0x01;
	return value;
}

void gpioWriteToPin(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t value){
	if(value){
		gpioX->ODR |= 1 << pinNumber;
	} else {
		gpioX->ODR &= ~(1 << pinNumber);
	}
}
