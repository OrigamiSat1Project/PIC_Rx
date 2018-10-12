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
*OK and ERROR
******************************************************************************/
#define B0select_for_commandID     0x00
#define HighAddress_for_commandID  0xE0
#define LowAddress_for_commandID   0x00

#define B0select_for_Ok       0x00
#define HighAddress_for_Ok    0xE0
#define LowAddress_for_Ok     0x00

#define B0select_for_Error     0x00
#define HighAddress_for_Error  0xE0
#define LowAddress_for_Error   0x00

/*******************************************************************************
*Last Command ID
******************************************************************************/
#define B0select_for_TXCOBCLastCommand     0x00
#define HighAddress_for_TXCOBCLastCommand  0xE0
#define LowAddress_for_TXCOBCLastCommand   0x00


/*******************************************************************************
*ADC value
******************************************************************************/
//TODO:change address
#define adcValue_addressHigh 0x00
#define adcValue_addressLow  0xE0

//ch1
#define adcValue_CH1_DATAHIGH_addressHigh 0x00
#define adcValue_CH1_DATAHIGH_addressLow  0x1F
#define adcValue_CH1_DATALOW_addressHigh 0x00
#define adcValue_CH1_DATALOW_addressLow  0x2F
//ch2
#define adcValue_CH2_DATAHIGH_addressHigh 0x00
#define adcValue_CH2_DATAHIGH_addressLow  0x3F
#define adcValue_CH2_DATALOW_addressHigh 0x00
#define adcValue_CH2_DATALOW_addressLow  0x4F
//ch3
#define adcValue_CH3_DATAHIGH_addressHigh 0x00
#define adcValue_CH3_DATAHIGH_addressLow  0x5F
#define adcValue_CH3_DATALOW_addressHigh 0x00
#define adcValue_CH3_DATALOW_addressLow  0x6F
//ch4
#define adcValue_CH4_DATAHIGH_addressHigh 0x00
#define adcValue_CH4_DATAHIGH_addressLow  0x7F
#define adcValue_CH4_DATALOW_addressHigh 0x00
#define adcValue_CH4_DATALOW_addressLow  0x8F

/*******************************************************************************
*address for satellite mode
******************************************************************************/
//TODO:change address
#define sattelliteMode_EEPROMAndB0Select 0x00
#define sattelliteMode_addressHigh       0x00
#define sattelliteMode_addressLow        0x00
#define sattelliteMode_DataSize          1

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

