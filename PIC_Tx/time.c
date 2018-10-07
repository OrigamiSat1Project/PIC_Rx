/*** éûä‘êßå‰ ***/

#include <htc.h>
#include "Type_define.h"
#include "time.h"
/**/

static UINT timer_counter_only_getChar = 0;

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

//wait [s] function
void delay_s(UWORD sec)
{
	while(sec)
	{
		__delay_s(1);
		sec--;
	}	
}

//timer counter only getChar
void set_timer_counter_only_getChar(UINT time){
    timer_counter_only_getChar = time;
}

UINT get_timer_counter_only_getChar(void){
    return timer_counter_only_getChar;
}