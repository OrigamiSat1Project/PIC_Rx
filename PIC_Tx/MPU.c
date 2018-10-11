#include <xc.h>
#include "MPU.h"
#include "Type_define.h"
#include "time.h"
//#include "decode_AX25.h"
#include "encode_AX25.h"
#include "pinDefine.h"
#include "OkError.h"


UINT invertStateWithTime(UINT,UBYTE,UBYTE);

void Init_MPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
	ANSEL  = 0x00;	//AD?¿½Ý’ï¿½
	ANSELH = 0x00;	//AD?¿½Ý’ï¿½
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00010000;
    TRISC  = 0b01011100;
    //TRISD  = 0b00000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    //PORTD  = 0x00;
    PORTE  = 0x00;
}

//void led_onoff(void){
//    ledy_on();
//    __delay_us(espan);
//    ledy_off();
//    __delay_us(espan);
//}
//void debug_ledy(void){
//    if(led_yellow == high){
//        ledy_off();
//    }else{
//        ledy_on();
//    }
//}

//Used to switch PIN to the opposite status(high/low)
UINT invertStateWithTIme(UINT pinState,UBYTE timeHigh, UBYTE timeLow){
    if (timeHigh != 0x00 && timeLow != 0x00){
        UWORD operationTime;
        operationTime = (UWORD)timeHigh * 0x100 + timeLow;
        delay_ms(operationTime);
        pinState = 1 - pinState;
    }else{
        return pinState;
    }
}

void cutWire(UBYTE timeHigh, UBYTE timeLow){
    UWORD cutTime;
    cutTime = (UWORD)timeHigh * 0x100 + timeLow;
    WIRE_CUTTER = 1;
    delay_ms(cutTime);
    WIRE_CUTTER = 0;
}

//process command data if the command type is 'power supply'
void commandSwitchPowerSupply(UBYTE command, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){
        case 'h':   //Heater
            HEATER = onOff;
            HEATER = invertStateWithTIme(HEATER,timeHigh, timeLow);
            break;
        case 'n':   //NTRX
            NTRX = onOff;
            NTRX = invertStateWithTIme(NTRX,timeHigh, timeLow);
            break;
        case '5': //5R8G
            SW_5R8G = onOff;
            HEATER = invertStateWithTIme(SW_5R8G,timeHigh, timeLow);
            break;
        case 'a': //WIRE_CUTTER
            WIRE_CUTTER = onOff;
            HEATER = invertStateWithTIme(WIRE_CUTTER,timeHigh, timeLow);
            break;
        case 'w': //WDT
            WDT_POWER = 1 - onOff;
            HEATER = invertStateWithTIme(WIRE_CUTTER,timeHigh, timeLow);
            break;
        case 'p': //FMPTT
            FMPTT = onOff;
            HEATER = invertStateWithTIme(FMPTT,timeHigh, timeLow);
            break;
        case 'k': //CWKEY
            CWKEY = onOff;
            HEATER = invertStateWithTIme(CWKEY,timeHigh, timeLow);
            break;
        default:
            switchError(error_MPU_commandSwitchPowerSupply);
            break;
    }
}

