/** @file interrupt_controller.h
 *  @brief Function prototypes for interrupt.
 *
 *  This contains the function prototypes for interrupt
 *	and also macros, constants, variables needed.
 *
 *  @author 	Tran Nhat Duat (duattn)
 *	@version 	V0.1
 */
 
#ifndef INTERRUPT_CONTROLLER_H_
#define INTERRUPT_CONTROLLER_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>
#include "GPIO_driver.h"


#define	EXTI0 		0	


#define EXTI_RISING_TRIGGER_DISABLE 	0
#define EXTI_RISING_TRIGGER_ENABLE 		1	

#define EXTIx_PA		0
#define EXTIx_PB		1
#define EXTIx_PC		2
#define EXTIx_PD		3
#define EXTIx_PE		4
#define EXTIx_PF		5
#define EXTIx_PG		6
#define EXTIx_PH		7
#define EXTIx_PI		8
#define EXTIx_PJ		9
#define EXTIx_PK		10


/** @brief setup NVIC for all interrupts
 *
 *  @param void.
 *
 *  @return void.
 */
void NVICSetup(void);

/** @brief configure all used GPIO interrupts in application.
 *
 *  @param void.
 *
 *  @return void.
 */
void GPIOInterruptConfig(void);

void extiConfig(uint8_t extiLine, uint8_t risingTriggerEnable);

void extiSourceSelect(uint8_t extiLine, uint8_t extiSource);

#endif /* INTERRUPT_CONTROLLER_H_ */
