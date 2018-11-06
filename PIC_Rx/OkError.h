/* 
 * File:   OkError
 * Author: Madoka
 * Created on 2018/10/11
 */

#include <xc.h>
#include "typeDefine.h"

#ifndef OKERROR_H
#define	OKERROR_H

#define SUCCESS        1
#define FAILURE        0
#define RESET_STATUS   0

/*******************************************************************************
*Ok
******************************************************************************/
// #define ok_ADC_downlinkReceivedCommand_DcDc         0xaa
// #define ok_ADC_downlinkReceivedCommand_5VBus        0xab
// #define ok_ADC_downlinkReceivedCommand_3VBus        0xac
// #define ok_ADC_downlinkReceivedCommand_5VCIB        0xad
// #define ok_ADC_downlinkReceivedCommand_All          0xae

// #define ok_FMCW_commandSwitchCWDownlink_Start    0xfa
// #define ok_FMCW_commandSwitchCWDownlink_aa       0xfb
// #define ok_FMCW_commandSwitchCWDownlink_bb       0xfc

//#define ok_main_forOBCstatus_ALIVE                 0x1a
//#define ok_main_forOBCstatus_DIED                  0x1b
#define ok_main_crcCheck                           0x1c

/*******************************************************************************
*error
******************************************************************************/
#define no_error                                 0x00
// #define error_ADC_commandSwitchHKdata         0xa0
#define error_FMCW_commandSwitchFMCW             0xf0
#define error_I2C_commandSwitchI2C               0x20
#define error_I2C_commandSwitchEEPROM            0x21
#define error_main_reveiveCommand                0x11
#define error_main_crcCheck                      0x12
#define error_MPU_commandSwitchSatMode           0x90
#define error_MPU_commandSwitchPowerSupply       0x91
#define error_MPU_commandSwitchIntProcess        0x92
#define error_MPU_switchPowerSpply1pin           0x93
#define error_MPU_changeInOut                    0x94
#define error_MPU_changeHighLow                  0x95
#define error_initialOpe_powerShortage           0x30
#define error_initialOpe_obcAlive                0x31
#define error_initialOpe_satMode                 0x32
#define error_initialOpe_alreadyMelting          0x33
#define error_initialOpe_OBCstatus               0x34
#define error_initialOpe_readSatMode             0x35



#define error_UART_commandSwitchUART             0x30


void switchOk(UBYTE);
void switchError(UBYTE);

#endif	/* OKERROR_H */

