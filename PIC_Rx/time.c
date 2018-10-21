    /*** Timer controll ***/

#include <htc.h>
#include "typeDefine.h"
#include "time.h"
/**/

////Global
//static UINT timer_counter_only_getChar = 0;

void delay_s(UWORD sec)
{
	while(sec)
	{
		__delay_s(1);
		sec--;
	}	
}


//wait [ms] function
void delay_ms(UWORD msec)
{
	while(msec)
	{
		__delay_ms(1);
		msec--;
	}	
}

//wait [us] function
void delay_us(UWORD usec)
{
	while(usec)
	{
		__delay_us(1);
		usec--;
	}	
}

////timer counter only getChar
//void set_timer_counter_only_getChar(UINT time){
//    timer_counter_only_getChar = time;
//}
//
//UINT get_timer_counter_only_getChar(void){
//    return timer_counter_only_getChar;
//}
