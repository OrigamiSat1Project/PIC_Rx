/* 
 * File:   I2C.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 0:41
 */

#include <xc.h>


#ifndef I2C_H
#define	I2C_H

#include "typeDefine.h"

#define I2Cbps 400000

void InitI2CMaster(const UDWORD c);
void WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data);
void Write1byteDataToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data);
UBYTE ReadEEPROM(UBYTE EEPROM_address,UBYTE high_address,UBYTE low_address);
void commandSwitchI2C(UBYTE, UBYTE, UBYTE*, UBYTE*);

#endif	/* I2C_H */

