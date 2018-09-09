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
//XXX
#define datalength 4

void InitI2CMaster(const UDWORD c);
void I2CMasterWait(void);
void I2CMasterStart(void);
void I2CMasterRepeatedStart(void);
void I2CMasterStop(void);
void I2CMasterWrite(unsigned d);
UBYTE I2CMasterRead(UBYTE a);
void WriteToEEPROM(UBYTE ,UBYTE ,UBYTE ,UBYTE *);
void WriteToEEPROMWithDataSize(UBYTE ,UBYTE ,UBYTE ,UBYTE *, UBYTE);
//UBYTE *EEPROM_Read(UBYTE ,UBYTE ,UBYTE , UINT);
void ReadDataFromEEPROM(UBYTE Address7Bytes,UBYTE high_address,UBYTE low_address,UBYTE *ReadData, UINT EEPROMDataLength);

#endif	/* I2C_H */

