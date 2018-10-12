/*
 * File:   OkError
 * Author: Madoka
 * Revision history: v1.0
 */

#include "Type_define.h"
#include "UART.h"
#include "time.h"
#include "EEPROM.h"
#include "I2C.h"
#include "OkError.h"
#include <xc.h>
#include <stdio.h>

void switchOk(UBYTE action_select){
    UBYTE mainAddress;
    UBYTE subAddress;
    UBYTE Ok_data[2];
    mainAddress = EEPROM_address | B0select_for_Ok;
    subAddress = EEPROM_subaddress | B0select_for_Ok;
    /*Ok_data[0]->coomandID*/
    Ok_data[0] = ReadEEPROM(mainAddress, HighAddress_for_commandID, LowAddress_for_commandID);
    Ok_data[1] = action_select;
    WriteToEEPROM(EEPROM_address, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
    WriteToEEPROM(EEPROM_subaddress, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
    //for debug
    put_ok();
    putChar(Ok_data[0]);
    putChar(action_select);
}

void switchError(UBYTE action_select){
    UBYTE mainAddress;
    UBYTE subAddress;
    UBYTE error_data[2];
    mainAddress = EEPROM_address | B0select_for_Error;
    subAddress = EEPROM_subaddress | B0select_for_Error;
    /*error_data[0]->coomandID*/
    error_data[0] = ReadEEPROM(mainAddress, HighAddress_for_commandID, LowAddress_for_commandID);
    error_data[1] = action_select;
    WriteToEEPROM(EEPROM_address, HighAddress_for_Error, LowAddress_for_Error, error_data);
    WriteToEEPROM(EEPROM_subaddress, HighAddress_for_Error, LowAddress_for_Error, error_data);
    //for debug
    put_error();
    putChar(error_data[0]);
    putChar(action_select);
}