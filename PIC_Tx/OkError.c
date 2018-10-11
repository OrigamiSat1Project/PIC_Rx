/*
 * File:   OkError
 * Author: Madoka
 * Revision history: v1.0
 */

#include "Type_define.h"
#include "UART.h"
#include "time.h"
#include "ADC.h"
#include "EEPROM.h"
#include "I2C.h"
#include "OkError"
#include <xc.h>
#include <stdio.h>

void changeStatusSuccess(UBYTE action_select, UBYTE high_address, UBYTE low_address){
    action_select = SUCCESS;
}