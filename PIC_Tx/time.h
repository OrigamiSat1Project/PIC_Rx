/* 
 * File:   time.h
 * Author: Curie
 *
 * Created on 2017/01/21, 18:49
 */

#include "Type_define.h"
#ifndef TIME_H
#define	TIME_H

#define _XTAL_FREQ       10000000       // Clock frequency

//#define __delay_ms(1)    _delay((unsigned long)((1)*(_XTAL_FREQ/4000UL)))
#define __delay_us(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define __delay_ms(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4000.0)))
#define __delay_s(x) _delay((unsigned long)((x)*(_XTAL_FREQ/4.0)))

#define span 833
#define espan 795//10MHz:795-803 20MHz:817   811-823
#define hspan 400   //20MHz:360(300?),415
#define lspan 1250
#define TURN_ON_WAIT_TIME  2
#define CW_START_WAIT_TIME 200

#define WIRE_CUT_INTERVAL 3 //[s] 

//?øΩs?øΩ?øΩ?øΩm?øΩ?øΩdelay?øΩ÷êÔøΩ
//functions to wait [ms] or [us]
void delay_ms(UWORD);
void delay_us(UWORD);
void delay_s(UWORD);

UWORD calTime2Byte(UBYTE, UBYTE);

//timer counter only getChar
void set_timer_counter_only_getChar(UINT);
UINT get_timer_counter_only_getChar(void);

#endif	/* TIME_H */

