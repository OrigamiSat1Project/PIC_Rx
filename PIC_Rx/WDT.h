/* 
 * File:   WDT.h
 * Author: Kosuke
 *
 * Created on 2018/01/18, 11:45
 */

#ifndef WDT_H
#define	WDT_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "typeDefine.h"

#define HIGH 1
#define LOW  0

void commandWDT(UBYTE, UBYTE, UBYTE);
void resetWDT(void);
void sendPulseWDT(void);

#endif	/* WDT_H */

