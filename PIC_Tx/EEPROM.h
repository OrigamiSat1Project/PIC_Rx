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
#define EEPROM_address_B1 0x54
#define EEPROM_subaddress 0x52
#define EEPROM_subMaddress_B1 0x56
#define whigh_address 0x00
#define wlow_address 0xE0
#define rhigh_address 0x00
#define rlow_address 0xE0

/*******************************************************************************
*OK and ERROR
******************************************************************************/
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
#define B0select_for_TXCOBCLastCommandID     0x00
#define HighAddress_for_TXCOBCLastCommandID  0xE0
#define LowAddress_for_TXCOBCLastCommandID   0x00
#define HighAddress_for_RXCOBCLastCommandID  0x00
#define LowAddress_for_RXCOBCLastCommandID   0x00

/*******************************************************************************
*CRC check
******************************************************************************/
//#define crcResult_B0select        0x00
#define crcResult_addressHigh     0xE0
#define crcResult_addressLow      0x00


/*******************************************************************************
*ADC value
******************************************************************************/
//TODO:change address
#define adcValue_addressHigh 0x00
#define adcValue_addressLow  0x00

//ch1 (Battery Temperature))
#define adcValue_CH1_addressHigh 0x00
#define adcValue_CH1_addressLow  0x00
//ch2 (5VBUS voltage(EPS 5V))
#define adcValue_CH2_addressHigh 0x00
#define adcValue_CH2_addressLow  0x00
//ch3 (3V3BUS voltage(EPS 3.3V))
#define adcValue_CH3_addressHigh 0x00
#define adcValue_CH3_addressLow  0x00
//ch4
#define adcValue_CH4_addressHigh 0x00
#define adcValue_CH4_addressLow  0x00

/*******************************************************************************
*address for satellite mode
******************************************************************************/
//TODO:change address
#define satelliteMode_EEPROMAndB0Select 0x00
#define satelliteMode_addressHigh       0x81//include SEP(3,2bit) and RBF(1,0bit)
#define satelliteMode_addressLow        0x00//include SEP(3,2bit) and RBF(1,0bit)
#define satelliteMode_DataSize          1

/*******************************************************************************
*CW downlink
******************************************************************************/

//TODO:change address
#define OBC_LastCommand_addressHigh     0x00
#define OBC_LastCommand_addressLow      0x00
#define RXCOBC_LastCommand_addressHigh  0x00
#define RXCOBC_LastCommand_addressLow   0x00
#define TXCOBC_LastCommand_addressHigh  0x00
#define TXCOBC_LastCommand_addressLow   0x00
#define RXCOBC_CommandErrorStatus_addressHigh 0x00
#define RXCOBC_CommandErrorStatus_addressLow 0x00
#define TXCOBC_CommandErrorStatus_addressHigh 0x00
#define TXCOBC_CommandErrorStatus_addressLow 0x00

//HK
#define BatteryVoltageOBC_addressHigh  0x00
#define BatteryVoltageOBC_addressLow 0x00
#define BatteryVoltageCIB_addressHigh  0x00
#define BatteryVoltageCIB_addressLow 0x00
#define MeltingStatus_addressHigh  0x85
#define MeltingStatus_addressLow   0x00
#define LatestExcutionCommandID_addressHigh   0x00
#define LatestExcutionCommandID_addressLow   0x00
#define OBC_CommandErrorStatus_addressHigh 0x00
#define OBC_CommandErrorStatus_addressLow 0x00
#define BatteryCurrent_addressHigh 0x00
#define BatteryCurrent_addressLow  0x00
#define EpsSwitchStatus_addressHigh   0x00
#define EpsSwitchStatus_addressLow   0x00
#define TxTemperature_addressHigh 0x00 
#define TxTemperature_addressLow 0x00 
#define RxTemperature_addressHigh 0x00 
#define RxTemperature_addressLow 0x00 

#define FreeData1Highaddress_addressHigh 0x00
#define FreeData1Highaddress_addressLow 0x00
#define FreeData1Lowaddress_addressHigh 0x00
#define FreeData1Lowaddress_addressLow 0x00
#define FreeData2Highaddress_addressHigh 0x00
#define FreeData2Highaddress_addressLow 0x00
#define FreeData2Lowaddress_addressHigh 0x00
#define FreeData2Lowaddress_addressLow 0x00

/*******************************************************************************
*Initial Operation
******************************************************************************/
#define HighAddress_for_meltingCompelationFlag      0xE0
#define LowAddress_for_meltingCompelationFlag       0x00

#endif	/* EEPROM_H */

