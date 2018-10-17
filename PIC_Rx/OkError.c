/*
 * File:   OkError
 * Author: Madoka
 * Revision history: v1.0
 */

#include "typeDefine.h"
#include "UART.h"
#include "EEPROM.h"
#include "FMCW.h"
#include "I2C.h"
#include "OkError.h"
#include <xc.h>
#include <stdio.h>

void switchOk(UBYTE action_select){
    UBYTE mainAddress;
    UBYTE subAddress;
    UBYTE Ok_data[2];
    mainAddress = MAIN_EEPROM_ADDRESS | B0select_for_Ok;
    subAddress = SUB_EEPROM_ADDRESS | B0select_for_Ok;
    /*Ok_data[0]->coomandID*/
    Ok_data[0] = ReadEEPROM(mainAddress, HighAddress_for_commandID, LowAddress_for_commandID);
    Ok_data[1] = action_select;
    WriteToEEPROM(MAIN_EEPROM_ADDRESS, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
    WriteToEEPROM(SUB_EEPROM_ADDRESS, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
    //for debug
    put_ok();
    putChar(Ok_data[0]);
    putChar(action_select);
}

void switchError(UBYTE action_select){
    UBYTE mainAddress;
    UBYTE subAddress;
    UBYTE error_data[2];
    mainAddress = MAIN_EEPROM_ADDRESS | B0select_for_Error;
    subAddress = SUB_EEPROM_ADDRESS | B0select_for_Error;
    /*error_data[0]->coomandID*/
    error_data[0] = ReadEEPROM(mainAddress, HighAddress_for_commandID, LowAddress_for_commandID);
    error_data[1] = action_select;
    WriteToEEPROM(MAIN_EEPROM_ADDRESS, HighAddress_for_Error, LowAddress_for_Error, error_data);
    WriteToEEPROM(SUB_EEPROM_ADDRESS, HighAddress_for_Error, LowAddress_for_Error, error_data);
    //for debug
    put_error();
    putChar(error_data[0]);
    putChar(action_select);
    //TODO:need debug
    //only FM downlink? need CW?
    UBYTE downlinlTimes = 36;
    UBYTE DataSize = 2; 
    sendCommand('t', 'f', 0xaa, B0select_for_Error, HighAddress_for_Error, LowAddress_for_Error, downlinlTimes, DataSize);
}

//TODO:need debug
//ex: 0b01101011 -> 0+1+1+0+1+0+1+1=5
UBYTE bitCalResult(UBYTE bit_input){
    UBYTE bit_cal_result = 0; 
    for(UBYTE cal_counter=0; cal_counter<8; cal_counter++){
        if((bit_input & 1)==1){
            bit_cal_result = bit_cal_result + 1;
        } else {
            bit_cal_result = bit_cal_result + 0;
        }
        bit_input = bit_input >>1;
    }
    return bit_cal_result;
}