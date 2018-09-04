/* 
 * File:   Initmpu.h
 * Author: Reo
 *
 * Created on 2017/01/21, 20:52
 */
#include <xc.h>  

#ifndef INITMPU_H
#define	INITMPU_H

#include "typeDefine.h"
//#include "pinDefine.h"

#define HIGH 1
#define LOW  0
#define LED_Y_ON() LED_YELLOW = HIGH;
#define LED_Y_OFF() LED_YELLOW = LOW;



void InitMPU(void);
void LEDOnOff(void);
void debugLEDyellow(void);
UINT invertState(UINT);
void killEPS(void);
void EPSOn(void);
void switchPowerSpply1pin(UINT, UBYTE, UBYTE, UBYTE);
void switchPowerSpply2pin(UINT, UINT, UBYTE, UBYTE, UBYTE);
void changeInOut(UINT, UBYTE);
void changeHighLow(UINT, UBYTE);
void changeXtalFrequency(UBYTE);
void commandSwitchSatMode(UBYTE, UBYTE, UBYTE);
void commandSwitchPowerSupply(UBYTE, UBYTE, UBYTE, UBYTE);
void commandSwitchIntProcess(UBYTE, UBYTE, UBYTE);
#endif /*INITMPU_H*/

