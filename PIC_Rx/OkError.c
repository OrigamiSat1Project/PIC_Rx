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

//void switchOk(UBYTE action_select){
//    UBYTE mainAddress;
//    UBYTE subAddress;
//    UBYTE Ok_data[2];
//    mainAddress = MAIN_EEPROM_ADDRESS | B0select_for_Ok;
//    subAddress = SUB_EEPROM_ADDRESS | B0select_for_Ok;
//    /*Ok_data[0]->coomandID*/
//    Ok_data[0] = ReadEEPROM(mainAddress, HighAddress_for_commandID, LowAddress_for_commandID);
//    Ok_data[1] = action_select;
//    WriteToEEPROM(MAIN_EEPROM_ADDRESS, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
//    WriteToEEPROM(SUB_EEPROM_ADDRESS, HighAddress_for_Ok, LowAddress_for_Ok, Ok_data);
//    //for debug
//    put_ok();
//    putChar(Ok_data[0]);
//    putChar(action_select);
//}

void switchError(UBYTE action_select){

    /*error_data[0]->coomandID*/
    UBYTE error_data[2];
    error_data[0] = ReadEEPROM(MAIN_EEPROM_ADDRESS, HighAddress_for_commandID, LowAddress_for_commandID);
    error_data[1] = action_select;
    WriteOneByteToMainAndSubB0EEPROM(HighAddress_for_Error, LowAddress_for_Error, error_data);
    
    //for debug
    put_error();
//    putChar(error_data[0]);
//    putChar(action_select);
    
    //TODO:need debug
    //only FM downlink? need CW?
//    UBYTE downlinlTimes = 36;
//    UBYTE DataSize = 2;
//    UBYTE send_command[8];
//    send_command[0] = 't';
//    send_command[1] = 'f';
//    send_command[2] = 0xaa;
//    send_command[3] = B0select_for_Error;
//    send_command[4] = HighAddress_for_Error;
//    send_command[5] = LowAddress_for_Error;
//    send_command[6] = downlinlTimes;
//    send_command[7] = DataSize;
//    sendCommandByPointer(send_command); 
//    sendCommand('t','f',0xaa,B0select_for_Error,HighAddress_for_Error,LowAddress_for_Error,downlinlTimes,DataSize);
}
