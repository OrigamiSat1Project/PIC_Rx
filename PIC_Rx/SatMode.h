/* 
 * File:   SatMode.h
 * Author: Yuki
 *
 * Created on 2018/11/07, 13:00
 */

#ifndef SATMODE_H
#define	SATMODE_H

    
#include "typeDefine.h"

#define SATMODE_NOMINAL   0x50 
#define SATMODE_SAVING    0x60 
#define SATMODE_SURVIVAL  0xA0 

#define SATMODE_NOMINAL_SEPON_RBFON   0x5A
#define SATMODE_SAVING_SEPOFF_RBFON   0x66
#define SATMODE_SURVIVAL_SEPOFF_RBFON 0xA6

#define SEP_ON  0x08
#define SEP_OFF 0x04
#define RBF_ON  0x02
#define SEP_OFF 0x01

//initial nominal_saving thershold BatVol is 7.5V (0x02,0x1D)
#define Init_TheresholdBatVol_nominal_saving_high 0x02//7.5V
#define Init_TheresholdBatVol_nominal_saving_low 0x1D
//initial saving_nominal thershold BatVol is 7.5V (0x02,0x08)
#define Init_TheresholdBatVol_saving_survival_high 0x02//7.2V
#define Init_TheresholdBatVol_saving_survival_low 0x08
//initial nominal revival thershold BatVol is 7.8V (0x02,0x32)
#define Init_TheresholdBatVol_nominal_revival_high 0x02
#define Init_TheresholdBatVol_nominal_revival_low 0x32
//initial saving revival thershold BatVol is 7.5V (0x02,0x08)
#define Init_TheresholdBatVol_saving_revival_high 0x02
#define Init_TheresholdBatVol_saving_revival_low 0x08
    
UBYTE MeasureBatVoltageAndChangeSatMode(void);
UBYTE BitCount(UBYTE);
void SwitchToSavingMode(void);
    

#endif	/* SATMODE_H */

