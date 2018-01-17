#include "ut_gpio_driver.h"

void (*f[])(GPIO_TypeDef*) = {
	ut_gpioInitNoAF_0,
	ut_gpioInitNoAF_1,
	ut_gpioInitNoAF_2,
	ut_gpioInitNoAF_3,
	ut_gpioInitAF_1,
	ut_gpioInitAF_2,
	ut_gpioInitAF_3,
	ut_gpioInitAF_4,
	ut_gpioInitAF_5,
	ut_gpioInitAF_6,
	ut_gpioInitAF_7,
	ut_gpioInitAF_8,
	ut_gpioInitAF_9,
	ut_gpioInitAF_10,
	ut_gpioInitAF_11
};

uint8_t numberOfTestcase = sizeof(f)/sizeof(int);

void ut_gpioInitNoAF_0(GPIO_TypeDef *gpioX){		
	tc_gpioInit(gpioX, 0, INPUT, PUSHPULL, LOWSPEED, NOPULL, NOAF);
}

void ut_gpioInitNoAF_1(GPIO_TypeDef *gpioX){		
	tc_gpioInit(gpioX, 1, OUTPUT, OPENDRAIN, MEDIUMSPEED, PULLUP, NOAF);	
}

void ut_gpioInitNoAF_2(GPIO_TypeDef *gpioX){		
	tc_gpioInit(gpioX, 2, ANALOG, PUSHPULL, HIGHSPEED, PULLDOWN, NOAF);	
}

void ut_gpioInitNoAF_3(GPIO_TypeDef *gpioX){		
	tc_gpioInit(gpioX, 3, INPUT, OPENDRAIN, VERYHIGHSPEED, NOPULL, NOAF);
}

/*
16 AF TCs

*/
void ut_gpioInitAF_0(GPIO_TypeDef *gpioX){		
	tc_gpioInit(gpioX, 0, ALTFUNCTION, PUSHPULL, LOWSPEED, NOPULL, AF0);
}

void ut_gpioInitAF_1(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 1, ALTFUNCTION, OPENDRAIN, MEDIUMSPEED, PULLUP, AF1);
}

void ut_gpioInitAF_2(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 2, ALTFUNCTION, PUSHPULL, HIGHSPEED, PULLDOWN, AF2);
}

void ut_gpioInitAF_3(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 3, ALTFUNCTION, OPENDRAIN, VERYHIGHSPEED, NOPULL, AF3);
}

void ut_gpioInitAF_4(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 4, ALTFUNCTION, PUSHPULL, LOWSPEED, PULLUP, AF4);
}

void ut_gpioInitAF_5(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 5, ALTFUNCTION, OPENDRAIN, MEDIUMSPEED, PULLDOWN, AF5);
}

void ut_gpioInitAF_6(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 6, ALTFUNCTION, PUSHPULL, HIGHSPEED, NOPULL, AF6);
}

void ut_gpioInitAF_7(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 7, ALTFUNCTION, OPENDRAIN, VERYHIGHSPEED, PULLUP, AF7);
}

void ut_gpioInitAF_8(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 8, ALTFUNCTION, PUSHPULL, LOWSPEED, PULLDOWN, AF8);
}

void ut_gpioInitAF_9(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 9, ALTFUNCTION, OPENDRAIN, MEDIUMSPEED, NOPULL, AF9);
}

void ut_gpioInitAF_10(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 10, ALTFUNCTION, PUSHPULL, HIGHSPEED, PULLUP, AF10);
}

void ut_gpioInitAF_11(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 11, ALTFUNCTION, OPENDRAIN, VERYHIGHSPEED, PULLDOWN, AF11);
}

void ut_gpioInitAF_12(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 12, ALTFUNCTION, PUSHPULL, LOWSPEED, NOPULL, AF12);
}

void ut_gpioInitAF_13(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 13, ALTFUNCTION, OPENDRAIN, MEDIUMSPEED, PULLUP, AF13);
}

void ut_gpioInitAF_14(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 14, ALTFUNCTION, PUSHPULL, HIGHSPEED, PULLDOWN, AF14);
}

void ut_gpioInitAF_15(GPIO_TypeDef *gpioX){	
	tc_gpioInit(gpioX, 15, ALTFUNCTION, OPENDRAIN, VERYHIGHSPEED, NOPULL, AF15);
}

void tc_gpioInit(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction){
	
	gpioInit(gpioX, pinNumber, mode, type, speedType, pullType, altFunction);

	TEST_ASSERT_EQUAL_HEX32( mode << (2*pinNumber), 			(PIN_x_CONFIG_BY_2BIT_MASK << 2*pinNumber) & readRegister(&GPIOA->MODER));
	TEST_ASSERT_EQUAL_HEX32( type << pinNumber, 					(PIN_x_CONFIG_BY_1BIT_MASK << pinNumber) & readRegister(&GPIOA->OTYPER));
	TEST_ASSERT_EQUAL_HEX32( speedType << (2*pinNumber), 	(PIN_x_CONFIG_BY_2BIT_MASK << 2*pinNumber) & readRegister(&GPIOA->OSPEEDR));
	TEST_ASSERT_EQUAL_HEX32( pullType << (2*pinNumber) , 	(PIN_x_CONFIG_BY_2BIT_MASK << 2*pinNumber) & readRegister(&GPIOA->PUPDR));

	if(mode == ALTFUNCTION){
		if(pinNumber < 8){
		//gpioX->AFR[0] &= ~(0x0f << (4 * pinNumber));
		//gpioX->AFR[0] |= altFunction << (4 * pinNumber);
		TEST_ASSERT_EQUAL_HEX32( altFunction << (4*pinNumber) , 	(PIN_x_CONFIG_BY_4BIT_MASK << 4*pinNumber) & readRegister(&GPIOA->AFR[0]));
	} else {
		//gpioX->AFR[1] &= ~(0x0f << (4 * pinNumber - 32));
		//gpioX->AFR[1] |= altFunction << (4 * pinNumber - 32);
		TEST_ASSERT_EQUAL_HEX32( altFunction << (4*pinNumber - 32) , 	(PIN_x_CONFIG_BY_4BIT_MASK << (4*pinNumber - 32)) & readRegister(&GPIOA->AFR[1]));

	}
	}
}

void test(int x){	
	TEST_ASSERT_EQUAL_HEX32(x, 10);
}