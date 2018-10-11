/* 
 * File:   FMCW.h
 * Author: Kosuke
 *
 * Created on 2017/04/18, 17:45
 */

#ifndef FMCW_H
#define	FMCW_H

#include "typeDefine.h"

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
void Morse_Y(void);
void Morse_J(void);
long changeCharMorse (char);
char changeBinaryToChar(UBYTE);
void DevideDataAndChangeBinaryToChar (UBYTE, UBYTE*);
void sendMorse(char*,size_t);
void ReadOneByteDataFromEEPROMandSendMorse(UBYTE, UBYTE, UBYTE);
void ReadDatasFromEEPROMWithDataSizeAndSendMorse(UBYTE, UBYTE, UBYTE, UBYTE*, UINT);
void ReadDatasFromEEPROMWithDataSizeAndSendMorseWithDownlinkTimes(UBYTE, UBYTE, UBYTE, UBYTE *, UINT, UBYTE);
// void CwDownlinkFR0(void);
// void CwDownlinkFR1(void);
// void CwDownlinkFR2(void);
// void CwDownlinkFRXXX(void);
void downlinkCWSignal(void);

void testForCwFunctions(void);

#endif	/* FMCW_H */

