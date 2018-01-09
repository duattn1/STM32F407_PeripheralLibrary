#include "ut_main.h"

void setUp(){
	
}
	
void tearDown(){
	
}


int runAllTest(void){	
	UNITY_BEGIN();

	/* GPIO Driver UnitTest */
	for(uint8_t i; i < numberOfTestcase; i++){
		RUN_TEST(f[i]);
	}
	
	
	UNITY_END();
}