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
    
//BatMode survival <= 6.114V
#define BatVol_saving_survival_high 0x01
#define BatVol_saving_survival_Low 0xB9
#define BatVol_OBCrevival_high 0x02
#define BatVol_OBCrevival_Low 0x32

void initADC(void);
void ReadBatVoltage(void);
void SendBatVoltage(void);

void ReadBatVoltageWithPointer(UBYTE*);
void MeasureBatVoltageAnChangeSatMode(void);

#endif	/* ADC_H */
