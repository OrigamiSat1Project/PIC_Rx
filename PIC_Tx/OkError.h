/* 
 * File:   OkError
 * Author: Madoka
 * Created on 2018/10/11
 */

#include <xc.h>
#include "Type_define.h"

#ifndef OKERROR_H
#define	OKERRORC_H

#define SUCCESS        1
#define FAILURE        0
#define RESET_STATUS   0

/*******************************************************************************
*Ok
******************************************************************************/
#define ok_ADC_downlinkReceivedCommand_DcDc         0xaa
#define ok_ADC_downlinkReceivedCommand_5VBus        0xab
#define ok_ADC_downlinkReceivedCommand_3VBus        0xac
#define ok_ADC_downlinkReceivedCommand_5VCIB        0xad
#define ok_ADC_downlinkReceivedCommand_All          0xae

#define ok_FMCW_commandSwitchCWDownlink_Start    0xfa
#define ok_FMCW_commandSwitchCWDownlink_aa       0xfb
#define ok_FMCW_commandSwitchCWDownlink_bb       0xfc


/*******************************************************************************
*error
******************************************************************************/
#define error_ADC_commandSwitchHKdata         0xa0
#define error_FMCW_downlinkReceivedCommand    0xf0
#define error_FMCW_commandSwitchCWDownlink    0xf1
#define error_I2C_commandSwitchI2C            0x20
#define error_MPU_commandSwitchPowerSupply    0x90
#define error_main_crcCheck                   0x10
#define error_main_commandfromOBCorRXCOBC     0x11


void switchOk(UBYTE);
void switchError(UBYTE);
void putErrorNoDownlink(UBYTE);

#endif	/* OKERROR_H */

