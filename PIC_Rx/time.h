/* 
 * File:   time.h
 * Author: Reo
 *
 * Created on 2017/01/21, 18:49
 */


#ifndef TIME_H
#define	TIME_H

#include "typeDefine.h"

#define _XTAL_FREQ       10000000       // Clock frequency
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#define HALF_INTERVAL   400   // half the time interval of one bit //decided by measurement
     

//functions to wait [ms] or [us]
void delay_ms(UWORD);
void delay_us(UWORD);

//timer counter only getChar
void set_timer_counter_only_getChar(UINT);
UINT get_timer_counter_only_getChar(void);

#endif	/* TIME_H */

