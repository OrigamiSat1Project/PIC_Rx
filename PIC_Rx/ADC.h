/* 
 * File:   ADC.h
 * Author: kosuk
 *
 * Created on 2018/08/25, 16:55
 */

#ifndef ADC_H
#define	ADC_H

#include "typeDefine.h"

//UBYTE adcH = 0x00;
//UBYTE adcL = 0x00;
    
void initADC(void);
void ReadBatVoltage(void);
void SendBatVoltage(void);

void ReadBatVoltageWithPointer(UBYTE*);

#endif	/* ADC_H */
