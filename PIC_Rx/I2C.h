/* 
 * File:   I2C.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 0:41
 */

#include <xc.h>
#include "Type_define.h"

#ifndef I2C_H
#define	I2C_H

#define I2Cbps 400000

void Init_I2C_M(const UDWORD c);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
void I2C_Master_Write(unsigned d);
UBYTE I2C_Master_Read(UBYTE a);
void EEPROM_Write(UBYTE ,UBYTE ,UBYTE ,UBYTE *);
UBYTE *EEPROM_Read(UBYTE ,UBYTE ,UBYTE , UINT);


#endif	/* I2C_H */

