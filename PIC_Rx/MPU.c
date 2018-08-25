#include <xc.h>
#include "MPU.h"
#include "pinDefine.h"
#include "time.h"
#include "typeDefine.h"



void InitMPU(void)
{
	//Initialize Port Status
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
	
	//AnalogorDigital Setting(All Digital)
//	ANSEL  = 0x00;	//ADê›íË// AD setting
//	ANSELH = 0x00;	//ADê›íË// AD setting
	
	//Port I/O Setting 
    //       0b76543210
	TRISA  = 0b10000000;
	TRISB  = 0b00100000;
    TRISC  = 0b10011000;
    TRISD  = 0b00000000;
    TRISE  = 0b00000000;	

	//Port Initial Value Setting	
	PORTA  = 0x00;
	PORTB  = 0x00;
	PORTC  = 0x00;
    PORTD  = 0x00;
    PORTE  = 0x00;
}
void LEDOnOff(void){
    LED_Y_ON();
    __delay_us(HALF_INTERVAL);
    LED_Y_OFF();
    __delay_us(HALF_INTERVAL);
}
void debugLEDyellow(void){
    if(LED_YELLOW == HIGH){
        LED_Y_OFF();
    }else{
        LED_Y_ON();
    }
}

//Used to switch PIN to the opposite status(high/low)
//bit invertState(bit pinState){
UINT invertState(UINT pinState){
    if(pinState==HIGH){
        return LOW;
    }else{
        return HIGH;
    }
}
//process command data if the command type is 'change satellite mode'
void commandSwitchSatMode(UBYTE command, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case 0x00: //Nominal mode //ON: CIB, EPS, OBC, Tx(CW), Rx
            //TODO: write method for nominal mode
            break;
        case 0x0F: //Power saving mode //ON: CIB, Tx(CW), Rx //OFF: EPS, OBC
            //TODO: write method for power saving mode
            //TODO: first kill EPS (this also kills Rx/Tx/OBC)
            //TODO: send command to TXCOBC to turn back on RX and TX
            //TODO: RXCOBC reset PLL data
            break;
        case 0xFF: //Survival mode //ON: CIB //OFF: EPS, OBC, Tx(CW), Rx
            //only enter if time in survival mode is specified //set automatical revival time
            //if time has run out switch to power saving mode
            //TODO: write method for survival mode
            break;
        default:
            //TODO: error message
            break;
    }
}

//process command data if the command type is 'power supply'
void commandSwitchPowerSupply(UBYTE command, UBYTE onOff, UBYTE timeHigh, UBYTE timeLow){ //times are given in ms
    switch(command){    
        case '5': //5R8G
            if(timeLow = 0x00){
                POWER_5R8G =invertState(POWER_5R8G);
            }else{
                POWER_5R8G =invertState(POWER_5R8G);
                        //insert wait function
                POWER_5R8G =invertState(POWER_5R8G);
            }
            break;
        case 'e': //EPS
            //TODO: write method for EPS using pins: SEP_SW and RBF_SW
            break;
        case 'o': //OBC
            //TODO: write method for OBC using pin: POWER_OBC
            break;
        case 'w': //WDT
            //TODO: write method for WDT using pin: POWER_WDT
            break;
        default:
            //TODO: error message
            break;
    }
}

//process command data if the command type is 'internal processing'
void commandSwitchIntProcess(UBYTE command, UBYTE data1, UBYTE data2){ 
    switch(command){    
        case 'i': //change in/out
            //TODO: write method for change in/out
            break;
        case 'h': //change high/low
            //TODO: write method for change high/low
            break;
        case 'o': //change frequency
            //TODO: write method for change frequency
            break;
        default:
            //TODO: error message
            break;
    }
}