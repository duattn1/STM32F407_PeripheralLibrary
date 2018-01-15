/** @file gpio_driver.c
 *  @brief Function implementation for the gpio driver.
 *
 *  This contains the function implementation for the gpio driver.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 * 	@date    	02-Jan-2018
 */
 

#include "../Include/gpio_driver.h"

void gpioInit(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction){
	
	/* Turn on the clock for the GPIO whose pin to be configured */
	if(gpioX == GPIOA)	_GPIOA_CLK_ENABLE();
	else if(gpioX == GPIOB)	_GPIOB_CLK_ENABLE();
	else if(gpioX == GPIOC)	_GPIOC_CLK_ENABLE();
	else if(gpioX == GPIOD)	_GPIOD_CLK_ENABLE();
	
	/* Configure the pin mode */
	gpioX->MODER &= ~(0x03 << (2*pinNumber));
	gpioX->MODER |= mode << (2*pinNumber);
	
	/* Configure the pin output type */
	gpioX->OTYPER |= type << pinNumber;
	
	/* Configure the pin output speed */
	gpioX->OSPEEDR &= ~(0x03 << (2*pinNumber));
	gpioX->OSPEEDR |= speedType << (2*pinNumber);
	
	/* Configure the pin pull-up/ pull-down */
	gpioX->PUPDR &= ~(0x03 << (2*pinNumber));
	gpioX->PUPDR |= pullType << (2*pinNumber);
	
	/**
	 *	Configure the pin alternate function if pin mode is AF 
	 *	Each pin has 4 bit to configure the alternate function.
	 *	Therefore, 16 pins of each port are configured with 4*16 = 64 bits 
	 *	(equals to 2 register: AFRL & AFRH) 
	 */
	if(pinNumber < 8){
		gpioX->AFR[0] &= ~(0x0f << (4 * pinNumber));
		gpioX->AFR[0] |= altFunction << (4 * pinNumber);
	} else {
		gpioX->AFR[1] &= ~(0x0f << (4 * pinNumber - 32));
		gpioX->AFR[1] |= altFunction << (4 * pinNumber - 32);
	}
}

uint8_t gpioReadFromPin(GPIO_TypeDef *gpioX, uint8_t pinNumber){
	uint8_t value;
	value = (gpioX->IDR >> pinNumber) & 0x01;
	return value;
}

void gpioPinSet(GPIO_TypeDef *gpioX, uint8_t pinNumber){
	 gpioX->BSRR |= 1 << pinNumber;
}

void gpioPinReset(GPIO_TypeDef *gpioX, uint8_t pinNumber){
	 gpioX->BSRR |= 1 << (pinNumber + 16);
}

void gpioWriteToPort(GPIO_TypeDef *gpioX, uint32_t value){
	gpioX->ODR &= ~0x00001111;
	gpioX->ODR |= value;
}

uint32_t gpioPinConfigLock(GPIO_TypeDef *gpioX, uint8_t pinNumber){
	volatile uint32_t tmp = 0x00010000;
	tmp |= 1 << pinNumber;
	/* Set LCKK bit */
	gpioX->LCKR = tmp;
	/* Reset LCKK bit */
	gpioX->LCKR = (1 << pinNumber);
	/* Set LCKK bit */
	gpioX->LCKR = tmp;
	/* Read LCKK bit*/
	tmp = gpioX->LCKR;
  /* Read LCKK bit*/
  tmp = gpioX->LCKR;
	return tmp;
}
