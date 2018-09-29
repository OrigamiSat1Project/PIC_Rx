

#ifndef TIMER_H
#define	TIMER_H

#include "typeDefine.h"

#define _XTAL_FREQ       10000000       // Clock frequency
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

int timer_counter = 0;

void initTimer(void);
//void interrupt TimerReset(void);
void interrupt TimerCheck(void);

#endif	/* TIME_H */