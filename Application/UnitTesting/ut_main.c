#include "ut_main.h"

void setUp(){
	
}
	
void tearDown(){
	
}


int runAllTest(void){	
	UNITY_BEGIN();

	RUN_TEST(ut_gpioInit);
	
	UNITY_END();
}