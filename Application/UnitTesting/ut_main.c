#include "ut_main.h"

GPIO_TypeDef *stm32f4Port[] = {
	GPIOA,
	GPIOB,
	GPIOC
};

void setUp(){
	
}
	
void tearDown(){
	
}


int runAllTest(void){	
	UNITY_BEGIN();
		/* GPIO Driver UnitTest */
	for(uint8_t j = 0; j < 2; j++){
		printf("portX: %u\n", stm32f4Port[j]);
		for(uint8_t i = 0; i < numberOfTestcase; i++){
			//RUN_TEST((ptr[i]));			
			RUN_TEST_ON_PORTX((f[i]), stm32f4Port[j]);			
		}		
	}

	return UNITY_END();
}
