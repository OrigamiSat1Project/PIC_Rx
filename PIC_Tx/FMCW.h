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

// #define SHORT_DELAYTIMES_FOR_MORSE     15  //80wpm
// #define MIDDLE_DELAYTIMES_FOR_MORSE    45
// #define LONG_DELAYTIMES_FOR_MORSE      105
#define SHORT_DELAYTIMES_FOR_MORSE     65800  //80wpm
#define MIDDLE_DELAYTIMES_FOR_MORSE    197400
#define LONG_DELAYTIMES_FOR_MORSE      460600
#define ADD_BLANK_FOR_MORSE            LONG_DELAYTIMES_FOR_MORSE-MIDDLE_DELAYTIMES_FOR_MORSE

/*Downlink Command*/
void downlinkReceivedCommand(UBYTE, UBYTE, UBYTE, UBYTE);

/*Function : FM and CW downkink */
void _NOP(void);

/*FM*/
void downlinkFMSignal(UBYTE,UBYTE, UBYTE, UBYTE, UBYTE);

/*Function : CW downlink*/
void downlinkCWSignal(void);
void commandSwitchCWDownlink(UBYTE, UBYTE, UBYTE, UBYTE, UBYTE, UINT, UBYTE);

/*for debug*/
void testForCwFunctions(void);

#endif	/* FMCW_H */

