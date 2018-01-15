#ifndef UT_GPIO_DRIVER_H_
#define UT_GPIO_DRIVER_H_

#include "unity/unity.h"
#include "utilities/ut_utilities.h"
#include "../Include/gpio_driver.h"

//void x(GPIO_TypeDef *gpioX);

extern void (*f[])(GPIO_TypeDef*);
extern uint8_t numberOfTestcase;

typedef enum{
	PIN_x_CONFIG_BY_1BIT_MASK 		= 0x00000001, //0x01	
	PIN_x_CONFIG_BY_2BIT_MASK 		= 0x00000003, //0x03
	PIN_x_CONFIG_BY_4BIT_MASK			= 0x0000000F	//0x0F
} Mask;

void ut_gpioInitNoAF_0(GPIO_TypeDef *gpioX);
void ut_gpioInitNoAF_1(GPIO_TypeDef *gpioX);
void ut_gpioInitNoAF_2(GPIO_TypeDef *gpioX);
void ut_gpioInitNoAF_3(GPIO_TypeDef *gpioX);
/*
16 AF TCs

*/
void ut_gpioInitAF_0(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_1(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_2(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_3(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_4(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_5(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_6(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_7(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_8(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_9(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_10(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_11(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_12(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_13(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_14(GPIO_TypeDef *gpioX);
void ut_gpioInitAF_15(GPIO_TypeDef *gpioX);
	
void tc_gpioInit(GPIO_TypeDef *gpioX, uint8_t pinNumber, uint8_t mode, uint8_t type, uint8_t speedType, uint8_t pullType, uint8_t altFunction);
void test(int x);

#endif /* UT_GPIO_DRIVER_H_ */
