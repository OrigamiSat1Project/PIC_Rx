/* 
 * File:   FMCW.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 17:45
 */

#ifndef FMCW_H
#define	FMCW_H

#include "Type_define.h"

//***Read Me!!!
//***comentout methods are defined in FMCW.c

/*Downlink Command*/
void downlinkReceivedCommand(UBYTE, UBYTE, UBYTE, UBYTE);

/*Function : FM and CW downkink */
void _NOP(void);

/*FM*/
void downlinkFMSignal(UBYTE, UBYTE,UBYTE, UBYTE, UBYTE, UBYTE);

/*Function : CW downlink*/
void Morse_V(void);
int changeCharMorse (char);
char changeBinaryToChar(UBYTE);
void DevideDataAndChangeBinaryToChar (UBYTE, UBYTE*);
void sendMorse(char*);
void sendMorsenew(char*,size_t);
void ReadOneByteDataFromEEPROMandSendMorse(UBYTE, UBYTE, UBYTE);
void ReadDatasFromEEPROMWithDataSizeAndSendMorse(UBYTE, UBYTE, UBYTE, UBYTE*, UINT);
void ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(UBYTE, UBYTE, UBYTE, UBYTE *, UINT, UBYTE);
// void CwDownlinkFR0(void);
// void CwDownlinkFR1(void);
// void CwDownlinkFR2(void);
// void CwDownlinkFRXXX(void);
void downlinkCWSignal(void);

#endif	/* FMCW_H */

