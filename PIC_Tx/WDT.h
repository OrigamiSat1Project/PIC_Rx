/* 
 * File:   WDT.h
 * Author: Kosuke
 *
 * Created on 2018/01/18, 11:45
 */

#ifndef WDT_H
#define	WDT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "Type_define.h"

#define TIMER_INTERVAL (0xffff - 62500) // TMR?????????????
                                        // 10MHz, 1/4????????100msec??????????
#define HIGH 1
#define LOW  0

void commandWDT(UBYTE);
void Init_WDT(void);
void resetWDT();
void sendPulseWDT(void);
//void interrupt intr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* WDT_H */

