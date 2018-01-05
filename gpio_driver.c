#include "gpio_driver.h"

void gpioInit(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction){
	_GPIOD_CLK_ENABLE();
	
	gpioX->MODER &= ~(0x03 << (2*pinNumber));
	gpioX->MODER |= mode << (2*pinNumber);
	gpioX->OTYPER |= type << pinNumber;
	gpioX->OSPEEDR &= ~(0x03 << (2*pinNumber));
	gpioX->OSPEEDR |= speedType << (2*pinNumber);
	gpioX->PUPDR &= ~(0x03 << (2*pinNumber));
	gpioX->PUPDR |= pullType << (2*pinNumber);
	if(pinNumber < 8){
		gpioX->AFR[0] &= ~(0x0f << (4 * pinNumber));
		gpioX->AFR[0] |= altFunction << (4 * pinNumber);
	} else {
		gpioX->AFR[1] &= ~(0x0f << (4 * pinNumber));
		gpioX->AFR[1] |= altFunction << (4 * pinNumber);
	}
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
