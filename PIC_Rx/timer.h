

#ifndef TIMER_H
#define	TIMER_H

#include "typeDefine.h"

#define _XTAL_FREQ       10000000       // Clock frequency
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#define one_second 38
#define one_minute 60
#define one_hour 60
#define one_day 24
#define one_week 7

int timer_counter = 0;
int second_counter = 0;
int minute_counter = 0;
int hour_counter = 0;
int day_counter = 0;

void initTimer(void);
void interrupt TimerCheck(void);
//void interrupt TimerReset(void);

#endif	/* TIME_H */