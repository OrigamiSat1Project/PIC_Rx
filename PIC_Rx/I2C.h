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

#define I2C_baud_rate_high 400000  //400kbps
#define I2C_baud_rate_low  100000  //100kbps
#define I2C_baud_rate_def  I2C_baud_rate_low

void InitI2CMaster(const UDWORD c);
void I2CMasterWait(void);                                               
void I2CMasterStart(void);                                              
void I2CMasterRepeatedStart(void);                                   
void I2CMasterStop(void);                                               
void I2CMasterWrite(unsigned d);                                        
UBYTE I2CMasterRead(UBYTE a);  

/*write*/
void WriteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE *data);
void WriteToEEPROMWithDataSize(UBYTE, UBYTE ,UBYTE, UBYTE*, UBYTE);
void WriteOneByteToEEPROM(UBYTE addressEEPROM,UBYTE addressHigh,UBYTE addressLow,UBYTE data);
void WriteCheckByteToEEPROMs(UBYTE B0Select,UBYTE addressHigh,UBYTE addressLow,UBYTE data);
void WriteLastCommandIdToEEPROM(UBYTE last_command_ID);

/*read*/
UBYTE ReadEEPROM(UBYTE, UBYTE, UBYTE);
void ReadDataFromEEPROMWithDataSize(UBYTE, UBYTE, UBYTE , UBYTE*, UINT);

void I2CBufferClear(void);
void ChangeI2CBaudRate(UBYTE);

void TestEEPROM(UBYTE);

void commandSwitchI2C(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);
void commandSwitchEEPROM(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UBYTE);


#endif	/* I2C_H */

