/* 
 * File:   WDT.h
 * Author: Kosuke
 *
 * Created on 2018/01/18, 11:45
 */

#ifndef WDT_H
#define	WDT_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define TIMER_INTERVAL (0xffff - 62500) // TMR1 // 10MHz, 1/4 100msec // TODO: useful comment

void InitWDT(void);
void commandWDT(UBYTE, UBYTE, UBYTE);

#endif	/* WDT_H */

