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

/*Initial Operation*/
#define OBC_ALIVE   HIGH
#define OBC_DIED    LOW
#define OnOff_forCutWIRE    0x01
#define hightime_forCutWIRE 0x0f
#define lowtime_forCutWIRE  0xff
#define cuttimes_forCutWIRE 3 

void InitMPU(void);

void LEDOnOff(void);
void debugLEDyellow(void);
UINT invertState(UINT);

void onOff5R8G(UBYTE, UBYTE, UBYTE);
void onOffOBC(UBYTE, UBYTE, UBYTE);
void onOffWDT(UBYTE, UBYTE, UBYTE);
void switchPowerSpply1pin(UBYTE, UBYTE, UBYTE, UBYTE);

void killEPS(void);
void onEPS(void);
void switchPowerEPS(UBYTE, UBYTE, UBYTE);
void reviveEPS(UBYTE, UBYTE);

void changeInOut(UINT, UBYTE);
void changeHighLow(UINT, UBYTE);
void changeXtalFrequency(UBYTE);

void commandSwitchSatMode(UBYTE, UBYTE, UBYTE);
void commandSwitchPowerSupply(UBYTE, UBYTE, UBYTE, UBYTE);
void commandSwitchIntProcess(UBYTE, UBYTE, UBYTE);
#endif /*INITMPU_H*/

