#include "ut_gpio_driver.h"


void (*f[])(GPIO_TypeDef *gpioX) = {
	ut_gpioInitNoAF_1,
	ut_gpioInitNoAF_2,
	ut_gpioInitNoAF_3,
	ut_gpioInitNoAF_4
};

uint8_t numberOfTestcase = sizeof(f)/sizeof(int);

void ut_gpioInitNoAF_1(GPIO_TypeDef *gpioX){		
	tc_gpioInit_noAF(gpioX, 0, INPUT, PUSHPULL, LOWSPEED, NOPULL, NOAF);
}

void ut_gpioInitNoAF_2(GPIO_TypeDef *gpioX){		
	tc_gpioInit_noAF(gpioX, 1, OUTPUT, OPENDRAIN, MEDIUMSPEED, PULLUP, NOAF);	
}

void ut_gpioInitNoAF_3(GPIO_TypeDef *gpioX){		
	tc_gpioInit_noAF(gpioX, 2, ANALOG, PUSHPULL, HIGHSPEED, PULLDOWN, NOAF);	
}

void ut_gpioInitNoAF_4(GPIO_TypeDef *gpioX){		
	tc_gpioInit_noAF(gpioX, 3, INPUT, OPENDRAIN, VERYHIGHSPEED, NOPULL, NOAF);
}

void tc_gpioInit_noAF(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction){
	
	gpioInit(gpioX, pinNumber, mode, type, speedType, pullType, altFunction);
	
	//pin Number
	//mode << (2*pinNo)
	//otype << pinNo
	//ospeed << (2*pinNo)
	//pupd << (2*pinNo)
	TEST_ASSERT_EQUAL_HEX32( mode << (2*pinNumber), 			PIN_0_CONFIG_BY_2BIT_MASK & readRegister(&GPIOA->MODER));
	TEST_ASSERT_EQUAL_HEX32( type << pinNumber, 					PIN_0_CONFIG_BY_1BIT_MASK & readRegister(&GPIOA->OTYPER));
	TEST_ASSERT_EQUAL_HEX32( speedType << (2*pinNumber), 	PIN_0_CONFIG_BY_2BIT_MASK & readRegister(&GPIOA->OSPEEDR));
	TEST_ASSERT_EQUAL_HEX32( pullType << (2*pinNumber) , 	PIN_0_CONFIG_BY_2BIT_MASK & readRegister(&GPIOA->PUPDR));
}

void test(void){	
	TEST_ASSERT_EQUAL_HEX32(9, 10);
}
