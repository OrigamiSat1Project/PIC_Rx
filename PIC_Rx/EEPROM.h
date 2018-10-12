/* 
 * File:   EEPROM.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 2:17
 */

#include <xc.h>


#ifndef EEPROM_H
#define	EEPROM_H

#include "typeDefine.h"

/*******************************************************************************
*EEPROM address
******************************************************************************/
#define MAIN_EEPROM_ADDRESS 0x50
#define SUB_EEPROM_ADDRESS 0x52

//for test EEPROM
//TODO:change address
#define datalength_for_test         4
#define B0select_for_testEEPROM     0x00
#define HighAddress_for_testEEPROM  0xE0
#define LowAddress_for_testEEPROM   0x00


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
#define B0select_for_RXCOBCLastCommand     0x00
#define HighAddress_for_RXCOBCLastCommand  0xE0
#define LowAddress_for_RXCOBCLastCommand   0x00

#endif	/* EEPROM_H */

