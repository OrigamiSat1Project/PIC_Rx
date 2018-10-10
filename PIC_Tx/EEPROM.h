/* 
 * File:   EEPROM.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 2:17
 */

#include <xc.h>
#include "Type_define.h"

#ifndef EEPROM_H
#define	EEPROM_H

/*******************************************************************************
*EEPROM address
******************************************************************************/
#define EEPROM_address 0x50
//#define EEPROM_Maddress 0x54
#define EEPROM_subaddress 0x52
//#define EEPROM_subMaddress 0x56
#define whigh_address 0x00
#define wlow_address 0xE0
#define rhigh_address 0x00
#define rlow_address 0xE0

/*******************************************************************************
*address for satellite mode
******************************************************************************/
//TODO:change address
#define deviceOnOff_EEPROMAndB0Select 0x00
#define deviceOnOff_addressHigh       0x00
#define deviceOnOff_addressLow        0x00
#define deviceOnOff_DataSize          1

/*******************************************************************************
*CW downlink
******************************************************************************/
//TODO:change address
#define batteryStatus_addressHigh       0x00
#define batteryStatus_addressLow        0x00
#define satelliteBusStatus_addressHigh  0x00
#define satelliteBusStatus_addressLow   0x00
#define satelliteMode_addressHigh       0x00
#define satelliteMode_addressLow        0x00
//TODO:change address
#define OBC_LastCommand_addressHigh     0x00
#define OBC_LastCommand_addressLow      0x00
#define RXCOBC_LastCommand_addressHigh  0x00
#define RXCOBC_LastCommand_addressLow   0x00
#define TXCOBC_LastCommand_addressHigh  0x00
#define TXCOBC_LastCommand_addressLow   0x00

#endif	/* EEPROM_H */

